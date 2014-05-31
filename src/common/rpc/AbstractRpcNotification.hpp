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
#ifndef _ABSTRACTRPCNOTIFICATION_HPP
#define _ABSTRACTRPCNOTIFICATION_HPP

#include <string>

#include <common/rpc/IRpcMessage.hpp>
#include <common/rpc/RpcMessageType.hpp>

namespace tibee
{
namespace common
{

/**
 * Abstract RPC notification. All RPC notifications must inherit
 * this class.
 *
 * @author Philippe Proulx
 */
class AbstractRpcNotification :
    public IRpcMessage
{
public:
    /**
     * Builds an abstract RPC notification.
     */
    AbstractRpcNotification();

    virtual ~AbstractRpcNotification();

    RpcMessageType getType() const
    {
        return RpcMessageType::NOTIFICATION;
    }

    bool isRequest() const
    {
        return false;
    }

    bool isResponse() const
    {
        return false;
    }

    bool isNotification() const
    {
        return true;
    }

    /**
     * Returns the RPC request method name.
     *
     * @returns RPC request method name
     */
    const std::string& getMethod() const;

protected:
    virtual const std::string& getMethodImpl() const = 0;
};

}
}

#endif // _ABSTRACTRPCNOTIFICATION_HPP
