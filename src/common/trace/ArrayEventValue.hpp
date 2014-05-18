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
#ifndef _ARRAYEVENTVALUE_HPP
#define _ARRAYEVENTVALUE_HPP

#include <cstdint>
#include <vector>
#include <babeltrace/babeltrace.h>
#include <babeltrace/ctf/events.h>

#include <common/trace/AbstractEventValue.hpp>
#include <common/trace/EventValueType.hpp>

namespace tibee
{
namespace common
{

/**
 * Event value carrying an array of values.
 *
 * @author Philippe Proulx
 */
class ArrayEventValue :
    public AbstractEventValue
{
public:
    /**
     * Builds an array value out of a field definition and an event.
     *
     * @param def BT field definition
     * @param ev  BT event
     */
    ArrayEventValue(const ::bt_definition* def, const ::bt_ctf_event* ev);

    /**
     * Returns the number of items in this array.
     *
     * @returns Item count
     */
    std::size_t size() const;

    /**
     * Returns the event value at index \p index without checking
     * boundaries.
     *
     * @param index Index of item to get
     * @returns     Event value
     */
    AbstractEventValue::UP operator[](std::size_t index) const;

    /**
     * Convenience method which builds a vector of event values
     * contained in this array.
     *
     * The vector is not cached internally, but built everytime this is
     * called.
     *
     * @returns Vector of event values
     */
    std::vector<AbstractEventValue::UP> getVector() const;

    /**
     * Returns whether this array is in fact a string of representable
     * characters.
     *
     * @returns True if this array is a representable string
     */
    bool isString() const;

    /**
     * Returns the string equivalent of this array, or \a nullptr if
     * not possible.
     *
     * @returns String equivalent or \a nullptr if not available
     */
    const char* getString() const;

    /**
     * @see AbstractEventValue::toString()
     */
    std::string toString() const;

private:
    void buildCache();

private:
    const ::bt_definition* _btDef;
    const ::bt_declaration* _btDecl;
    const ::bt_ctf_event* _btEvent;
    ::ctf_type_id _btType;
    std::size_t _size;
    bool _done;
};

}
}

#endif // _ARRAYEVENTVALUE_HPP