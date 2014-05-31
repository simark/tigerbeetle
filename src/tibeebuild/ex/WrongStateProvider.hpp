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
#ifndef _WRONGSTATEPROVIDER_HPP
#define _WRONGSTATEPROVIDER_HPP

#include <string>
#include <stdexcept>
#include <boost/filesystem/path.hpp>

namespace tibee
{
namespace ex
{

class WrongStateProvider :
    public std::runtime_error
{
public:
    WrongStateProvider(const std::string& msg, const boost::filesystem::path& path) :
        std::runtime_error {msg},
        _path {path}
    {
    }

    const boost::filesystem::path& getPath() const {
        return _path;
    }

private:
    boost::filesystem::path _path;
};

}
}

#endif // _WRONGSTATEPROVIDER_HPP
