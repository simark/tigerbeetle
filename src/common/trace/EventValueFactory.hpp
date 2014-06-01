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
#ifndef _TIBEE_COMMON_EVENTVALUEFACTORY_HPP
#define _TIBEE_COMMON_EVENTVALUEFACTORY_HPP

#include <array>
#include <functional>
#include <babeltrace/ctf/events.h>

#include <common/trace/EventValuePool.hpp>
#include <common/trace/AbstractEventValue.hpp>
#include <common/trace/EventValueType.hpp>
#include <common/trace/StringEventValue.hpp>
#include <common/trace/SintEventValue.hpp>
#include <common/trace/UintEventValue.hpp>
#include <common/trace/FloatEventValue.hpp>
#include <common/trace/UnknownEventValue.hpp>
#include <common/trace/EnumEventValue.hpp>
#include <common/trace/ArrayEventValue.hpp>
#include <common/trace/DictEventValue.hpp>

namespace tibee
{
namespace common
{

/**
 * Event value factory.
 *
 * This is the appropriate object to use for creating event values in an
 * efficient and portable way.
 *
 * @author Philippe Proulx
 */
class EventValueFactory
{
public:
    /**
     * Builds an event value factory.
     */
    EventValueFactory();

    /**
     * Destroys an event value factory.
     *
     * Any event value built using this factory will not be valid when
     * it's destroyed.
     */
    ~EventValueFactory();

    /**
     * Returns an abstract event value out of a BT definition/event pair,
     * potentially building it.
     *
     * Caller doesn't own this pointer and should not free it.
     *
     * @param def BT field definition
     * @param ev  BT event
     * @returns   Abstract event value for this event/definition pair
     */
    const AbstractEventValue* buildEventValue(const ::bt_definition* def,
                                              const ::bt_ctf_event* ev) const;

    /**
     * Resets all internal pools.
     */
    void resetPools();

private:
    typedef std::function<const AbstractEventValue* (const ::bt_definition*, const ::bt_ctf_event* ev)> BuildValueFunc;

private:
    void initTypes();

private:
    // array mapping (CTF types -> event value builder functions)
    std::array<BuildValueFunc, 32> _builders;

    // our object pools
    EventValuePool<ArrayEventValue> _arrayPool;
    EventValuePool<DictEventValue> _dictPool;
    EventValuePool<EnumEventValue> _enumPool;
    EventValuePool<FloatEventValue> _floatPool;
    EventValuePool<SintEventValue> _sintPool;
    EventValuePool<StringEventValue> _stringPool;
    EventValuePool<UintEventValue> _uintPool;
    EventValuePool<UnknownEventValue> _unknownPool;
};

}
}

#endif // _TIBEE_COMMON_EVENTVALUEFACTORY_HPP
