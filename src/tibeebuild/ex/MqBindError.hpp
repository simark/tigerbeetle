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
#ifndef _MQBINDERROR_HPP
#define _MQBINDERROR_HPP

#include <string>
#include <stdexcept>
#include <boost/filesystem/path.hpp>

namespace tibee
{
namespace ex
{

class MqBindError :
    public std::runtime_error
{
public:
    MqBindError(const std::string& bindAddr) :
        std::runtime_error {"message queue bind error"},
        _bindAddr {bindAddr}
    {
    }

    const std::string& getBindAddr() const {
        return _bindAddr;
    }

private:
    std::string _bindAddr;
};

}
}

#endif // _MQBINDERROR_HPP
