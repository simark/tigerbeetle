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
#include <cstdint>
#include <babeltrace/ctf/events.h>

#include <common/trace/EventValueType.hpp>
#include <common/trace/AbstractEventValue.hpp>
#include <common/trace/FloatEventValue.hpp>

namespace tibee
{
namespace common
{

FloatEventValue::FloatEventValue(const ::bt_definition* def) :
    AbstractEventValue {EventValueType::FLOAT},
    _btDef {def}
{
}

double FloatEventValue::getValue() const
{
    return ::bt_ctf_get_float(_btDef);
}

std::string FloatEventValue::toStringImpl() const
{
    return std::to_string(this->getValue());
}

}
}
