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
#ifndef _QUARKSTATEVALUE_HPP
#define _QUARKSTATEVALUE_HPP

#include <memory>
#include <cstdint>

#include <common/state/SimpleStateValue.hpp>
#include <common/state/StateValueType.hpp>

namespace tibee
{
namespace common
{

/**
 * Quark state value (32-bit unsigned integer referencing
 * data elsewhere).
 *
 * @author Philippe Proulx
 */
class QuarkStateValue :
    public SimpleStateValue<std::uint32_t, StateValueType::QUARK>
{
public:
    typedef std::shared_ptr<QuarkStateValue> SP;
    typedef std::unique_ptr<QuarkStateValue> UP;

public:
    using SimpleStateValue::SimpleStateValue;
};

}
}

#endif // _QUARKSTATEVALUE_HPP
