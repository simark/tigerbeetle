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
#ifndef _TIBEE_COMMON_EVENT_HPP
#define _TIBEE_COMMON_EVENT_HPP

#include <string>
#include <babeltrace/ctf/events.h>

#include <common/BasicTypes.hpp>
#include <common/trace/DictEventValue.hpp>
#include <common/trace/EventValueFactory.hpp>

namespace tibee
{
namespace common
{

/**
 * An event, the object returned by a TraceSetIterator.
 *
 * @author Philippe Proulx
 */
class Event
{
    friend class TraceSetIterator;

public:
    /**
     * Returns the event name.
     *
     * @returns Event name
     */
    const char* getName() const;

    /**
     * Returns a copy of the event name.
     *
     * @returns Event name copy
     */
    std::string getNameStr() const;

    /**
     * Returns the cycle count of this event.
     *
     * @returns Cycle count
     */
    trace_cycles_t getCycles() const;

    /**
     * Returns the event timestamp.
     *
     * @returns Event timestamp
     */
    timestamp_t getTimestamp() const;

    /**
     * Returns the event fields dictionary.
     *
     * The returned pointer is only valid while this event is valid.
     *
     * Once this dictionary is created, a copy of the pointer is cached
     * by the owning event. The copy is kept as long as this event remains
     * valid.
     *
     * @returns Event fields dictionary or \a nullptr if not available
     */
    const DictEventValue* getFields();

    /**
     * Returns the event context dictionary.
     *
     * The returned pointer is only valid while this event is valid.
     *
     * Once this dictionary is created, a copy of the pointer is cached
     * by the owning event. The copy is kept as long as this event remains
     * valid.
     *
     * @returns Event context or \a nullptr if not available
     */
    const DictEventValue* getContext();

    /**
     * Returns the stream event context dictionary.
     *
     * The returned pointer is only valid while this event is valid.
     *
     * Once this dictionary is created, a copy of the pointer is cached
     * by the owning event. The copy is kept as long as this event remains
     * valid.
     *
     * @returns Stream event context or \a nullptr if not available
     */
    const DictEventValue* getStreamEventContext();

    /**
     * Returns the stream packet context dictionary.
     *
     * The returned pointer is only valid while this event is valid.
     *
     * Once this dictionary is created, a copy of the pointer is cached
     * by the owning event. The copy is kept as long as this event remains
     * valid.
     *
     * @returns Stream packet context or \a nullptr if not available
     */
    const DictEventValue* getStreamPacketContext();

    /**
     * Returns a specific event field value using its name.
     *
     * Once at least one field value is accessed, the created fields
     * dictionary is cached by the owning event. The copy is kept as
     * long as this event remains valid.
     *
     * @param name Name of field value to retrieve
     * @returns    Retrieved field value
     */
    const AbstractEventValue* operator[](const char* name);

    /**
     * @see operator[](const char*)
     */
    const AbstractEventValue* operator[](const std::string& name);

    /**
     * Returns a specific event field value using its numeric index.
     *
     * Same as operator[](const char* name), but using the numeric
     * index of the field value to retrieve.
     *
     * Caller must make sure the value exists for the specified
     * index.
     *
     * @param index Index of field value to retrieve
     * @returns     Retrieved field value
     */
    const AbstractEventValue* operator[](std::size_t index);

    /**
     * Returns this event's numeric ID.
     *
     * @returns Event numeric ID
     */
    const event_id_t getId() const
    {
        return _id;
    }

    /**
     * Returns this event's trace numeric ID.
     *
     * @returns Numeric ID of trace this event is in
     */
    const trace_id_t getTraceId() const
    {
        return _traceId;
    }

private:
    Event(const EventValueFactory* valueFactory);
    const DictEventValue* getTopLevelScope(::bt_ctf_scope topLevelScope);
    void setPrivateEvent(::bt_ctf_event* btEvent);

private:
    ::bt_ctf_event* _btEvent;
    const EventValueFactory* _valueFactory;
    const DictEventValue* _fieldsDict;
    const DictEventValue* _contextDict;
    const DictEventValue* _streamEventContextDict;
    const DictEventValue* _streamPacketContextDict;
    event_id_t _id;
    trace_id_t _traceId;
};

}
}

#endif // _TIBEE_COMMON_EVENT_HPP
