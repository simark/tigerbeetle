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
#include <map>
#include <babeltrace/ctf/events.h>

#include <common/trace/EventValueType.hpp>
#include <common/trace/EventValueUtils.hpp>
#include <common/trace/AbstractEventValue.hpp>
#include <common/trace/DictEventValue.hpp>

namespace tibee
{
namespace common
{

DictEventValue::DictEventValue(const ::bt_definition* def,
                                 const ::bt_ctf_event* ev) :
    AbstractEventValue {EventValueType::DICT},
    _btDef {def},
    _btEvent {ev},
    _btFieldList {nullptr},
    _size {0},
    _done {false}
{
    this->buildCache();
}

void DictEventValue::buildCache()
{
    if (!_done) {
        _btDecl = ::bt_ctf_get_decl_from_def(_btDef);

        unsigned int count;

        auto ret = ::bt_ctf_get_field_list(_btEvent, _btDef, &_btFieldList, &count);

        if (ret >= 0) {
            _size = count;
        } else {
            _btFieldList = nullptr;
        }

        _done = true;
    }
}

std::size_t DictEventValue::size() const
{
    return _size;
}

const char* DictEventValue::getKeyName(std::size_t index) const
{
    if (!_btFieldList) {
        return nullptr;
    }

    auto def = _btFieldList[index];

    return ::bt_ctf_field_name(def);
}

std::string DictEventValue::getKeyNameStr(std::size_t index) const
{
    auto name = this->getKeyName(index);

    if (!name) {
        name = "";
    }

    return std::string {name};
}

AbstractEventValue::UP DictEventValue::operator[](std::size_t index) const
{
    if (!_btFieldList) {
        return nullptr;
    }

    auto itemDef = _btFieldList[index];

    if (!itemDef) {
        return nullptr;
    }

    return EventValueUtils::getEventValue(itemDef, _btEvent);
}

std::map<std::string, AbstractEventValue::UP> DictEventValue::getMap() const
{
    std::map<std::string, AbstractEventValue::UP> ret;

    for (std::size_t x = 0; x < this->size(); ++x) {
        auto keyName = this->getKeyName(x);
        auto eventValue = (*this)[x];

        ret.insert(std::make_pair(keyName, std::move(eventValue)));
    }

    return ret;
}

std::string DictEventValue::toString() const
{
    std::stringstream ss;

    if (this->size() == 0) {
        return "{}";
    }

    auto map = this->getMap();

    ss << "{";

    auto beforeEnd = map.end();
    beforeEnd--;

    for (auto it = map.begin(); it != beforeEnd; ++it) {
        const auto& keyName = it->first;
        const auto& value = it->second;

        ss << "\"";
        ss << keyName;
        ss << "\": ";
        ss << value->toString();
        ss << ", ";
    }

    ss << "\"";
    ss << beforeEnd->first;
    ss << "\": ";
    ss << beforeEnd->second->toString();
    ss << "}";

    return ss.str();
}

}
}
