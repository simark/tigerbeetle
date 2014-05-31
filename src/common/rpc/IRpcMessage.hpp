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
#ifndef _IRPCMESSAGE_HPP
#define _IRPCMESSAGE_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

#include <common/rpc/RpcMessageType.hpp>

namespace tibee
{
namespace common
{

/**
 * RPC message interface. All RPC messages must implement this interface.
 *
 * @author Philippe Proulx
 */
class IRpcMessage
{
public:
    virtual ~IRpcMessage() = 0;

    /**
     * Returns the type of this message.
     *
     * @returns Message type
     */
    virtual RpcMessageType getType() = 0;

    /**
     * Returns whether or not this message is a request.
     *
     * @returns True if this message is a request
     */
    virtual bool isRequest() const = 0;

    /**
     * Returns whether or not this message is a response.
     *
     * @returns True if this message is a response
     */
    virtual bool isResponse() const = 0;

    /**
     * Returns whether or not this message is a notification.
     *
     * @returns True if this message is a notification
     */
    virtual bool isNotification() const = 0;
};

}
}

#endif // _IRPCMESSAGE_HPP
