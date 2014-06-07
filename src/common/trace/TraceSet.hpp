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
#ifndef _TIBEE_COMMON_TRACESET_HPP
#define _TIBEE_COMMON_TRACESET_HPP

#include <memory>
#include <cstdint>
#include <set>
#include <boost/filesystem.hpp>
#include <boost/utility.hpp>
#include <babeltrace/babeltrace.h>

#include <common/BasicTypes.hpp>
#include <common/trace/TraceSetIterator.hpp>

namespace tibee
{
namespace common
{

/**
 * A trace set.
 *
 * All sorts of traces may be added to this set and then events may be
 * read in a trace format-agnostic way.
 *
 * Trace formats are automagically recognized, either using file
 * extensions or by inspecting the actual data or directory structure.
 *
 * @author Philippe Proulx
 */
class TraceSet :
    boost::noncopyable
{
public:
    typedef TraceSetIterator iterator;

public:
    /**
     * Builds an empty trace set.
     */
    TraceSet();

    virtual ~TraceSet();

    /**
     * Adds trace with path \p path to the trace set.
     *
     * @param path Path of trace to add
     * @returns    True if the trace was successfully added
     */
    bool addTrace(const boost::filesystem::path& path);

    /**
     * Returns whether a given file path points to a known trace format.
     *
     * @returns True if \p path points to a known trace format
     */
    static bool isFormatKnown(const boost::filesystem::path& path);

    /**
     * Returns the begin timestamp of the set.
     *
     * @returns Begin timestamp of the set
     */
    timestamp_t getBegin() const;

    /**
     * Returns the end timestamp of the set.
     *
     * @returns End timestamp of the set
     */
    timestamp_t getEnd() const;

    /**
     * Returns an iterator pointing to the first event of the set.
     *
     * @returns Iterator pointing to the first event of the set
     */
    iterator begin() const;

    /**
     * Returns an iterator pointing after the last event of the set.
     *
     * @returns Iterator pointing after the last event of the set
     */
    iterator end() const;

private:
    void seekBegin() const;

private:
    std::set<boost::filesystem::path> _tracePaths;
    ::bt_context* _btCtx;
    ::bt_iter* _btIter;
    ::bt_ctf_iter* _btCtfIter;
};

}
}

#endif // _TIBEE_COMMON_TRACESET_HPP
