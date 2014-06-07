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
#include <string>
#include <boost/filesystem/path.hpp>

#include <common/trace/EventValueType.hpp>
#include <common/trace/AbstractEventValue.hpp>
#include <common/mq/MqMessage.hpp>
#include "ProgressPublisher.hpp"
#include "ex/MqBindError.hpp"

namespace bfs = boost::filesystem;
namespace bptime = boost::posix_time;

namespace tibee
{

ProgressPublisher::ProgressPublisher(const std::string& bindAddr,
                                     common::timestamp_t beginTs, common::timestamp_t endTs,
                                     const std::vector<boost::filesystem::path>& tracesPaths,
                                     const std::vector<boost::filesystem::path>& stateProvidersPaths,
                                     const StateHistoryBuilder* stateHistoryBuilder,
                                     std::size_t updatePeriodEvents,
                                     std::size_t updatePeriodMs) :
    _bindAddr {bindAddr},
    _evCount {0},
    _rpcMessageEncoder {new BuilderJsonRpcMessageEncoder},
    _rpcNotification {new ProgressUpdateRpcNotification},
    _stateHistoryBuilder {stateHistoryBuilder},
    _updatePeriodEvents {updatePeriodEvents},
    _updatePeriodMs {updatePeriodMs},
    _tmpEvCounter {0},
    _lastTs {beginTs}
{
    // initially set progress update RPC notification
    _rpcNotification->setBeginTs(beginTs);
    _rpcNotification->setCurTs(beginTs);
    _rpcNotification->setEndTs(endTs);
    _rpcNotification->setTracesPaths(tracesPaths);
    _rpcNotification->setStateProvidersPaths(stateProvidersPaths);
    _rpcNotification->setStateChanges(0);

    // create and bind to message queue publish socket
    _mqContext = std::unique_ptr<common::MqContext> {
        new common::MqContext {1}
    };
    _mqSocket = _mqContext->createPublishSocket();

    if (!_mqSocket->bind(bindAddr)) {
        _mqSocket = nullptr;
        _mqContext = nullptr;

        throw ex::MqBindError {bindAddr};
    };
}

ProgressPublisher::~ProgressPublisher()
{
    _mqSocket = nullptr;
    _mqContext = nullptr;
}

bool ProgressPublisher::onStartImpl(const std::shared_ptr<const common::TraceSet>& traceSet)
{
    std::cout << "progress publisher: publishing start" << std::endl;

    // first publication
    _evCount = 0;
    _tmpEvCounter = 0;
    _lastTs = _rpcNotification->getBeginTs();
    _rpcNotification->setStateChanges(0);
    this->publish();

    // update last time
    _lastTime = bptime::microsec_clock::local_time();

    return true;
}

void ProgressPublisher::onEventImpl(const common::Event& event)
{
    // increase event count
    _evCount++;

    // update?
    if (_tmpEvCounter++ == _updatePeriodEvents) {
        // reset temporary counter
        _tmpEvCounter = 0;

        // really update?
        bptime::ptime curTime {bptime::microsec_clock::local_time()};

        if (curTime - _lastTime > bptime::milliseconds(_updatePeriodMs)) {
            // publish now
            _lastTs = event.getTimestamp();
            this->publish();

            // update last time
            _lastTime = curTime;
        }
    }
}

void ProgressPublisher::publish()
{
    // update RPC notification object
    _rpcNotification->setCurTs(_lastTs);
    _rpcNotification->setStateChanges(_stateHistoryBuilder->getStateChanges());
    _rpcNotification->setProcessedEvents(_evCount);

    // get JSON-RPC notification
    auto json = _rpcMessageEncoder->encodeProgressUpdateRpcNotification(*_rpcNotification);

    // create message to publish
    common::MqMessage::UP msg {new common::MqMessage {json->c_str(), json->size()}};

    // publish message
    _mqSocket->send(std::move(msg));
}

bool ProgressPublisher::onStopImpl()
{
    // publish one last time
    _lastTs = _rpcNotification->getEndTs();
    this->publish();

    return true;
}

}
