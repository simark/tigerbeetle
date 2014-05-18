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
#ifndef _STATEHISTORYBUILDER_HPP
#define _STATEHISTORYBUILDER_HPP

#include <vector>
#include <memory>
#include <boost/filesystem.hpp>

#include <common/trace/TraceSet.hpp>
#include <common/trace/Event.hpp>
#include "AbstractCacheBuilder.hpp"
#include "AbstractStateProvider.hpp"

namespace tibee
{

/**
 * State history builder.
 *
 * An instance of this class is responsible for building the state
 * history on disk during a trace playback.
 *
 * @author Philippe Proulx
 */
class StateHistoryBuilder :
    public AbstractCacheBuilder
{
public:
    /**
     * Builds a state history builder.
     *
     * @param dir            Cache directory
     * @param providersPaths List of state providers paths
     */
    StateHistoryBuilder(const boost::filesystem::path& dir,
                        const std::vector<boost::filesystem::path>& providersPaths);

    ~StateHistoryBuilder();

    bool onStart(const std::shared_ptr<const common::TraceSet>& traceSet);
    void onEvent(const common::Event& event);
    bool onStop();

private:
    std::vector<boost::filesystem::path> _providersPaths;
    std::vector<AbstractStateProvider::UP> _providers;
};

}

#endif // _STATEHISTORYBUILDER_HPP
