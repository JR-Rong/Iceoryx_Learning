#include <csignal>
#include <chrono>
#include <condition_variable>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include <numeric>
#include <cmath> // for isfinite
#include <atomic>
#include <stdlib.h>
#include <cassert>  
#include <cstring>
//utils
#include "top.h"
#include "GetTime.hpp"
#include "msg.hpp"
//for iceoryx
#include "iceoryx_hoofs/posix_wrapper/signal_watcher.hpp"
#include "iceoryx_posh/popo/listener.hpp"
#include "iceoryx_posh/popo/subscriber.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"
#include "iceoryx_posh/popo/wait_set.hpp"
#include "iceoryx_posh/popo/user_trigger.hpp"
#include "iceoryx_hoofs/cxx/vector.hpp"

constexpr char APP_NAME[] = "iox-test-1to1-sub";
static int flag = 0;

std::atomic_bool keepRunning{true};

int main()
{
    iox::runtime::PoshRuntime::initRuntime(APP_NAME);
    iox::popo::WaitSet<> waitset;
    
    // register signal handler to handle termination of the loop
    iox::popo::Subscriber<RadarObject> subscriber({"IceoryxTest", "one2one", "topic"});
    
    waitset.attachState(subscriber, iox::popo::SubscriberState::HAS_DATA).or_else([](auto) {
        std::cerr << "failed to attach subscriber" << std::endl;
        std::exit(EXIT_FAILURE);
    });

    while (!iox::posix::hasTerminationRequested())
    {
        // We block and wait for samples to arrive.
        auto notificationVector = waitset.wait();

        // for (auto& notification : notificationVector)
        // {
        //     if (notification->doesOriginateFrom(&subscriber))
        //     {
        //         // Consume a sample
        //         subscriber.take().and_then([](auto& sample) {
        //         auto nowtime = GetNs();
        //         auto deltatime = nowtime - sample->header.stamp.nsec;
        //         std::cout << "now time is:" << nowtime << " and pub time is:" << sample->header.stamp.nsec <<" sent and received cost time: " << deltatime << std::endl;
        //         })
        //         .or_else([](auto& result) {
        //             // only has to be called if the alternative is of interest,
        //             // i.e. if nothing has to happen when no data is received and
        //             // a possible error alternative is not checked or_else is not needed
        //             if (result != iox::popo::ChunkReceiveResult::NO_CHUNK_AVAILABLE)
        //             {
        //                 std::cout << "Error receiving chunk." << std::endl;
        //             }
        //         });

        //         std::stringstream message;
        //         double cpu_load = 0.0;
        //         cpu_load = get_cpu_load_mem(::getpid());
        //         unsigned int _vmrss =get_proc_mem(::getpid());
        //         message << "APP:" << APP_NAME << " recevied data. CPU load [%]: "
        //             << std::fixed << std::setprecision(5)
        //             << (std::isfinite(cpu_load) ? cpu_load : 0.0)
        //             << " VmRSS is "
        //             << std::fixed << std::setprecision(2)
        //             << (std::isfinite(_vmrss/1024.0) ? _vmrss/1024.0 : 0)
        //             << "[Kb]";
        //         std::cout << message.str() << std::endl;

        //             // We could consume all samples but do not need to.
        //             // If there is more than one sample we will wake up again since the state of the subscriber is still
        //             // iox::popo::SubscriberState::HAS_DATA in this case.
        //     }
        // }
    }

    std::cout << "shutting down" << std::endl;

    return (EXIT_SUCCESS);
}