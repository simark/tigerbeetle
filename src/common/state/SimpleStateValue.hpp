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
#ifndef _TIBEE_COMMON_SIMPLESTATEVALUE_HPP
#define _TIBEE_COMMON_SIMPLESTATEVALUE_HPP

#include <common/state/AbstractStateValue.hpp>
#include <common/state/StateValueType.hpp>

namespace tibee
{
namespace common
{

/**
 * Simple state value template class. This is used to create state value
 * classes carrying simple types.
 *
 * @author Philippe Proulx
 */
template<typename T, StateValueType SVT>
class SimpleStateValue :
    public AbstractStateValue
{
public:
    /**
     * Builds a simple state value.
     *
     * @param value State value
     */
    SimpleStateValue(T value);

    virtual ~SimpleStateValue();

    /**
     * Returns this state value
     *
     * @returns State value
     */
    T getValue() const
    {
        return _value;
    }

private:
    T _value;
};

template<typename T, StateValueType SVT>
SimpleStateValue<T, SVT>::SimpleStateValue(T value) :
    AbstractStateValue {SVT},
    _value {value}
{
}

template<typename T, StateValueType SVT>
SimpleStateValue<T, SVT>::~SimpleStateValue()
{
}

}
}

#endif // _TIBEE_COMMON_SIMPLESTATEVALUE_HPP
