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
#include <fstream>
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
    _currentState {*this},
    _stateChangesCount {0}
{
    _intervalFileSink = std::unique_ptr<delo::HistoryFileSink> {
        new delo::HistoryFileSink
    };

    this->initTranslators();
    this->open();
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

    _opened = true;
}

void StateHistorySink::close()
{
    // silently ignore if already closed
    if (!_opened) {
        return;
    }

    // write all remaining state values as intervals
    for (const auto& quarkStateEntryPair : _stateValues) {
        this->writeInterval(quarkStateEntryPair.first);
    }

    // clear all state values now
    _stateValues.clear();

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

quark_t StateHistorySink::getQuark(StringDb& stringDb, const std::string& value,
                                   quark_t& curQuark)
{
    // find in map
    auto it = stringDb.find(value);

    if (it == stringDb.end()) {
        // not found: insert it and return new quark
        stringDb[value] = curQuark;
        curQuark++;

        return curQuark - 1;
    } else {
        return it->second;
    }
}

quark_t StateHistorySink::getPathQuark(const std::string& path)
{
    return this->getQuark(_pathsDb, path, _curPathQuark);
}

quark_t StateHistorySink::getStringValueQuark(const std::string& value)
{
    return this->getQuark(_strValuesDb, value, _curStrValueQuark);
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

    // do not bother writing a zero-length/weird interval
    if (stateValueEntry.beginTs >= _ts) {
        return;
    }

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

    // update internal statistics
    _stateChangesCount++;
}

void StateHistorySink::setState(quark_t pathQuark, AbstractStateValue::UP value)
{
    // write interval and set new state value
    this->writeInterval(pathQuark);

    _stateValues[pathQuark] = {
        .beginTs = _ts,
        .value = std::move(value)
    };
}

void StateHistorySink::removeState(quark_t pathQuark)
{
    // write interval and then remove entry in current state values
    this->writeInterval(pathQuark);
    _stateValues.erase(pathQuark);
}

void StateHistorySink::writeStringDb(const StringDb& stringDb,
                                     const boost::filesystem::path& path)
{
    // open output file for writing
    bfs::ofstream output;

    output.open(path, std::ios::binary);

    // write all string/quark pairs
    for (const auto& stringQuarkPair : stringDb) {
        const auto& string = stringQuarkPair.first;
        auto quark = stringQuarkPair.second;

        // write string part
        output.write(string.c_str(), string.size() + 1);

        // align for quark
        output.seekp((output.tellp() + static_cast<long>(sizeof(quark) - 1)) & ~(sizeof(quark) - 1));

        // write quark
        output.write(reinterpret_cast<char*>(&quark), sizeof(quark));
    }

    // close output file
    output.close();
}

}
}
