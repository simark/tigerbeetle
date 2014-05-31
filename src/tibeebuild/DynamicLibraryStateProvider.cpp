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
#include "DynamicLibraryStateProvider.hpp"

namespace bfs = boost::filesystem;

namespace tibee
{

DynamicLibraryStateProvider::DynamicLibraryStateProvider(const boost::filesystem::path& path) :
    AbstractStateProvider {path}
{
    std::cout << "dynamic library state provider: loading library " << path << std::endl;
}

DynamicLibraryStateProvider::~DynamicLibraryStateProvider()
{
    std::cout << "dynamic library state provider: unloading library" << std::endl;
}

void DynamicLibraryStateProvider::onInit(common::CurrentState& state)
{
    std::cout << "dynamic library state provider: initializing" << std::endl;
}

void DynamicLibraryStateProvider::onEvent(common::CurrentState& state, const common::Event& event)
{
    // getting event
}

void DynamicLibraryStateProvider::onFini(common::CurrentState& state)
{
    std::cout << "dynamic library state provider: finishing" << std::endl;
}

}
