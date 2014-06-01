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
    Event(const EventValueFactory* valueFactory);

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
     * Returns the event fields.
     *
     * The returned pointer is only valid while this event is valid.
     *
     * @returns Event fields or \a nullptr if not available
     */
    const DictEventValue* getFields() const;

    /**
     * Returns the event context.
     *
     * The returned pointer is only valid while this event is valid.
     *
     * @returns Event context or \a nullptr if not available
     */
    const DictEventValue* getContext() const;

    /**
     * Returns the stream event context.
     *
     * The returned pointer is only valid while this event is valid.
     *
     * @returns Stream event context or \a nullptr if not available
     */
    const DictEventValue* getStreamEventContext() const;

    /**
     * Returns the stream packet context.
     *
     * The returned pointer is only valid while this event is valid.
     *
     * @returns Stream packet context or \a nullptr if not available
     */
    const DictEventValue* getStreamPacketContext() const;

private:
    const DictEventValue* getTopLevelScope(::bt_ctf_scope topLevelScope) const;

private:
    ::bt_ctf_event* _btEvent;
    const EventValueFactory* _valueFactory;
};

}
}

#endif // _TIBEE_COMMON_EVENT_HPP
