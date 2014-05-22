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
#include <babeltrace/ctf/iterator.h>

#include <common/trace/TraceSetIterator.hpp>
#include <common/trace/Event.hpp>
#include <common/trace/TraceSet.hpp>
#include <common/ex/TraceSet.hpp>

namespace tibee
{
namespace common
{

TraceSet::TraceSet()
{
    _btCtx = ::bt_context_create();

    if (!_btCtx) {
        throw ex::TraceSet {"cannot create Babeltrace context"};
    }

    ::bt_iter_pos beginPos;
    beginPos.type = ::BT_SEEK_BEGIN;
    beginPos.u.seek_time = 0;

    _btCtfIter = ::bt_ctf_iter_create(_btCtx, &beginPos, nullptr);

    if (!_btCtfIter) {
        ::bt_context_put(_btCtx);

        throw ex::TraceSet {"cannot create Babeltrace iterator"};
    }

    _btIter = ::bt_ctf_get_iter(_btCtfIter);
}

TraceSet::~TraceSet()
{
    ::bt_ctf_iter_destroy(_btCtfIter);
    ::bt_context_put(_btCtx);
}

void TraceSet::seekBegin() const
{
    ::bt_iter_pos beginPos;
    beginPos.type = ::BT_SEEK_BEGIN;
    beginPos.u.seek_time = 0;

    ::bt_iter_set_pos(_btIter, &beginPos);
}

bool TraceSet::addTrace(const boost::filesystem::path& path)
{
    if (_tracePaths.find(path) != _tracePaths.end()) {
        // already added
        return false;
    }

    // TODO: eventually, here would be the place to detect the trace type

    auto ret = ::bt_context_add_trace(_btCtx, path.string().c_str(), "ctf",
                                      nullptr, nullptr, nullptr);

    if (ret < 0) {
        // Babeltrace error
        return false;
    }

    // add to our set now
    _tracePaths.insert(path);

    return true;
}

trace_ts_t TraceSet::getBegin() const
{
    // ignore if no trace is loaded
    if (_tracePaths.empty()) {
        return -1;
    }

    // save position (iterator might be shared)
    auto savedPos = ::bt_iter_get_pos(_btIter);

    // go to beginning
    this->seekBegin();

    // read event
    auto event = bt_ctf_iter_read_event(_btCtfIter);

    if (!event) {
        ::bt_iter_set_pos(_btIter, savedPos);
        ::bt_iter_free_pos(savedPos);
        return -1;
    }

    // read event timestamp
    auto ts = ::bt_ctf_get_timestamp(event);

    // restore saved position
    ::bt_iter_set_pos(_btIter, savedPos);
    ::bt_iter_free_pos(savedPos);

    return static_cast<trace_ts_t>(ts);
}

trace_ts_t TraceSet::getEnd() const
{
    // ignore if no trace is loaded
    if (_tracePaths.empty()) {
        return -1;
    }

    // save position (iterator might be shared)
    auto savedPos = ::bt_iter_get_pos(_btIter);

    // go to end
    ::bt_iter_pos endPos;
    endPos.type = ::BT_SEEK_LAST;
    endPos.u.seek_time = 0;
    ::bt_iter_set_pos(_btIter, &endPos);

    // read event
    auto event = bt_ctf_iter_read_event(_btCtfIter);

    if (!event) {
        ::bt_iter_set_pos(_btIter, savedPos);
        ::bt_iter_free_pos(savedPos);
        return -1;
    }

    // read event timestamp
    auto ts = ::bt_ctf_get_timestamp(event);

    // restore saved position
    ::bt_iter_set_pos(_btIter, savedPos);
    ::bt_iter_free_pos(savedPos);

    return static_cast<trace_ts_t>(ts);
}


TraceSet::iterator TraceSet::begin() const
{
    // go back to beginning (will also affect all existing iterators)
    this->seekBegin();

    // create new iterator
    return TraceSet::iterator {_btCtfIter};
}


TraceSet::iterator TraceSet::end() const
{
    // "end" is just a null iterator
    return TraceSet::iterator {nullptr};
}

}
}
