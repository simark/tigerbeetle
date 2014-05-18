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
#include <string>
#include <sstream>
#include <babeltrace/ctf/events.h>

#include <common/trace/EventValueType.hpp>
#include <common/trace/AbstractEventValue.hpp>
#include <common/trace/EnumEventValue.hpp>

namespace tibee
{
namespace common
{

EnumEventValue::EnumEventValue(const ::bt_definition* def) :
    AbstractEventValue {EventValueType::ENUM},
    _btDef {def}
{
}

std::uint64_t EnumEventValue::getValue() const
{
    auto intDef = ::bt_ctf_get_enum_int(_btDef);

    return ::bt_ctf_get_uint64(intDef);
}

const char* EnumEventValue::getLabel() const
{
    return ::bt_ctf_get_enum_str(_btDef);
}

std::string EnumEventValue::getLabelStr() const
{
    return std::string {this->getLabel()};
}

std::string EnumEventValue::toString() const
{
    std::stringstream ss;

    ss << "\"";
    ss << this->getLabel();
    ss << " (";
    ss << this->getValue();
    ss << ")\"";

    return ss.str();
}

}
}
