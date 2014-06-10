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
#include <common/stateprov/PythonStateProvider.hpp>

namespace bfs = boost::filesystem;

namespace tibee
{
namespace common
{

PythonStateProvider::PythonStateProvider(const boost::filesystem::path& path) :
    AbstractStateProviderFile {path}
{
    std::cout << "python state provider: loading script " << path << std::endl;
}

PythonStateProvider::~PythonStateProvider()
{
    std::cout << "python state provider: unloading script" << std::endl;
}

void PythonStateProvider::onInitImpl(CurrentState& state)
{
    std::cout << "python state provider: initializing" << std::endl;
}

void PythonStateProvider::onEventImpl(CurrentState& state, Event& event)
{
    // getting event
}

void PythonStateProvider::onFiniImpl(CurrentState& state)
{
    std::cout << "python state provider: finishing" << std::endl;
}

}
}
