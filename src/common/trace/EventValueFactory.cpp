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
#include <memory>
#include <functional>
#include <map>
#include <babeltrace/ctf/events.h>
#include <iostream>

#include <common/trace/EventValuePool.hpp>
#include <common/trace/EventValueType.hpp>
#include <common/trace/AbstractEventValue.hpp>
#include <common/trace/StringEventValue.hpp>
#include <common/trace/SintEventValue.hpp>
#include <common/trace/UintEventValue.hpp>
#include <common/trace/FloatEventValue.hpp>
#include <common/trace/UnknownEventValue.hpp>
#include <common/trace/EnumEventValue.hpp>
#include <common/trace/ArrayEventValue.hpp>
#include <common/trace/DictEventValue.hpp>
#include <common/trace/EventValueFactory.hpp>

namespace tibee
{
namespace common
{

EventValueFactory::EventValueFactory() :
    _arrayPool {128},
    _dictPool {32},
    _enumPool {64},
    _floatPool {64},
    _sintPool {128},
    _stringPool {64},
    _uintPool {128},
    _unknownPool {1}
{
    this->initTypes();
}

EventValueFactory::~EventValueFactory()
{
    // everything should be handled by std and Boost here
}

void EventValueFactory::initTypes()
{
    // precious builder functions
    auto unknownBuilder = [this](const ::bt_definition* def, const ::bt_ctf_event* ev)
    {
        return new(_unknownPool.get()) UnknownEventValue;
    };
    auto intBuilder = [this](const ::bt_definition* def, const ::bt_ctf_event* ev) -> const AbstractEventValue*
    {
        auto decl = ::bt_ctf_get_decl_from_def(def);

        if (::bt_ctf_get_int_signedness(decl) == 1) {
            return new(_sintPool.get()) SintEventValue {def};
        } else {
            return new(_uintPool.get()) UintEventValue {def};
        }
    };
    auto floatBuilder = [this](const ::bt_definition* def, const ::bt_ctf_event* ev)
    {
        return new(_floatPool.get()) FloatEventValue {def};
    };
    auto enumBuilder = [this](const ::bt_definition* def, const ::bt_ctf_event* ev)
    {
        return new(_enumPool.get()) EnumEventValue {def};
    };
    auto stringBuilder = [this](const ::bt_definition* def, const ::bt_ctf_event* ev)
    {
        return new(_stringPool.get()) StringEventValue {def};
    };
    auto structBuilder = [this](const ::bt_definition* def, const ::bt_ctf_event* ev)
    {
        return new(_dictPool.get()) DictEventValue {def, ev, this};
    };
    auto variantBuilder = [this](const ::bt_definition* def, const ::bt_ctf_event* ev)
    {
        auto realDef = ::bt_ctf_get_variant(def);

        return this->buildEventValue(realDef, ev);
    };
    auto arraySequenceBuilder = [this](const ::bt_definition* def, const ::bt_ctf_event* ev)
    {
        return new(_arrayPool.get()) ArrayEventValue {def, ev, this};
    };

    /* Since the enumeration values of CTF types could change in time,
     * we go through all values from 0 to n (n >= last CTF type value)
     * and set the right builder for the right CTF type using a switch.
     * This is slow but done only once.
     */
    for (std::size_t x = 0; x < _builders.size(); ++x) {
        switch (x) {
        case ::CTF_TYPE_INTEGER:
            _builders[x] = intBuilder;
            break;

        case ::CTF_TYPE_FLOAT:
            _builders[x] = floatBuilder;
            break;

        case ::CTF_TYPE_ENUM:
            _builders[x] = enumBuilder;
            break;

        case ::CTF_TYPE_STRING:
            _builders[x] = stringBuilder;
            break;

        case ::CTF_TYPE_STRUCT:
            _builders[x] = structBuilder;
            break;

        case ::CTF_TYPE_VARIANT:
            _builders[x] = variantBuilder;
            break;

        case ::CTF_TYPE_ARRAY:
        case ::CTF_TYPE_SEQUENCE:
            _builders[x] = arraySequenceBuilder;
            break;

        default:
            _builders[x] = unknownBuilder;
        }
    }
}

const AbstractEventValue* EventValueFactory::buildEventValue(const ::bt_definition* def,
                                                             const ::bt_ctf_event* ev) const
{
    // get event value type
    auto decl = ::bt_ctf_get_decl_from_def(def);
    auto valueType = ::bt_ctf_field_type(decl);

    // call builder
    return _builders[valueType](def, ev);
}

void EventValueFactory::resetPools()
{
    _arrayPool.reset();
    _dictPool.reset();
    _enumPool.reset();
    _floatPool.reset();
    _sintPool.reset();
    _stringPool.reset();
    _uintPool.reset();
    _unknownPool.reset();
}

}
}
