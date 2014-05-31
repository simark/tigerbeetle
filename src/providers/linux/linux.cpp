#include <iostream>

#include <common/state/CurrentState.hpp>
#include <common/trace/Event.hpp>

extern "C" {

void onInit(tibee::common::CurrentState& state)
{
    std::cout << "from linux.so: onInit()" << std::endl;
}

void onEvent(tibee::common::CurrentState& state, const tibee::common::Event& event)
{
    auto fields = event.getFields();

    if (fields) {
        std::cout << fields->toString() << std::endl;
    }
}

void onFini(tibee::common::CurrentState& state)
{
    std::cout << "from linux.so: onFini()" << std::endl;
}

}
