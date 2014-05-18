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
#include <boost/filesystem/path.hpp>

#include <common/trace/EventValueType.hpp>
#include <common/trace/AbstractEventValue.hpp>
#include "ProgressPublisher.hpp"

namespace bfs = boost::filesystem;

namespace tibee
{

ProgressPublisher::ProgressPublisher(const std::string& bindAddr) :
    _bindAddr {bindAddr},
    _evCount {0}
{
    std::cout << "progress publisher: binding socket to " << bindAddr << std::endl;
}

ProgressPublisher::~ProgressPublisher()
{
    std::cout << "progress publisher: closing socket to " << std::endl;
}

bool ProgressPublisher::onStart(const std::shared_ptr<const common::TraceSet>& traceSet)
{
    std::cout << "progress publisher: publishing start" << std::endl;

    _evCount = 0;

    return true;
}

void ProgressPublisher::onEvent(const common::Event& event)
{
    _evCount++;
    if (_evCount % 5 == 0) {
        std::cout << "progress publisher: publishing: at event #" << _evCount << std::endl;
    }
}

bool ProgressPublisher::onStop()
{
    std::cout << "progress publisher: publishing stop" << std::endl;

    return true;
}

}
