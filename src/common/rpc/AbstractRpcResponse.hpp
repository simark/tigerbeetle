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
#ifndef _ABSTRACTRPCRESPONSE_HPP
#define _ABSTRACTRPCRESPONSE_HPP

#include <common/BasicTypes.hpp>
#include <common/rpc/IRpcMessage.hpp>
#include <common/rpc/RpcMessageType.hpp>

namespace tibee
{
namespace common
{

/**
 * Abstract RPC response. All RPC responses must inherit this class.
 *
 * @author Philippe Proulx
 */
class AbstractRpcResponse :
    public IRpcMessage
{
public:
    /**
     * Builds an abstract RPC response.
     */
    AbstractRpcResponse();

    virtual ~AbstractRpcResponse();

    RpcMessageType getType() const
    {
        return RpcMessageType::RESPONSE;
    }

    bool isRequest() const
    {
        return false;
    }

    bool isResponse() const
    {
        return true;
    }

    bool isNotification() const
    {
        return false;
    }

    /**
     * Returns whether this response contains an error.
     *
     * @returns True if this response has an error
     */
    bool hasError() const;

    /**
     * Sets the RPC response ID.
     *
     * @param id RPC response ID
     */
    void setId(rpc_msg_id_t id)
    {
        _id = id;
    }

    /**
     * Returns the RPC response ID.
     *
     * @returns RPC response ID.
     */
    rpc_msg_id_t getId() const
    {
        return _id;
    }

protected:
    virtual bool hasErrorImpl() const;


private:
    rpc_msg_id_t _id;
};

}
}

#endif // _ABSTRACTRPCRESPONSE_HPP