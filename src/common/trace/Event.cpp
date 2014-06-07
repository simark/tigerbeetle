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
#include <cstring>

#include <common/trace/babeltrace-internals.h>
#include <common/trace/DictEventValue.hpp>
#include <common/trace/Event.hpp>

namespace tibee
{
namespace common
{

Event::Event(const EventValueFactory* valueFactory) :
    _valueFactory {valueFactory}
{
}

const char* Event::getName() const
{
    return ::bt_ctf_event_name(_btEvent);
}

std::string Event::getNameStr() const
{
    return std::string {this->getName()};
}

trace_cycles_t Event::getCycles() const
{
    return static_cast<trace_cycles_t>(::bt_ctf_get_cycles(_btEvent));
}

timestamp_t Event::getTimestamp() const
{
    return static_cast<timestamp_t>(::bt_ctf_get_timestamp(_btEvent));
}

const DictEventValue* Event::getTopLevelScope(::bt_ctf_scope topLevelScope)
{
    // get fields scope
    auto scopeDef = ::bt_ctf_get_top_level_scope(_btEvent, topLevelScope);

    // make sure it's a struct
    if (scopeDef && ::bt_ctf_field_type(::bt_ctf_get_decl_from_def(scopeDef)) == ::CTF_TYPE_STRUCT) {
        /* We know for sure a DictEventValue will be returned here because
         * of the check above.
         */
        return _valueFactory->buildEventValue(scopeDef, _btEvent)->asDict();
    }

    return nullptr;
}

const DictEventValue* Event::getFields()
{
    if (!_fieldsDict) {
        _fieldsDict = this->getTopLevelScope(::BT_EVENT_FIELDS);
    }

    return _fieldsDict;
}

const DictEventValue* Event::getContext()
{
    if (!_contextDict) {
        _contextDict = this->getTopLevelScope(::BT_EVENT_CONTEXT);
    }

    return _contextDict;
}

const DictEventValue* Event::getStreamEventContext()
{
    if (!_streamEventContextDict) {
        _streamEventContextDict = this->getTopLevelScope(::BT_STREAM_EVENT_CONTEXT);
    }

    return _streamEventContextDict;
}

const DictEventValue* Event::getStreamPacketContext()
{
    if (!_streamPacketContextDict) {
        _streamPacketContextDict = this->getTopLevelScope(::BT_STREAM_PACKET_CONTEXT);
    }

    return _streamPacketContextDict;
}

const AbstractEventValue* Event::operator[](const char* name)
{
    auto fields = this->getFields();

    if (fields == nullptr) {
        return nullptr;
    }

    for (std::size_t i = 0; i < fields->size(); i++) {
        if (std::strcmp(name, fields->getKeyName(i)) == 0) {
            return (*fields)[i];
        }
    }

    return nullptr;
}

const AbstractEventValue* Event::operator[](const std::string& name)
{
    return this->operator[](name.c_str());
}

const AbstractEventValue* Event::operator[](std::size_t index)
{
    auto fields = this->getFields();

    if (fields == nullptr) {
        return nullptr;
    }

    return (*fields)[index];
}

void Event::setPrivateEvent(::bt_ctf_event* btEvent)
{
    // set the attribute
    _btEvent = btEvent;

    // reset cached pointers
    _fieldsDict = nullptr;
    _contextDict = nullptr;
    _streamEventContextDict = nullptr;
    _streamPacketContextDict = nullptr;

    /* In CTF, an event ID is unique within its _stream_, so in order
     * to keep a real unique ID for the whole trace, we include the
     * CTF stream ID and the CTF event ID in our version of an event ID.
     * The Event class user should not have to care about internal
     * concepts like streams, specific to a trace format.
     *
     * Left-shifting the stream ID by 20 positions makes it possible
     * to have 1 mibievents per stream and 4096 different streams per
     * trace, which seems reasonable.
     */
    auto tibeeBtCtfEvent = reinterpret_cast<::tibee_bt_ctf_event*>(btEvent);
    auto tibeeStream = tibeeBtCtfEvent->parent->stream;
    auto ctfEventId = static_cast<std::uint32_t>(tibeeStream->event_id);
    auto ctfStreamId = static_cast<std::uint32_t>(tibeeStream->stream_id);
    _id = (ctfStreamId << 20) | (ctfEventId & 0xfffff);

    /* Let's use the trace handle (an integer starting at 0) here, which
     * is unique for each trace in the same Babeltrace context (and we
     * only have one).
     */
    _traceId = tibeeStream->stream_class->trace->parent.handle->id;
}

}
}
