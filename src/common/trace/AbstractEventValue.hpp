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
#ifndef _TIBEE_COMMON_ABSTRACTEVENTVALUE_HPP
#define _TIBEE_COMMON_ABSTRACTEVENTVALUE_HPP

#include <memory>

#include <common/trace/EventValueType.hpp>

namespace tibee
{
namespace common
{

/**
 * Abstract event value. All event values must inherit this class.
 *
 * @author Philippe Proulx
 */
class AbstractEventValue
{
public:
    /// Unique pointer to abstract event value
    typedef std::unique_ptr<AbstractEventValue> UP;

public:
    /**
     * Builds an event value with type \p type.
     *
     * @param type Event value type
     */
    AbstractEventValue(EventValueType type) :
        _type {type}
    {
    }

    virtual ~AbstractEventValue();

    /**
     * Returns a string representation of this event value.
     *
     * This method may be really slow and is only useful for
     * debugging.
     *
     * @returns String representation of this event value.
     */
    virtual std::string toString() const = 0;

    /**
     * Returns the event value type.
     *
     * @returns Event value type.
     */
    EventValueType getType() const
    {
        return _type;
    }

private:
    EventValueType _type;
};

}
}

#endif // _TIBEE_COMMON_ABSTRACTEVENTVALUE_HPP
