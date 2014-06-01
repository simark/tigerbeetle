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
#ifndef _TIBEE_COMMON_DICTEVENTVALUE_HPP
#define _TIBEE_COMMON_DICTEVENTVALUE_HPP

#include <cstddef>
#include <memory>
#include <map>
#include <babeltrace/ctf/events.h>

#include <common/trace/AbstractEventValue.hpp>
#include <common/trace/EventValueType.hpp>

namespace tibee
{
namespace common
{

class EventValueFactory;

/**
 * Event value carrying an dictionary of values.
 *
 * @author Philippe Proulx
 */
class DictEventValue :
    public AbstractEventValue
{
public:
    /// Unique pointer to dictionary event value
    typedef std::unique_ptr<DictEventValue> UP;

public:
    /**
     * Builds a dictionary value out of a field definition.
     *
     * @param def          BT field definition
     * @param ev           BT event
     * @param valueFactory Factory to be used for building other values
     */
    DictEventValue(const ::bt_definition* def, const ::bt_ctf_event* ev,
                   const EventValueFactory* valueFactory);

    /**
     * Returns the number of items in this dictionary.
     *
     * This must be checked before using any method using an index
     * because DictEventValue doesn't check bounds.
     *
     * @returns Item count
     */
    std::size_t size() const;

    /**
     * Returns the key name at index \p index without checking
     * bounds.
     *
     * @param index Index of key of which to get the name
     * @returns     Name of key at index \p index
     */
    const char* getKeyName(std::size_t index) const;

    /**
     * Returns a copy of the key name at index \p index without
     * checking bounds.
     *
     * @param index Index of key of which to get a copy of the name
     * @returns     Copy of name of key at index \p index
     */
    std::string getKeyNameStr(std::size_t index) const;

    /**
     * Returns the event value at index \p index.
     *
     * Dictionary values are indexed using an integer. Use
     * getKeyName(std::size_t) to get the name of the key at a
     * specific index. This method doesn't check bounds (call
     * size() first).
     *
     * @returns Event value at index \p index
     */
    const AbstractEventValue* operator[](std::size_t index) const;

    /**
     * Convenience method which builds a (key name -> event value) map
     * using data contained in this dictionary.
     *
     * The map is not cached internally, but built everytime this is
     * called (slow: use for debugging).
     *
     * @returns Map of (key name -> event value)
     */
    std::map<std::string, const AbstractEventValue*> getMap() const;

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
    const EventValueFactory* _valueFactory;
    ::bt_definition const* const* _btFieldList;
    std::size_t _size;
};

}
}

#endif // _TIBEE_COMMON_DICTEVENTVALUE_HPP
