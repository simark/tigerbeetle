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
#ifndef _ITRACEPLAYBACKLISTENER_HPP
#define _ITRACEPLAYBACKLISTENER_HPP

#include <memory>

#include <common/trace/TraceSet.hpp>
#include <common/trace/Event.hpp>

namespace tibee
{

/**
 * Trace playback observer interface.
 *
 * This is a simple interface which gets notified when beginning the
 * playback of a trace, on each event, and at the end.
 *
 * @author Philippe Proulx
 */
class ITracePlaybackListener
{
public:
    /// Unique pointer to track playback listener
    typedef std::unique_ptr<ITracePlaybackListener> UP;

public:
    /**
     * Playback start notification.
     *
     * @param traceSet Trace set which will be used for reading
     * @returns        True if the initialization was successful
     */
    virtual bool onStart(const std::shared_ptr<const common::TraceSet>& traceSet) = 0;

    /**
     * New event notification.
     *
     * @param event New event
     */
    virtual void onEvent(const common::Event& event) = 0;

    /**
     * Playback stop notification.
     *
     * @returns True if the finalization was successful
     */
    virtual bool onStop() = 0;
};

}

#endif // _ITRACEPLAYBACKLISTENER_HPP
