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
#ifndef _BASIC_TYPES_HPP
#define _BASIC_TYPES_HPP

#include <cstdint>

namespace tibee
{
namespace common
{

/**
 * @file
 * This header holds basic type definitions used throughout tigerbeetle.
 */

/// Trace/state timestamp
typedef std::uint64_t   timestamp_t;

/// Trace cycles
typedef std::uint64_t   trace_cycles_t;

/// State quark
typedef std::uint32_t   quark_t;

/// RPC message ID
typedef std::uint32_t   rpc_msg_id_t;

}
}

#endif // _BASIC_TYPES_HPP
