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
#include <memory>
#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>

#include <common/trace/TraceSet.hpp>
#include "StateHistoryBuilder.hpp"
#include "ProgressPublisher.hpp"
#include "TraceDeck.hpp"
#include "Arguments.hpp"
#include "BuilderBeetle.hpp"

namespace bfs = boost::filesystem;

namespace tibee
{

BuilderBeetle::BuilderBeetle(const Arguments& args) :
    _args(args)
{
}

bool BuilderBeetle::run()
{
    // create a trace set
    std::shared_ptr<common::TraceSet> traceSet {new common::TraceSet};

    // add traces to trace set
    for (const auto& tracePath : _args.traces) {
        if (!traceSet->addTrace(tracePath)) {
            std::cerr << "could not add trace " << tracePath << std::endl;
            return false;
        }
    }

    // create a state history builder
    std::unique_ptr<StateHistoryBuilder> stateHistoryBuilder {
        new StateHistoryBuilder {
            _args.cacheDir,
            _args.stateProviders
        }
    };

    // create a progress publisher
    std::unique_ptr<ProgressPublisher> progressPublisher {
        new ProgressPublisher {
            _args.bindProgress,
            traceSet->getBegin(),
            traceSet->getEnd(),
            _args.traces,
            _args.stateProviders,
            *stateHistoryBuilder,
            10000,
            250
        }
    };

    // create a list of trace listeners
    std::vector<ITracePlaybackListener::UP> listeners;

    // add listeners
    listeners.push_back(std::move(stateHistoryBuilder));
    listeners.push_back(std::move(progressPublisher));

    // ready for the deck
    return _traceDeck.play(traceSet, listeners);
}

void BuilderBeetle::stop()
{
    _traceDeck.stop();
}

}
