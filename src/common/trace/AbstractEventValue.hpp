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

class SintEventValue;
class UintEventValue;
class FloatEventValue;
class EnumEventValue;
class StringEventValue;
class ArrayEventValue;
class DictEventValue;

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

    virtual ~AbstractEventValue() = 0;

    /**
     * Returns a string representation of this event value.
     *
     * This method may be really slow and is only useful for
     * debugging.
     *
     * @returns String representation of this event value.
     */
    std::string toString() const;

    /**
     * Returns the event value type.
     *
     * @returns Event value type.
     */
    EventValueType getType() const
    {
        return _type;
    }

    /**
     * Statically casts this event value to a signed integer event
     * value.
     *
     * No runtime check is performed.
     *
     * @returns This event value as a signed integer event value
     */
    const SintEventValue* asSint() const;

    /**
     * Statically casts this event value to an unsigned integer event
     * value.
     *
     * No runtime check is performed.
     *
     * @returns This event value as an unsigned integer event value
     */
    const UintEventValue* asUint() const;

    /**
     * Statically casts this event value to a floating point number
     * event value.
     *
     * No runtime check is performed.
     *
     * @returns This event value as a floating point number event value
     */
    const FloatEventValue* asFloat() const;

    /**
     * Statically casts this event value to an enumeration event
     * value.
     *
     * No runtime check is performed.
     *
     * @returns This event value as an enumeration event value
     */
    const EnumEventValue* asEnum() const;

    /**
     * Statically casts this event value to a string event
     * value.
     *
     * No runtime check is performed.
     *
     * @returns This event value as a string event value
     */
    const StringEventValue* asString() const;

    /**
     * Statically casts this event value to an array event
     * value.
     *
     * No runtime check is performed.
     *
     * @returns This event value as an array event value
     */
    const ArrayEventValue* asArray() const;

    /**
     * Statically casts this event value to a dictionary event
     * value.
     *
     * No runtime check is performed.
     *
     * @returns This event value as a dictionary event value
     */
    const DictEventValue* asDict() const;

    /**
     * Returns whether or not this is a signed integer event value.
     *
     * @returns True if this is a signed integer event value
     */
    bool isSint() const
    {
        return _type == EventValueType::SINT;
    }

    /**
     * Returns whether or not this is an unsigned integer event value.
     *
     * @returns True if this is an unsigned integer event value
     */
    bool isUint() const
    {
        return _type == EventValueType::UINT;
    }

    /**
     * Returns whether or not this is a floating point number event value.
     *
     * @returns True if this is a floating point number integer event value
     */
    bool isFloat() const
    {
        return _type == EventValueType::FLOAT;
    }

    /**
     * Returns whether or not this is an enumeration event value.
     *
     * @returns True if this is an enumeration event value
     */
    bool isEnum() const
    {
        return _type == EventValueType::ENUM;
    }

    /**
     * Returns whether or not this is a string event value.
     *
     * @returns True if this is a string event value
     */
    bool isString() const
    {
        return _type == EventValueType::STRING;
    }

    /**
     * Returns whether or not this is an array event value.
     *
     * @returns True if this is an array event value
     */
    bool isArray() const
    {
        return _type == EventValueType::ARRAY;
    }

    /**
     * Returns whether or not this is a dictionary event value.
     *
     * @returns True if this is a dictionary event value
     */
    bool isDict() const
    {
        return _type == EventValueType::DICT;
    }

    /**
     * Returns whether or not this is an unknown event value.
     *
     * @returns True if this is an unknown event value
     */
    bool isUnknown() const
    {
        return _type == EventValueType::UNKNOWN;
    }

private:
    virtual std::string toStringImpl() const = 0;

private:
    EventValueType _type;
};

}
}

#endif // _TIBEE_COMMON_ABSTRACTEVENTVALUE_HPP
