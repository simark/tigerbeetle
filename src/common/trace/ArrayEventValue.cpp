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
#include <sstream>
#include <vector>
#include <babeltrace/ctf/events.h>

#include <common/trace/EventValueType.hpp>
#include <common/trace/EventValueUtils.hpp>
#include <common/trace/AbstractEventValue.hpp>
#include <common/trace/ArrayEventValue.hpp>

namespace tibee
{
namespace common
{

ArrayEventValue::ArrayEventValue(const ::bt_definition* def,
                                 const ::bt_ctf_event* ev) :
    AbstractEventValue {EventValueType::ARRAY},
    _btDef {def},
    _btEvent {ev},
    _size {0},
    _done {false}
{
    this->buildCache();
}

void ArrayEventValue::buildCache()
{
    if (!_done) {
        _btDecl = ::bt_ctf_get_decl_from_def(_btDef);
        _btType = ::bt_ctf_field_type(_btDecl);

        if (_btType == CTF_TYPE_SEQUENCE) {
            // FIXME: find the proper way to get a CTF sequence length
        } else {
            _size = ::bt_ctf_get_array_len(_btDecl);
        }

        _done = true;
    }
}

std::size_t ArrayEventValue::size() const
{
    return _size;
}

AbstractEventValue::UP ArrayEventValue::operator[](std::size_t index) const
{
    if (_btType == CTF_TYPE_SEQUENCE) {
        // FIXME: find the proper way to retrieve a CTF sequence item
        return nullptr;
    }

    auto itemDef = ::bt_ctf_get_index(_btEvent, _btDef, index);

    if (!itemDef) {
        return nullptr;
    }

    return EventValueUtils::getEventValue(itemDef, _btEvent);
}

std::vector<AbstractEventValue::UP> ArrayEventValue::getVector() const
{
    std::vector<AbstractEventValue::UP> ret;

    for (std::size_t x = 0; x < this->size(); ++x) {
        ret.push_back((*this)[x]);
    }

    return ret;
}

bool ArrayEventValue::isString() const
{
    auto encoding = ::bt_ctf_get_encoding(_btDecl);

    return encoding == ::CTF_STRING_UTF8 || encoding == ::CTF_STRING_ASCII;
}

const char* ArrayEventValue::getString() const
{
    if (_btType == CTF_TYPE_SEQUENCE) {
        // FIXME: find the proper way to retrieve a CTF sequence string
        return nullptr;
    }

    return ::bt_ctf_get_char_array(_btDef);
}

std::string ArrayEventValue::toString() const
{
    std::stringstream ss;

    if (this->size() == 0) {
        return "[]";
    }

    if (this->isString()) {
        auto str = this->getString();

        if (str) {
            ss << "\"";
            ss << str;
            ss << "\"";

            return ss.str();
        }
    }

    auto vec = this->getVector();

    ss << "[";

    for (auto it = vec.begin(); it != vec.end() - 1; ++it) {
        ss << (*it)->toString();
        ss << ", ";
    }

    ss << vec.back()->toString();
    ss << "]";

    return ss.str();
}

}
}
