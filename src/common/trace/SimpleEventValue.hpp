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
#ifndef _SIMPLEEVENTVALUE_HPP
#define _SIMPLEEVENTVALUE_HPP

#include <cstdint>
#include <babeltrace/ctf/events.h>

#include <common/trace/AbstractEventValue.hpp>
#include <common/trace/EventValueType.hpp>

namespace tibee
{
namespace common
{

/**
 * Simple event value template, useful for simple types.
 *
 * @author Philippe Proulx
 */
template<typename T, EventValueType VT>
class SimpleEventValue :
    public AbstractEventValue
{
public:
    /**
     * Builds a simple value out of a field definition.
     *
     * @param def BT field definition
     */

    SimpleEventValue(const ::bt_definition* def);
    virtual ~SimpleEventValue();

    /**
     * Returns the signed integer value.
     *
     * @returns Signed integer value
     */
    std::int64_t getValue() const;

private:
    const ::bt_definition* _btField;
};

}
}

#endif // _SIMPLEEVENTVALUE_HPP
