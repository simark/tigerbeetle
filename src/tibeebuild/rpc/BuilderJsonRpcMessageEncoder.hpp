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
#ifndef _BUILDERJSONRPCMESSAGEENCODER_HPP
#define _BUILDERJSONRPCMESSAGEENCODER_HPP

#include <memory>
#include <string>
#include <common/rpc/AbstractJsonRpcMessageEncoder.hpp>

#include "ProgressUpdateRpcNotification.hpp"

namespace tibee
{

/**
 * JSON-RPC message encoder for builder messages.
 *
 * @author Philippe Proulx
 */
class BuilderJsonRpcMessageEncoder :
    public common::AbstractJsonRpcMessageEncoder
{
public:
    /**
     * Builds a JSON-RPC encoder for builder messages.
     */
    BuilderJsonRpcMessageEncoder();

    /**
     * Encodes a ProgressUpdateRpcNotification object.
     *
     * @param object Object to encode
     */
    std::unique_ptr<std::string> encodeProgressUpdateRpcNotification(const ProgressUpdateRpcNotification& object);

protected:
    static bool encodeProgressUpdateRpcNotificationParams(const common::AbstractRpcMessage& msg, ::yajl_gen);
};

}

#endif // _BUILDERJSONRPCMESSAGEENCODER_HPP
