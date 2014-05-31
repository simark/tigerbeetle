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
#include <dlfcn.h>
#include <string>
#include <boost/filesystem/path.hpp>

#include <common/trace/EventValueType.hpp>
#include <common/trace/AbstractEventValue.hpp>
#include "AbstractStateProvider.hpp"
#include "DynamicLibraryStateProvider.hpp"
#include "ex/WrongStateProvider.hpp"

namespace bfs = boost::filesystem;

namespace tibee
{

DynamicLibraryStateProvider::DynamicLibraryStateProvider(const boost::filesystem::path& path) :
    AbstractStateProvider {path},
    _dlHandle {nullptr}
{
    std::cout << "dynamic library state provider: loading library " << path << std::endl;

    // try loading the dynamic library
    _dlHandle = ::dlopen(path.string().c_str(), RTLD_NOW);

    if (!_dlHandle) {
        throw ex::WrongStateProvider {
            DynamicLibraryStateProvider::getErrorMsg("cannot load dynamic library"),
            path
        };
    }

    // resolve symbols
    _dlOnInit = reinterpret_cast<decltype(_dlOnInit)>(::dlsym(_dlHandle, "onInit"));

    if (!_dlOnInit) {
        ::dlclose(_dlHandle);

        throw ex::WrongStateProvider {
            DynamicLibraryStateProvider::getErrorMsg("cannot find \"onInit\" symbol"),
            path
        };
    }

    _dlOnEvent = reinterpret_cast<decltype(_dlOnEvent)>(::dlsym(_dlHandle, "onEvent"));

    if (!_dlOnInit) {
        ::dlclose(_dlHandle);

        throw ex::WrongStateProvider {
            DynamicLibraryStateProvider::getErrorMsg("cannot find \"onEvent\" symbol"),
            path
        };
    }

    _dlOnFini = reinterpret_cast<decltype(_dlOnFini)>(::dlsym(_dlHandle, "onFini"));

    if (!_dlOnInit) {
        ::dlclose(_dlHandle);

        throw ex::WrongStateProvider {
            DynamicLibraryStateProvider::getErrorMsg("cannot find \"onFini\" symbol"),
            path
        };
    }
}

std::string DynamicLibraryStateProvider::getErrorMsg(const std::string& base)
{
    std::string msg {base};
    const char* dlErr = ::dlerror();

    if (dlErr) {
        msg += ": ";
        msg += dlErr;
    }

    return msg;
}

DynamicLibraryStateProvider::~DynamicLibraryStateProvider()
{
    std::cout << "dynamic library state provider: unloading library" << std::endl;

    if (_dlHandle) {
        ::dlclose(_dlHandle);
    }
}

void DynamicLibraryStateProvider::onInit(common::CurrentState& state)
{
    // delegate
    _dlOnInit(state);
}

void DynamicLibraryStateProvider::onEvent(common::CurrentState& state, const common::Event& event)
{
    // delegate
    _dlOnEvent(state, event);
}

void DynamicLibraryStateProvider::onFini(common::CurrentState& state)
{
    // delegate
    _dlOnFini(state);
}

}
