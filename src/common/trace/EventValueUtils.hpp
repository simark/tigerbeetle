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
#ifndef _EVENTVALUEUTILS_HPP
#define _EVENTVALUEUTILS_HPP

#include <memory>
#include <babeltrace/ctf/events.h>

#include <common/trace/AbstractEventValue.hpp>
#include <common/trace/EventValueType.hpp>

namespace tibee
{
namespace common
{

/**
 * Event value utilities.
 *
 * @author Philippe Proulx
 */
class EventValueUtils
{
public:
    /**
     * Returns an abstract event value out of a BT definition/event pair.
     *
     * @param def BT field definition
     * @param ev  BT event
     * @returns   Abstract event value for this event/definition pair
     */
    static AbstractEventValue::UP getEventValue(const ::bt_definition* def,
                                                const ::bt_ctf_event* ev);
};

}
}

#endif // _EVENTVALUEUTILS_HPP
