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
#ifndef _PROGRESSPUBLISHER_HPP
#define _PROGRESSPUBLISHER_HPP

#include <memory>
#include <string>
#include <boost/filesystem/path.hpp>

#include <common/trace/TraceSet.hpp>
#include <common/trace/Event.hpp>
#include "ITracePlaybackListener.hpp"

namespace tibee
{

/**
 * Abstract cache builder. All cache builders must inherit this.
 *
 * @author Philippe Proulx
 */
class ProgressPublisher :
    public ITracePlaybackListener
{
public:
    /**
     * Builds a progress publisher.
     *
     * @param bindAddr Bind address for publishing progress
     */
    ProgressPublisher(const std::string& bindAddr);

    ~ProgressPublisher();

    bool onStart(const std::shared_ptr<const common::TraceSet>& traceSet);
    void onEvent(const common::Event& event);
    bool onStop();

private:
    boost::filesystem::path _bindAddr;
    std::size_t _evCount;
};

}

#endif // _PROGRESSPUBLISHER_HPP
