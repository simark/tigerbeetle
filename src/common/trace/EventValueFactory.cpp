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
#include <babeltrace/ctf/events.h>

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
}

void EventValueFactory::initTypes()
{
    // precious builder functions
    static auto unknownBuilder = [this] (const ::bt_definition* def, const ::bt_ctf_event* ev)
    {
        return new(_unknownPool.get()) UnknownEventValue;
    };
    static auto intBuilder = [this] (const ::bt_definition* def, const ::bt_ctf_event* ev) -> const AbstractEventValue*
    {
        auto decl = ::bt_ctf_get_decl_from_def(def);

        if (::bt_ctf_get_int_signedness(decl) == 1) {
            return new(_sintPool.get()) SintEventValue {def};
        } else {
            return new(_uintPool.get()) UintEventValue {def};
        }
    };
    static auto floatBuilder = [this] (const ::bt_definition* def, const ::bt_ctf_event* ev)
    {
        return new(_floatPool.get()) FloatEventValue {def};
    };
    static auto enumBuilder = [this] (const ::bt_definition* def, const ::bt_ctf_event* ev)
    {
        return new(_enumPool.get()) EnumEventValue {def};
    };
    static auto stringBuilder = [this] (const ::bt_definition* def, const ::bt_ctf_event* ev)
    {
        return new(_stringPool.get()) StringEventValue {def};
    };
    static auto structBuilder = [this] (const ::bt_definition* def, const ::bt_ctf_event* ev)
    {
        return new(_dictPool.get()) DictEventValue {def, ev, this};
    };
    static auto variantBuilder = [this] (const ::bt_definition* def, const ::bt_ctf_event* ev)
    {
        auto realDef = ::bt_ctf_get_variant(def);

        return this->buildEventValue(realDef, ev);
    };
    static auto arraySequenceBuilder = [this] (const ::bt_definition* def, const ::bt_ctf_event* ev)
    {
        return new(_arrayPool.get()) ArrayEventValue {def, ev, this};
    };

    // fill our builders
    for (auto& builder : _builders) {
        builder = unknownBuilder;
    }

    _builders[::CTF_TYPE_INTEGER] = intBuilder;
    _builders[::CTF_TYPE_FLOAT] = floatBuilder;
    _builders[::CTF_TYPE_ENUM] = enumBuilder;
    _builders[::CTF_TYPE_STRING] = stringBuilder;
    _builders[::CTF_TYPE_STRUCT] = structBuilder;
    _builders[::CTF_TYPE_VARIANT] = variantBuilder;
    _builders[::CTF_TYPE_ARRAY] = arraySequenceBuilder;
    _builders[::CTF_TYPE_SEQUENCE] = arraySequenceBuilder;
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
