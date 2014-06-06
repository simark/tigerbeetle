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
#include <cassert>

#include <common/trace/AbstractEventValue.hpp>
#include <common/trace/SintEventValue.hpp>
#include <common/trace/UintEventValue.hpp>
#include <common/trace/FloatEventValue.hpp>
#include <common/trace/EnumEventValue.hpp>
#include <common/trace/StringEventValue.hpp>
#include <common/trace/ArrayEventValue.hpp>
#include <common/trace/DictEventValue.hpp>

namespace tibee
{
namespace common
{

AbstractEventValue::~AbstractEventValue()
{
}

const SintEventValue* AbstractEventValue::asSint() const
{
    assert(this->isSint());

    return static_cast<const SintEventValue*>(this);
}

const UintEventValue* AbstractEventValue::asUint() const
{
    assert(this->isUint());

    return static_cast<const UintEventValue*>(this);
}

const FloatEventValue* AbstractEventValue::asFloat() const
{
    assert(this->isFloat());

    return static_cast<const FloatEventValue*>(this);
}

const EnumEventValue* AbstractEventValue::asEnum() const
{
    assert(this->isEnum());

    return static_cast<const EnumEventValue*>(this);
}

const StringEventValue* AbstractEventValue::asString() const
{
    assert(this->isString());

    return static_cast<const StringEventValue*>(this);
}

const ArrayEventValue* AbstractEventValue::asArray() const
{
    assert(this->isArray());

    return static_cast<const ArrayEventValue*>(this);
}

const DictEventValue* AbstractEventValue::asDict() const
{
    assert(this->isDict());

    return static_cast<const DictEventValue*>(this);
}

}
}
