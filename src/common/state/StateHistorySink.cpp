/* Copyright (c) 2014 Philippe Proulx <eepp.ca>
 *
 * This file is part of tigerbeetle.
 *
 * tigerbeetle is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * tigerbeetle is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with tigerbeetle.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <cstdint>
#include <boost/filesystem/path.hpp>
#include <delorean/BasicTypes.hpp>
#include <delorean/interval/AbstractInterval.hpp>
#include <delorean/interval/Int32Interval.hpp>
#include <delorean/interval/Uint32Interval.hpp>
#include <delorean/interval/Int64Interval.hpp>
#include <delorean/interval/Uint64Interval.hpp>
#include <delorean/interval/Float32Interval.hpp>
#include <delorean/interval/QuarkInterval.hpp>
#include <delorean/HistoryFileSink.hpp>

#include <common/state/StateValueType.hpp>
#include <common/state/StateHistorySink.hpp>
#include <common/state/CurrentState.hpp>
#include <common/state/Int32StateValue.hpp>
#include <common/state/Uint32StateValue.hpp>
#include <common/state/Int64StateValue.hpp>
#include <common/state/Uint64StateValue.hpp>
#include <common/state/Float32StateValue.hpp>
#include <common/state/QuarkStateValue.hpp>

namespace bfs = boost::filesystem;

namespace tibee
{
namespace common
{

StateHistorySink::StateHistorySink(const bfs::path& pathStrDbPath,
                                   const bfs::path& valueStrDbPath,
                                   const bfs::path& historyPath) :
    _pathStrDbPath {pathStrDbPath},
    _valueStrDbPath {valueStrDbPath},
    _historyPath {historyPath},
    _ts {0},
    _opened {false},
    _curPathQuark {0},
    _curStrValueQuark {0},
    _currentState {*this}
{
    _intervalFileSink = std::unique_ptr<delo::HistoryFileSink> {
        new delo::HistoryFileSink
    };
}

StateHistorySink::~StateHistorySink()
{
    this->close();
}

void StateHistorySink::initTranslators()
{
    auto unknownTranslator =
        [] (quark_t, const StateHistorySink::StateValueEntry&)
    {
        return nullptr;
    };

    auto int32Translator =
        [this] (quark_t pathQuark, const StateHistorySink::StateValueEntry& stateValueEntry)
    {
        auto interval = new delo::Int32Interval {
            static_cast<delo::timestamp_t>(stateValueEntry.beginTs),
            static_cast<delo::timestamp_t>(_ts),
            static_cast<delo::interval_key_t>(pathQuark)
        };

        interval->setValue(static_cast<const Int32StateValue&>(*stateValueEntry.value).getValue());

        return interval;
    };

    auto uint32Translator =
        [this] (quark_t pathQuark, const StateHistorySink::StateValueEntry& stateValueEntry)
    {
        auto interval = new delo::Uint32Interval {
            static_cast<delo::timestamp_t>(stateValueEntry.beginTs),
            static_cast<delo::timestamp_t>(_ts),
            static_cast<delo::interval_key_t>(pathQuark)
        };

        interval->setValue(static_cast<const Uint32StateValue&>(*stateValueEntry.value).getValue());

        return interval;
    };

    auto int64Translator =
        [this] (quark_t pathQuark, const StateHistorySink::StateValueEntry& stateValueEntry)
    {
        auto interval = new delo::Int64Interval {
            static_cast<delo::timestamp_t>(stateValueEntry.beginTs),
            static_cast<delo::timestamp_t>(_ts),
            static_cast<delo::interval_key_t>(pathQuark)
        };

        interval->setValue(static_cast<const Int64StateValue&>(*stateValueEntry.value).getValue());

        return interval;
    };

    auto uint64Translator =
        [this] (quark_t pathQuark, const StateHistorySink::StateValueEntry& stateValueEntry)
    {
        auto interval = new delo::Uint64Interval {
            static_cast<delo::timestamp_t>(stateValueEntry.beginTs),
            static_cast<delo::timestamp_t>(_ts),
            static_cast<delo::interval_key_t>(pathQuark)
        };

        interval->setValue(static_cast<const Uint64StateValue&>(*stateValueEntry.value).getValue());

        return interval;
    };

    auto float32Translator =
        [this] (quark_t pathQuark, const StateHistorySink::StateValueEntry& stateValueEntry)
    {
        auto interval = new delo::Float32Interval {
            static_cast<delo::timestamp_t>(stateValueEntry.beginTs),
            static_cast<delo::timestamp_t>(_ts),
            static_cast<delo::interval_key_t>(pathQuark)
        };

        interval->setValue(static_cast<const Float32StateValue&>(*stateValueEntry.value).getValue());

        return interval;
    };

    auto quarkTranslator =
        [this] (quark_t pathQuark, const StateHistorySink::StateValueEntry& stateValueEntry)
    {
        auto interval = new delo::QuarkInterval {
            static_cast<delo::timestamp_t>(stateValueEntry.beginTs),
            static_cast<delo::timestamp_t>(_ts),
            static_cast<delo::interval_key_t>(pathQuark)
        };

        interval->setValue(static_cast<const QuarkStateValue&>(*stateValueEntry.value).getValue());

        return interval;
    };

    // fill translators
    for (auto& translator : _translators) {
        translator = unknownTranslator;
    }

    _translators[static_cast<std::size_t>(StateValueType::INT32)] = int32Translator;
    _translators[static_cast<std::size_t>(StateValueType::UINT32)] = uint32Translator;
    _translators[static_cast<std::size_t>(StateValueType::INT64)] = int64Translator;
    _translators[static_cast<std::size_t>(StateValueType::UINT64)] = uint64Translator;
    _translators[static_cast<std::size_t>(StateValueType::FLOAT32)] = float32Translator;
    _translators[static_cast<std::size_t>(StateValueType::QUARK)] = quarkTranslator;
}

void StateHistorySink::open()
{
    // open history sink
    _intervalFileSink->open(_historyPath);
}

void StateHistorySink::close()
{
    // silently ignore if already closed
    if (!_opened) {
        return;
    }

    // write files
    _intervalFileSink->close();
    this->writeStringDb(_pathsDb, _pathStrDbPath);
    this->writeStringDb(_strValuesDb, _valueStrDbPath);

    // clear string databases
    _pathsDb.clear();
    _strValuesDb.clear();

    // set as closed
    _opened = false;
}

quark_t StateHistorySink::getQuark(StringDb& stringDb, const std::string& value)
{
    // find in map
    auto it = stringDb.find(value);

    if (it == stringDb.end()) {
        // not found: insert it and return new quark
        stringDb[value] = _curPathQuark;
        _curPathQuark++;

        return _curPathQuark - 1;
    } else {
        return it->second;
    }
}

quark_t StateHistorySink::getPathQuark(const std::string& path)
{
    return this->getQuark(_pathsDb, path);
}

quark_t StateHistorySink::getStringValueQuark(const std::string& value)
{
    return this->getQuark(_strValuesDb, value);
}

void StateHistorySink::writeInterval(quark_t pathQuark)
{
    // retrieve state value entry for this quark
    auto it = _stateValues.find(pathQuark);

    if (it == _stateValues.end()) {
        // not found: quit now
        return;
    }

    const auto& stateValueEntry = it->second;

    // translate from state value to interval
    auto stateValueType = static_cast<std::size_t>(stateValueEntry.value->getType());
    auto interval = _translators[stateValueType](pathQuark,
                                                 stateValueEntry);

    // ignore if unknown state value
    if (!interval) {
        return;
    }

    // add to interval history
    _intervalFileSink->addInterval(delo::AbstractInterval::UP {interval});
}

void StateHistorySink::setState(quark_t pathQuark, AbstractStateValue::UP value)
{
}

void StateHistorySink::removeState(quark_t pathQuark)
{
}

}
}
