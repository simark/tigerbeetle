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
#include <boost/filesystem/path.hpp>

#include <common/trace/EventValueType.hpp>
#include <common/trace/AbstractEventValue.hpp>
#include "AbstractStateProvider.hpp"
#include "PythonStateProvider.hpp"

namespace bfs = boost::filesystem;

namespace tibee
{

PythonStateProvider::PythonStateProvider(const boost::filesystem::path& path) :
    AbstractStateProvider {path}
{
    std::cout << "python state provider: loading script " << path << std::endl;
}

PythonStateProvider::~PythonStateProvider()
{
    std::cout << "python state provider: unloading script" << std::endl;
}

bool PythonStateProvider::onStart(const std::shared_ptr<const common::TraceSet>& traceSet)
{
    std::cout << "python state provider: starting" << std::endl;

    return true;
}

void PythonStateProvider::onEvent(const common::Event& event)
{
}

bool PythonStateProvider::onStop()
{
    std::cout << "python state provider: stopping" << std::endl;

    return true;
}

}
