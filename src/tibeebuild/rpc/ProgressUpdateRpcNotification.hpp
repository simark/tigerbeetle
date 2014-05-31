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
#ifndef _PROGRESSUPDATERPCNOTIFICATION_HPP
#define _PROGRESSUPDATERPCNOTIFICATION_HPP

#include <cstddef>

#include <common/rpc/AbstractRpcNotification.hpp>

namespace tibee
{

/**
 * Progress update RPC notification.
 *
 * This message is normally sent periodically to progress update
 * listeners to show progress updates to the user during a cache
 * construction.
 *
 * @author Philippe Proulx
 */
class ProgressUpdateRpcNotification :
    public common::AbstractRpcNotification
{
public:
    /**
     * Builds a progress update RPC notification.
     *
     * @param processedEvents Number of processed events.
     */
    ProgressUpdateRpcNotification();

    /**
     * Sets the number of processed events so far.
     *
     * @param processedEvents Number of processed events so far
     */
    void setProcessedEvents(unsigned int processedEvents)
    {
        _processedEvents = processedEvents;
    }

    /**
     * Returns the number of processed events so far.
     *
     * @returns Processed events so far
     */
    std::size_t getProcessedEvents() const
    {
        return _processedEvents;
    }

private:
    unsigned int _processedEvents;
};

}

#endif // _PROGRESSUPDATERPCNOTIFICATION_HPP
