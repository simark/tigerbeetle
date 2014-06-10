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
#include <common/ex/WrongStateProvider.hpp>
#include "StateHistoryBuilder.hpp"
#include "ProgressPublisher.hpp"
#include "TraceDeck.hpp"
#include "Arguments.hpp"
#include "BuilderBeetle.hpp"
#include "ex/MqBindError.hpp"
#include "ex/UnknownStateProviderType.hpp"

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
    std::unique_ptr<common::TraceSet> traceSet {new common::TraceSet};

    // add traces to trace set
    for (const auto& tracePath : _args.traces) {
        if (!traceSet->addTrace(tracePath)) {
            std::cerr << "Error: could not add trace " << tracePath << std::endl;

            return false;
        }
    }

    // create a list of trace listeners
    std::vector<AbstractTracePlaybackListener::UP> listeners;

    // create a state history builder
    std::unique_ptr<StateHistoryBuilder> stateHistoryBuilder;
    try {
        stateHistoryBuilder = std::unique_ptr<StateHistoryBuilder> {
            new StateHistoryBuilder {
                _args.cacheDir,
                _args.stateProviders
            }
        };
    } catch (const common::ex::WrongStateProvider& ex) {
        std::cerr << "Error: wrong state provider: " <<
                     ex.getPath() << std::endl <<
                     "  " << ex.what() << std::endl;

        return false;
    } catch (const ex::UnknownStateProviderType& ex) {
        std::cerr << "Error: unknown state provider type: " <<
                     ex.getPath() << std::endl;

        return false;
    }

    listeners.push_back(std::move(stateHistoryBuilder));

    // create a progress publisher
    if (!_args.bindProgress.empty()) {
        std::unique_ptr<ProgressPublisher> progressPublisher;
        try {
            progressPublisher = std::unique_ptr<ProgressPublisher> {
                new ProgressPublisher {
                    _args.bindProgress,
                    traceSet->getBegin(),
                    traceSet->getEnd(),
                    _args.traces,
                    _args.stateProviders,
                    stateHistoryBuilder.get(),
                    2801,
                    200
                }
            };
        } catch (const ex::MqBindError& ex) {
            std::cerr << "Error: cannot bind to address \"" <<
                         ex.getBindAddr() << "\"" << std::endl;

            return false;
        }

        listeners.push_back(std::move(progressPublisher));
    }

    // ready for the deck
    return _traceDeck.play(traceSet.get(), listeners);
}

void BuilderBeetle::stop()
{
    _traceDeck.stop();
}

}
