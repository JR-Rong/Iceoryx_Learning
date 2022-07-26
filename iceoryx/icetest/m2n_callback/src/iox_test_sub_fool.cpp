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
#include "iceoryx_posh/popo/listener.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"
#include "iceoryx_hoofs/cxx/string.hpp"

static int counter = 0;
static double cpu_avg = 0;
static double mem_avg = 0;
static int64_t timecost_avg = 0;

#include "iceoryx_hoofs/cxx/vector.hpp"



template <typename T = WaypointVec150>
void onSampleReceivedCallback(iox::popo::Subscriber<T>* subscriber){

    subscriber->take().and_then([&](auto& sample) {
        ++counter;
        std::cout << "counter is:" << counter << std::endl;
    });
}

template <typename T = WaypointVec150>
void onSampleReceived(std::string id, std::string topicn = "1")
{
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

    iox::popo::SubscriberOptions subscriberOptions;
    subscriberOptions.queueFullPolicy = iox::popo::QueueFullPolicy::DISCARD_OLDEST_DATA;
    subscriberOptions.queueCapacity = static_cast<uint64_t>(1);
    iox::popo::Subscriber<T> subscriber({"IceoryxTest", "one2one", topicname}, subscriberOptions);

    iox::popo::Listener listener;

    std::thread heartbeatThread([&] {
        while (!iox::posix::hasTerminationRequested())
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    listener
        .attachEvent(subscriber,
                     iox::popo::SubscriberEvent::DATA_RECEIVED,
                     iox::popo::createNotificationCallback(onSampleReceivedCallback<T>))
        .or_else([](auto) {
            std::cerr << "unable to attach subscriberLeft" << std::endl;
            std::exit(EXIT_FAILURE);
        });

    iox::posix::waitForTerminationRequest();

    listener.detachEvent(subscriber, iox::popo::SubscriberEvent::DATA_RECEIVED);
    heartbeatThread.join();

    cpu_avg = cpu_avg / counter;
    mem_avg = mem_avg / counter / 1024.0;
    timecost_avg = timecost_avg / counter;
    std::cout << "avg cpu load is[%]:" << std::fixed << std::setprecision(5) << cpu_avg 
        << " avg mem is[Kb]:" << std::fixed << std::setprecision(2) << mem_avg 
        << " and the avg send time is:" << timecost_avg << std::endl;
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