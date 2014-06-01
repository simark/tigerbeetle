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

#include <common/state/StateValueType.hpp>
#include <common/state/StateHistorySink.hpp>
#include <common/state/CurrentState.hpp>
#include <common/state/Int32StateValue.hpp>
#include <common/state/Uint32StateValue.hpp>
#include <common/state/Int64StateValue.hpp>
#include <common/state/Uint64StateValue.hpp>
#include <common/state/Float32StateValue.hpp>
#include <common/state/QuarkStateValue.hpp>

namespace tibee
{
namespace common
{

CurrentState::CurrentState(StateHistorySink* sink) :
    _sink {sink}
{
}

quark_t CurrentState::getPathQuark(const char* path) const
{
    // delegate to state history sink
    return _sink->getPathQuark(path);
}

quark_t CurrentState::getPathQuark(const std::string& path) const
{
    // delegate to state history sink
    return _sink->getPathQuark(path);
}

quark_t CurrentState::getStringValueQuark(const char* path) const
{
    // delegate to state history sink
    return _sink->getStringValueQuark(path);
}

quark_t CurrentState::getStringValueQuark(const std::string& path) const
{
    // delegate to state history sink
    return _sink->getStringValueQuark(path);
}

void CurrentState::setInt32State(quark_t pathQuark, std::int32_t value)
{
    AbstractStateValue::UP stateValue {new Int32StateValue {value}};

    _sink->setState(pathQuark, std::move(stateValue));
}

void CurrentState::setUint32State(quark_t pathQuark, std::uint32_t value)
{
    AbstractStateValue::UP stateValue {new Uint32StateValue {value}};

    _sink->setState(pathQuark, std::move(stateValue));
}

void CurrentState::setInt64State(quark_t pathQuark, std::int64_t value)
{
    AbstractStateValue::UP stateValue {new Int64StateValue {value}};

    _sink->setState(pathQuark, std::move(stateValue));
}

void CurrentState::setUint64State(quark_t pathQuark, std::uint64_t value)
{
    AbstractStateValue::UP stateValue {new Uint64StateValue {value}};

    _sink->setState(pathQuark, std::move(stateValue));
}

void CurrentState::setFloat32State(quark_t pathQuark, float value)
{
    AbstractStateValue::UP stateValue {new Float32StateValue {value}};

    _sink->setState(pathQuark, std::move(stateValue));
}

void CurrentState::setQuarkState(quark_t pathQuark, quark_t value)
{
    AbstractStateValue::UP stateValue {new QuarkStateValue {value}};

    _sink->setState(pathQuark, std::move(stateValue));
}

void CurrentState::setState(quark_t pathQuark, AbstractStateValue::UP value)
{
    _sink->setState(pathQuark, std::move(value));
}

bool CurrentState::incState(quark_t pathQuark, std::int64_t value)
{
    auto stateValue = _sink->getState(pathQuark);

    if (!stateValue) {
        return false;
    }

    auto stateValueType = stateValue->getType();

    if (stateValueType == StateValueType::INT32) {
        auto curStateValue = static_cast<const Int32StateValue*>(stateValue);

        this->setInt32State(pathQuark, curStateValue->getValue() + value);
    } else if (stateValueType == StateValueType::UINT32) {
        auto curStateValue = static_cast<const Uint32StateValue*>(stateValue);

        this->setUint32State(pathQuark, curStateValue->getValue() + value);
    } else if (stateValueType == StateValueType::INT64) {
        auto curStateValue = static_cast<const Int64StateValue*>(stateValue);

        this->setInt64State(pathQuark, curStateValue->getValue() + value);
    } else if (stateValueType == StateValueType::UINT64) {
        auto curStateValue = static_cast<const Uint64StateValue*>(stateValue);

        this->setUint64State(pathQuark, curStateValue->getValue() + value);
    } else {
        // not an integer state value
        return false;
    }

    return true;
}

bool CurrentState::decState(quark_t pathQuark, std::int64_t value)
{
    return this->incState(pathQuark, -value);
}

void CurrentState::removeState(quark_t pathQuark)
{
    _sink->removeState(pathQuark);
}

const AbstractStateValue* CurrentState::getState(quark_t pathQuark) const
{
    return _sink->getState(pathQuark);
}

}
}
