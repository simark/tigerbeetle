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
#ifndef _ABSTRACTRPCREQUEST_HPP
#define _ABSTRACTRPCREQUEST_HPP

#include <string>

#include <common/BasicTypes.hpp>
#include <common/rpc/IRpcMessage.hpp>
#include <common/rpc/RpcMessageType.hpp>

namespace tibee
{
namespace common
{

/**
 * Abstract RPC request. All RPC requests must inherit this class.
 *
 * @author Philippe Proulx
 */
class AbstractRpcRequest :
    public IRpcMessage
{
public:
    /**
     * Builds an abstract RPC request.
     *
     * @param id     RPC request ID
     */
    AbstractRpcRequest(rpc_msg_id_t id);

    virtual ~AbstractRpcRequest();

    RpcMessageType getType() const
    {
        return RpcMessageType::REQUEST;
    }

    bool isRequest() const
    {
        return true;
    }

    bool isResponse() const
    {
        return false;
    }

    bool isNotification() const
    {
        return false;
    }

    /**
     * Returns the RPC request method name.
     *
     * @returns RPC request method name
     */
    const std::string& getMethod() const;

    /**
     * Returns the RPC request ID.
     *
     * @returns RPC request ID.
     */
    rpc_msg_id_t getId() const
    {
        return _id;
    }

protected:
    virtual const std::string& getMethodImpl() const = 0;

private:
    rpc_msg_id_t _id;
};

}
}

#endif // _ABSTRACTRPCREQUEST_HPP
