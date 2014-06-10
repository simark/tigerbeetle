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
#ifndef _TIBEE_COMMON_ABSTRACTSTATEPROVIDER_HPP
#define _TIBEE_COMMON_ABSTRACTSTATEPROVIDER_HPP

#include <functional>
#include <memory>

#include <common/BasicTypes.hpp>
#include <common/state/CurrentState.hpp>
#include <common/trace/Event.hpp>
#include <common/trace/TraceSet.hpp>

namespace tibee
{
namespace common
{

/**
 * An abstract state provider. Any state provider must inherit
 * this class.
 *
 * @author Philippe Proulx
 */
class AbstractStateProvider
{
public:
    /// Unique pointer to abstract state provider
    typedef std::unique_ptr<AbstractStateProvider> UP;

    /// On event function
    typedef std::function<bool (CurrentState& state, Event& event)> OnEventFunction;

public:
    /**
     * Builds a state provider.
     */
    AbstractStateProvider();

    virtual ~AbstractStateProvider();

    /**
     * Called before processing any event.
     *
     * @param state    Current state (empty when getting it)
     * @param traceSet Trace set used for this state construction
     */
    void onInit(CurrentState& state, const TraceSet* traceSet);

    /**
     * Called on each event.
     *
     * @param state Current state
     * @param event New event
     * @returns     True to continue
     */
    bool onEvent(CurrentState& state, Event& event);

    /**
     * Called after having processed all events.
     *
     * @param state Current state
     */
    void onFini(CurrentState& state);

protected:
    /**
     * Registers an event callback to be called when an event matches
     * the specified (trace type, event name) pair.
     *
     * LTTng kernel traces have the trace type "lttng-kernel". LTTng
     * UST traces have the trace type "lttng-ust" and their event names
     * contain the UST provider name, followed by ":", followed by the
     * tracepoint name, e.g. "my_application:my_tracepoint_name".
     *
     * An empty trace type means "match all trace types not matched by
     * any other callback registered so far for the same event name".
     *
     * An empty event name means "match all events not matched by any
     * other callback registered so far for the same trace type".
     *
     * If both \p traceType and \p eventName are empty, the callback
     * is registered for all (trace type, event name) pairs which do
     * not have any registered callback so far.
     *
     * Be aware that calling this method shall _not_ override anything
     * previously set that matches the same constraints.
     *
     * The method returns true if there was at least one match for the
     * specified constraints.
     *
     * @param traceType Trace type
     * @param eventName Event name (empty string to match all)
     * @param onEvent   Callback function to be called during state construction
     * @returns         True if at least one event matched the constraints
     */
    bool registerEventCallback(const std::string& traceType,
                               const std::string& eventName,
                               const OnEventFunction& onEvent);

private:
    virtual void onInitImpl(CurrentState& state,
                            const TraceSet* traceSet);

    virtual void onFiniImpl(CurrentState& state);

    /**
     * Match function when building the infamous map.
     *
     * Handles direct matches and wildcards.
     */
    bool namesMatch(const std::string& asked, const std::string& candidate)
    {
        return asked.empty() || asked == candidate;
    }

private:
    typedef std::map<event_id_t, OnEventFunction> EventIdCallbackMap;
    typedef std::map<trace_id_t, EventIdCallbackMap> TraceIdEventIdCallbackMap;

private:
    TraceIdEventIdCallbackMap _infamousMap;
    const TraceSet* _curTraceSet;
};

}
}

#endif // _TIBEE_COMMON_ABSTRACTSTATEPROVIDER_HPP
