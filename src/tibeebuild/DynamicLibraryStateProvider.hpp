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
#ifndef _DYNAMICLIBRARYSTATEPROVIDER_HPP
#define _DYNAMICLIBRARYSTATEPROVIDER_HPP

#include <string>
#include <boost/filesystem.hpp>

#include <common/trace/EventValueType.hpp>
#include <common/trace/AbstractEventValue.hpp>
#include "AbstractStateProvider.hpp"

namespace tibee
{

/**
 * A state provider which loads a dynamic library and calls specific
 * functions to obtain state informations.
 *
 * @author Philippe Proulx
 */
class DynamicLibraryStateProvider :
    public AbstractStateProvider
{
public:
    /**
     * Builds a dynamic library state provider.
     *
     * @param path Dynamic library path
     */
    DynamicLibraryStateProvider(const boost::filesystem::path& path);

    ~DynamicLibraryStateProvider();

protected:
    static std::string getErrorMsg(const std::string& base);

private:
    void onInitImpl(common::CurrentState& state);
    void onEventImpl(common::CurrentState& state, common::Event& event);
    void onFiniImpl(common::CurrentState& state);

private:
    // DL handle
    void* _dlHandle;

    // DL resolved symbols
    void (*_dlOnInit)(common::CurrentState&);
    void (*_dlOnEvent)(common::CurrentState&, common::Event&);
    void (*_dlOnFini)(common::CurrentState&);
};

}

#endif // _DYNAMICLIBRARYSTATEPROVIDER_HPP
