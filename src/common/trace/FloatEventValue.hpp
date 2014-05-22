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
#ifndef _FLOATEVENTVALUE_HPP
#define _FLOATEVENTVALUE_HPP

#include <babeltrace/ctf/events.h>

#include <common/trace/AbstractEventValue.hpp>

namespace tibee
{
namespace common
{

/**
 * Event value carrying a floating point number.
 *
 * @author Philippe Proulx
 */
class FloatEventValue :
    public AbstractEventValue
{
public:
    /**
     * Builds a floating point number value out of a field definition.
     *
     * @param def BT field definition
     */
    FloatEventValue(const ::bt_definition* def);

    /**
     * Returns the floating point number value.
     *
     * @returns Floating point number value
     */
    double getValue() const;

    /**
     * @see AbstractEventValue::toString()
     */
    std::string toString() const;

private:
    const ::bt_definition* _btDef;
};

}
}

#endif // _FLOATEVENTVALUE_HPP
