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
#ifndef _ABSTRACTJSONRPCMESSAGEENCODER_HPP
#define _ABSTRACTJSONRPCMESSAGEENCODER_HPP

#include <yajl/yajl_gen.h>
#include <vector>
#include <cstdint>
#include <memory>
#include <functional>

#include <common/rpc/IRpcMessage.hpp>
#include <common/rpc/AbstractRpcRequest.hpp>
#include <common/rpc/AbstractRpcResponse.hpp>
#include <common/rpc/AbstractRpcNotification.hpp>

namespace tibee
{
namespace common
{

/**
 * Abstract JSON-RPC message encoder. All concrete JSON-RPC encoders
 * must inherit this class.
 *
 * This class provides JSON utilities.
 *
 * @author Philippe Proulx
 */
class AbstractJsonRpcMessageEncoder
{
public:
    typedef std::unique_ptr<std::vector<std::uint8_t>> ByteArray;

public:
    /**
     * Builds an abstract JSON-RPC message encoder.
     */
    AbstractJsonRpcMessageEncoder();

    virtual ~AbstractJsonRpcMessageEncoder();

    /**
     * Encodes an RPC message to a byte array.
     *
     * @param msg RPC message to encode
     * @returns   Encoded message or \a nullptr if an error occured
     */
    ByteArray encode(const IRpcMessage& msg);

protected:
    typedef std::function<bool (const IRpcMessage& msg, ::yajl_gen)> ObjectEncodeFunc;

protected:
    virtual ByteArray encodeRequestImpl(const AbstractRpcRequest& request);
    virtual ByteArray encodeResponseImpl(const AbstractRpcResponse& response);
    virtual ByteArray encodeNotificationImpl(const AbstractRpcNotification& notification);

    /**
     * Encodes an RPC request as JSON-RPC.
     *
     * @param request          RPC request to encode
     * @param paramsEncodeFunc Encode function to use for parameters
     * @returns                Encoded request or \a nullptr if an error occured
     */
    ByteArray encodeRequest(const AbstractRpcRequest& request,
                            const ObjectEncodeFunc& paramsEncodeFunc);

    /**
     * Encodes an RPC response as JSON-RPC.
     *
     * @param response         RPC response to encode
     * @param resultEncodeFunc Encode function to use for result
     * @param errorEncodeFunc  Encode function to use for error
     * @returns                Encoded response or \a nullptr if an error occured
     */
    ByteArray encodeResponse(const AbstractRpcResponse& response,
                             const ObjectEncodeFunc& resultEncodeFunc,
                             const ObjectEncodeFunc& errorEncodeFunc);

    /**
     * Encodes an RPC notification as JSON-RPC.
     *
     * @param notification     RPC notification to encode
     * @param paramsEncodeFunc Encode function to use for parameters
     * @returns                Encoded notification or \a nullptr if an error occured
     */
    ByteArray encodeNotification(const AbstractRpcNotification& notification,
                                 const ObjectEncodeFunc& paramsEncodeFunc);

private:
    ByteArray getCurrentByteArray();

private:
    ::yajl_gen _yajlGen;
};

}
}

#endif // _ABSTRACTJSONRPCMESSAGEENCODER_HPP
