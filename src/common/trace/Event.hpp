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
#ifndef _EVENT_HPP
#define _EVENT_HPP

#include <cstdint>
#include <string>
#include <babeltrace/ctf/events.h>

#include <common/BasicTypes.hpp>
#include <common/trace/DictEventValue.hpp>

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
public:
    Event();

    void setInternalPtr(::bt_ctf_event* internalPtr)
    {
        _btEvent = internalPtr;
    }

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
    trace_ts_t getTimestamp() const;

    /**
     * Returns the event fields.
     *
     * @returns Event fields or \a nullptr if not available
     */
    DictEventValue::UP getFields() const;

    /**
     * Returns the event context.
     *
     * @returns Event context or \a nullptr if not available
     */
    DictEventValue::UP getContext() const;

    /**
     * Returns the stream event context
     *
     * @returns Stream event context or \a nullptr if not available
     */
    DictEventValue::UP getStreamEventContext() const;

    /**
     * Returns the stream packet context
     *
     * @returns Stream packet context or \a nullptr if not available
     */
    DictEventValue::UP getStreamPacketContext() const;

private:
    DictEventValue::UP getTopLevelScope(::bt_ctf_scope topLevelScope) const;

private:
    ::bt_ctf_event* _btEvent;
};

}
}

#endif // _EVENT_HPP
