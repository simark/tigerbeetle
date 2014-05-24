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
#ifndef _ABSTRACTSTATEVALUE_HPP
#define _ABSTRACTSTATEVALUE_HPP

#include <memory>

#include <common/state/StateValueType.hpp>

namespace tibee
{
namespace common
{

/**
 * Abstract state value. All concrete state values must inherit
 * this class.
 *
 * @author Philippe Proulx
 */
class AbstractStateValue
{
public:
    /// Shared pointer to abstract state value
    typedef std::shared_ptr<AbstractStateValue> SP;

    /// Unique pointer to abstract state value
    typedef std::unique_ptr<AbstractStateValue> UP;

public:
    /**
     * Builds an abstract state value of a given type.
     *
     * @param type Type of state value
     */
    AbstractStateValue(StateValueType type);

    virtual ~AbstractStateValue() = 0;

    /**
     * Returns this state value's type.
     *
     * @returns State value type
     */
    StateValueType getType() const
    {
        return _type;
    }

private:
    StateValueType _type;
};

}
}

#endif // _ABSTRACTSTATEVALUE_HPP
