#include <iostream>
#include <cassert>
#include <cstring>

#include <common/state/CurrentState.hpp>
#include <common/stateprov/DynamicLibraryStateProvider.hpp>
#include <common/trace/Event.hpp>
#include <common/trace/TraceSet.hpp>

using namespace tibee;
using namespace tibee::common;

namespace
{

void printEventDetails(Event& event)
{
    auto fields = event.getFields();

    if (fields) {
        std::cout << "<" << event.getName() << "> " << "(" <<
                     event.getTraceId() <<
                     ", " <<
                     event.getId() <<
                     ")  " << fields->toString() <<
                     std::endl;
    }
}

bool onSchedSwitch(CurrentState& state, Event& event)
{
    assert(std::strcmp(event.getName(), "sched_switch") == 0);

    std::cout << "sched switching!" << std::endl;

    printEventDetails(event);

    return true;
}

bool onSysOpen(CurrentState& state, Event& event)
{
    assert(std::strcmp(event.getName(), "sys_open") == 0);

    std::cout << "sys opening!" << std::endl;

    printEventDetails(event);

    return true;
}

bool onSysClose(CurrentState& state, Event& event)
{
    assert(std::strcmp(event.getName(), "sys_close") == 0);

    std::cout << "sys closing!" << std::endl;

    printEventDetails(event);

    return true;
}

bool onEvent(CurrentState& state, Event& event)
{
    assert(std::strcmp(event.getName(), "sched_switch") != 0);
    assert(std::strcmp(event.getName(), "sys_close") != 0);
    assert(std::strcmp(event.getName(), "sys_open") != 0);

    std::cout << "##### OTHER EVENT #####" << std::endl;

    printEventDetails(event);

    return true;
}

}

extern "C" void onInit(CurrentState& state,
                       const TraceSet* traceSet,
                       DynamicLibraryStateProvider::StateProviderConfig& config)
{
    std::cout << "hello from linux.so: onInit()" << std::endl;

    if (config.registerEventCallback("lttng-kernel", "sched_switch", onSchedSwitch)) {
        std::cout << "successfully registered callback for lttng-kernel/sched_switch" << std::endl;
    } else {
        std::cout << "could not register callback for lttng-kernel/sched_switch" << std::endl;
    }

    if (config.registerEventCallback("lttng-kernel", "sys_open", onSysOpen)) {
        std::cout << "successfully registered callback for lttng-kernel/sys_open" << std::endl;
    } else {
        std::cout << "could not register callback for lttng-kernel/sys_open" << std::endl;
    }

    if (config.registerEventCallback("lttng-kernel", "sys_close", onSysClose)) {
        std::cout << "successfully registered callback for lttng-kernel/sys_close" << std::endl;
    } else {
        std::cout << "could not register callback for lttng-kernel/sys_close" << std::endl;
    }

    if (config.registerEventCallback("lttng-kernel", "meowmix", onSysClose)) {
        std::cout << "successfully registered callback for lttng-kernel/meowmix" << std::endl;
    } else {
        std::cout << "could not register callback for lttng-kernel/meowmix" << std::endl;
    }

    if (config.registerEventCallback("lttng-kernel", "", onEvent)) {
        std::cout << "successfully registered callback for lttng-kernel/*" << std::endl;
    } else {
        std::cout << "could not register callback for lttng-kernel/*" << std::endl;
    }

    const auto& tracesInfos = traceSet->getTracesInfos();

    std::cout << "traces infos:" << std::endl;

    for (const auto& traceInfos : tracesInfos) {
        std::cout << "trace ID:   " << traceInfos->getId() << std::endl;
        std::cout << "trace type: " << traceInfos->getTraceType() << std::endl;
        std::cout << "events:     " << traceInfos->getEventMap().size() << std::endl;
    }
}
