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
#include <cstddef>
#include <sstream>
#include <vector>
#include <babeltrace/ctf/events.h>

#include <common/trace/EventValueType.hpp>
#include <common/trace/EventValueFactory.hpp>
#include <common/trace/AbstractEventValue.hpp>
#include <common/trace/ArrayEventValue.hpp>

namespace tibee
{
namespace common
{

ArrayEventValue::ArrayEventValue(const ::bt_definition* def, const ::bt_ctf_event* ev,
                                 const EventValueFactory* valueFactory) :
    AbstractEventValue {EventValueType::ARRAY},
    _btDef {def},
    _btEvent {ev},
    _valueFactory {valueFactory},
    _btFieldList {nullptr},
    _size {0}
{
    this->buildCache();
}

void ArrayEventValue::buildCache()
{
    _btDecl = ::bt_ctf_get_decl_from_def(_btDef);

    unsigned int count;

    auto ret = ::bt_ctf_get_field_list(_btEvent, _btDef, &_btFieldList, &count);

    if (ret == 0) {
        _size = count;
    }
}

std::size_t ArrayEventValue::size() const
{
    return _size;
}

const AbstractEventValue* ArrayEventValue::operator[](std::size_t index) const
{
    // this should work for both CTF array and sequence
    auto itemDef = _btFieldList[index];

    return _valueFactory->buildEventValue(itemDef, _btEvent);
}

std::vector<const AbstractEventValue*> ArrayEventValue::getVector() const
{
    std::vector<const AbstractEventValue*> ret;

    for (std::size_t x = 0; x < this->size(); ++x) {
        ret.push_back(this->operator[](x));
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
    if (::bt_ctf_field_type(_btDecl) == CTF_TYPE_SEQUENCE) {
        // FIXME: find the proper way to retrieve a CTF sequence string
        return nullptr;
    }

    return ::bt_ctf_get_char_array(_btDef);
}

std::string ArrayEventValue::toStringImpl() const
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
        if (*it) {
            ss << (*it)->toString();
        } else {
            ss << "null";
        }

        ss << ", ";
    }

    ss << vec.back()->toString();
    ss << "]";

    return ss.str();
}

}
}
