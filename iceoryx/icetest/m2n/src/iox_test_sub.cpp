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
#include "msg_img.hpp"
//for iceoryx
#include "iceoryx_hoofs/posix_wrapper/signal_watcher.hpp"
#include "iceoryx_posh/popo/subscriber.hpp"
#include "iceoryx_posh/popo/untyped_subscriber.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"
#include "iceoryx_hoofs/cxx/string.hpp"

static int flag = 0;

#include "iceoryx_hoofs/cxx/vector.hpp"

//! [topic data]

template <typename T = WaypointVec150>
void onSampleReceived(std::string id, std::string topicn = "1")
{
    double cpu_avg = 0.0;
    double mem_avg = 0.0;
    uint64_t counter = 0;
    // initialize runtime
    iox::cxx::string<100U> APP_NAME = "iox-test-m2n-sub-";
    for(auto ch:id){
        APP_NAME.unsafe_append(ch);
    }
    iox::runtime::PoshRuntime::initRuntime(APP_NAME);

    std::cout << "type size is[MB]:" << sizeof(T)/1024.0/1024.0 << std::endl;

    //! [create subscriber]
    iox::cxx::string<15U> topicname = "topic-";
    for(auto ch:topicn){
        topicname.unsafe_append(ch);
    }
    iox::popo::Subscriber<T> subscriber({"IceoryxTest", "one2one", topicname});
    // iox::popo::UntypedSubscriber subscriber({"IceoryxTest", "m2n", topicname});
    //! [create subscriber]
    while (!iox::posix::hasTerminationRequested())
    {
        subscriber.take().and_then([&](auto& sample) {
            ++counter;
            auto nowtime = GetNs();
            auto deltatime = nowtime - sample->header.stamp.nsec;
            std::cout << "now time is:" << nowtime 
            << " and pub time is:" << sample->header.stamp.nsec 
            <<" sent and received cost time: " << deltatime 
            << " and counter is: " << counter << std::endl;
            std::stringstream message;
            double cpu_load = 0.0;
            cpu_load = get_cpu_load_mem(::getpid());
            unsigned int _vmrss =get_proc_mem(::getpid());
            cpu_avg += cpu_load;
            mem_avg += _vmrss;
            message << "APP:" << APP_NAME << " recevied data. CPU load [%]: "
                << std::fixed << std::setprecision(5)
                << (std::isfinite(cpu_load) ? cpu_load : 0.0)
                << " VmRSS is "
                << std::fixed << std::setprecision(2)
                << (std::isfinite(_vmrss/1024.0) ? _vmrss/1024.0 : 0)
                << "[Kb]";
            std::cout << message.str() << std::endl;
        });
        
        std::this_thread::sleep_for(std::chrono::milliseconds(2));

        // subscriber
        //     .take()
        //     //! [chunk happy path]
        //     .and_then([&](const void* userPayload) {
        //         //! [chunk received]
        //         auto sample = static_cast<const T*>(userPayload);
        //         ++counter;
        //         auto nowtime = GetNs();
        //         auto deltatime = nowtime - sample->header.stamp.nsec;
        //         std::cout << "now time is:" << nowtime 
        //         << " and pub time is:" << sample->header.stamp.nsec 
        //         <<" sent and received cost time: " << deltatime 
        //         << " and counter is: " << counter << std::endl;
        //         //! [chunk received]

        //         //! [release]
        //         // note that we explicitly have to release the sample
        //         // and afterwards the pointer access is undefined behavior
        //         // if(counter % 4 == 0) 
        //         subscriber.release(userPayload);
            
        //         std::stringstream message;
        //         double cpu_load = 0.0;
        //         cpu_load = get_cpu_load_mem(::getpid());
        //         unsigned int _vmrss =get_proc_mem(::getpid());
        //         cpu_avg += cpu_load;
        //         mem_avg += _vmrss;
        //         message << "APP:" << APP_NAME << " recevied data. CPU load [%]: "
        //             << std::fixed << std::setprecision(5)
        //             << (std::isfinite(cpu_load) ? cpu_load : 0.0)
        //             << " VmRSS is "
        //             << std::fixed << std::setprecision(2)
        //             << (std::isfinite(_vmrss/1024.0) ? _vmrss/1024.0 : 0)
        //             << "[Kb]";
        //         std::cout << message.str() << std::endl;
                
        //     })
        //     .or_else([](auto& result) {
        //         // only has to be called if the alternative is of interest,
        //         // i.e. if nothing has to happen when no data is received and
        //         // a possible error alternative is not checked or_else is not needed
        //         if (result != iox::popo::ChunkReceiveResult::NO_CHUNK_AVAILABLE)
        //         {
        //             // std::cout << "Error receiving chunk." << std::endl;
        //         }
        //     });
        // std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    cpu_avg = cpu_avg / counter;
    mem_avg = mem_avg / counter / 1024.0;
    std::cout << "avg cpu load is[%]:" << std::fixed << std::setprecision(5) << cpu_avg 
        << " avg mem is[Kb]:" << std::fixed << std::setprecision(2) << mem_avg << std::endl;
}

int main(int argc, char **argv)
{
    std::string use_waypoint150("-wp150");
    std::string use_waypoint1k("-wp1k");
    std::string use_waypoint2k("-wp2k");
    std::string use_image("-im");
    
    int i = 1;
    if(argc == 1){
        onSampleReceived<>("1");
    }else if(argc == 3){
        if(argv[i] == use_waypoint150){
            onSampleReceived<WaypointVec150>(argv[i+1]);
        }else if(argv[i] == use_waypoint1k){
            onSampleReceived<WaypointVec1k>(argv[i+1]);
        }else if(argv[i] == use_waypoint2k){
            onSampleReceived<WaypointVec2k>(argv[i+1]);
        }else if(argv[i] == use_image){
            onSampleReceived<ImageData>(argv[i+1]);
        }
    }else if(argc == 4){
        if(argv[i] == use_waypoint150){
            onSampleReceived<WaypointVec150>(argv[i+1], argv[i+2]);
        }else if(argv[i] == use_waypoint1k){
            onSampleReceived<WaypointVec1k>(argv[i+1], argv[i+2]);
        }else if(argv[i] == use_waypoint2k){
            onSampleReceived<WaypointVec2k>(argv[i+1], argv[i+2]);
        }else if(argv[i] == use_image){
            onSampleReceived<ImageData>(argv[i+1], argv[i+2]);
        }
    }
    
    return (EXIT_SUCCESS);
}