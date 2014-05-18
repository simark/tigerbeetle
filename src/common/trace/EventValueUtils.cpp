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
#include <common/trace/EventValueUtils.hpp>

namespace tibee
{
namespace common
{

AbstractEventValue::UP EventValueUtils::getEventValue(const ::bt_definition* def,
                                                      const ::bt_ctf_event* ev)
{
    // event value builder function
    typedef std::function<AbstractEventValue::UP (const ::bt_definition*, const ::bt_ctf_event* ev)> BuildValueFunc;

    // unknown event value builder
    static BuildValueFunc unknownBuilder = [](const ::bt_definition* def, const ::bt_ctf_event* ev)
    {
        return AbstractEventValue::UP {new UnknownEventValue};
    };

    // (CTF ID -> event value builder function) map
    static std::map<::ctf_type_id, BuildValueFunc> builders = {
        {
            ::CTF_TYPE_UNKNOWN,
            unknownBuilder
        },
        {
            ::CTF_TYPE_INTEGER,
            [](const ::bt_definition* def, const ::bt_ctf_event* ev)
            {
                auto decl = ::bt_ctf_get_decl_from_def(def);

                if (::bt_ctf_get_int_signedness(decl) == 1) {
                    return AbstractEventValue::UP {new SintEventValue {def}};
                } else {
                    return AbstractEventValue::UP {new UintEventValue {def}};
                }
            }
        },
        {
            ::CTF_TYPE_FLOAT,
            [](const ::bt_definition* def, const ::bt_ctf_event* ev)
            {
                return AbstractEventValue::UP {new FloatEventValue {def}};
            }
        },
        {
            ::CTF_TYPE_ENUM,
            [](const ::bt_definition* def, const ::bt_ctf_event* ev)
            {
                return AbstractEventValue::UP {new EnumEventValue {def}};
            }
        },
        {
            ::CTF_TYPE_STRING,
            [](const ::bt_definition* def, const ::bt_ctf_event* ev)
            {
                return AbstractEventValue::UP {new StringEventValue {def}};
            }
        },
        {
            ::CTF_TYPE_STRUCT,
            [](const ::bt_definition* def, const ::bt_ctf_event* ev)
            {
                return AbstractEventValue::UP {new DictEventValue {def, ev}};
            }
        },
        {
            ::CTF_TYPE_UNTAGGED_VARIANT,
            unknownBuilder
        },
        {
            ::CTF_TYPE_VARIANT,
            [](const ::bt_definition* def, const ::bt_ctf_event* ev)
            {
                auto realDef = ::bt_ctf_get_variant(def);

                return EventValueUtils::getEventValue(realDef, ev);
            }
        },
        {
            ::CTF_TYPE_ARRAY,
            [](const ::bt_definition* def, const ::bt_ctf_event* ev)
            {
                return AbstractEventValue::UP {new ArrayEventValue {def, ev}};
            }
        },
        {
            ::CTF_TYPE_SEQUENCE,
            [](const ::bt_definition* def, const ::bt_ctf_event* ev)
            {
                return AbstractEventValue::UP {new ArrayEventValue {def, ev}};
            }
        }
    };

    // find event value builder in above map
    auto decl = ::bt_ctf_get_decl_from_def(def);
    auto eventType = ::bt_ctf_field_type(decl);
    auto builderIt = builders.find(eventType);

    // make sure builder exists
    if (builderIt == builders.end()) {
        return unknownBuilder(def, ev);
    }

    return builderIt->second(def, ev);
}

}
}
