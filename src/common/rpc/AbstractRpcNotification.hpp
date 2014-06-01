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
#ifndef _TIBEE_COMMON_ABSTRACTRPCNOTIFICATION_HPP
#define _TIBEE_COMMON_ABSTRACTRPCNOTIFICATION_HPP

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
     *
     * @param method RPC notification method name
     */
    AbstractRpcNotification(const std::string& method);

    virtual ~AbstractRpcNotification();

    RpcMessageType getType() const;

    bool isRequest() const;

    bool isResponse() const;

    bool isNotification() const;

    /**
     * Returns the RPC request method name.
     *
     * @returns RPC request method name
     */
    const std::string& getMethod() const
    {
        return _method;
    }

protected:
    std::string _method;
};

}
}

#endif // _TIBEE_COMMON_ABSTRACTRPCNOTIFICATION_HPP
