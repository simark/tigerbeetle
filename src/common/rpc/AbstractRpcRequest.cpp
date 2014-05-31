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
#include <common/rpc/AbstractRpcRequest.hpp>

namespace tibee
{
namespace common
{

AbstractRpcRequest::AbstractRpcRequest(rpc_msg_id_t id) :
    _id {id}
{
}

AbstractRpcRequest::~AbstractRpcRequest()
{
}

const std::string& AbstractRpcRequest::getMethod() const
{
    return this->getMethodImpl();
}

}
}
