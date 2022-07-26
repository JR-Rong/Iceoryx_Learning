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
#include <chrono>
//utils
#include "top.h"
#include "GetTime.hpp"
#include "msg.hpp"
#include "msg_img.hpp"
//for iceoryx
#include "iceoryx_hoofs/posix_wrapper/signal_watcher.hpp"
#include "iceoryx_posh/popo/listener.hpp"
#include "iceoryx_posh/popo/subscriber.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"
#include "iceoryx_hoofs/cxx/string.hpp"

static int counter = 0;
static double cpu_avg_eql = 0;
static double mem_avg_eql = 0;
static double timecost_avg_eql = 0;
static double cpu_avg = 0;
static double mem_avg = 0;
static double timecost_avg = 0;
static int copytimes = 100;

#include "iceoryx_hoofs/cxx/vector.hpp"


template <typename T = WaypointVec150>
void onSampleReceivedCallback(iox::popo::Subscriber<T>* subscriber){
    std::unique_ptr<T> obj = std::make_unique<T>();
    // std::unique_ptr<T> obj;
    // T* obj;
    subscriber->take().and_then([&](auto& sample) {
        ++counter;
        std::cout << std::endl;
        auto nowtime = GetNs();
        
        auto deltatime = nowtime - sample->header.stamp.nsec;
        std::cout << "now time is:" << nowtime 
            << " and pub time is:" << sample->header.stamp.nsec 
            <<" sent and received cost time: " << deltatime 
            << " and counter is: " << counter << std::endl;

        double cpu_load = 0.0;
        unsigned int _vmrss = 0;
        auto start = std::chrono::steady_clock::now();
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff;

        std::cout << "sample->position[0].pose.position.x=" 
                << sample->position[0].pose.position.x << std::endl;

        start = std::chrono::steady_clock::now();
        for(int i=0;i<copytimes;i++){
            *obj = *sample;
        }
        end = std::chrono::steady_clock::now();
        diff = end - start;
        timecost_avg_eql += diff.count();
        obj->position[0].pose.position.x = 9.87654321;
        std::cout << "use '=' copy cost time:" << diff.count() * 1000
            << " sample.x is:"
            << sample->position[0].pose.position.x 
            << " and copy obj.x change to:" << obj->position[0].pose.position.x
            << std::endl;
        std::stringstream message1;
        cpu_load = get_cpu_load_mem(::getpid());
        _vmrss =get_proc_mem(::getpid());
        cpu_avg_eql += cpu_load;
        mem_avg_eql += _vmrss;
        message1 << "using '=' copy cost CPU load [%]: "
            << std::fixed << std::setprecision(5)
            << (std::isfinite(cpu_load) ? cpu_load : 0.0)
            << " VmRSS is "
            << std::fixed << std::setprecision(2)
            << (std::isfinite(_vmrss/1024.0) ? _vmrss/1024.0 : 0)
            << "[Kb]";
        std::cout << message1.str() << std::endl;

        start = std::chrono::steady_clock::now();
        for(int i=0;i<copytimes;i++){
            memcpy(obj.get(), &sample, sizeof(std::unique_ptr<T>));
        }
        end = std::chrono::steady_clock::now();
        diff = end - start;
        timecost_avg += diff.count();
        obj->position[0].pose.position.x = 1.23456789;
        std::cout << "use 'memcpy'copy cost time:" << diff.count() * 1000
            << " sample.x is:"
            << sample->position[0].pose.position.x 
            << " and copy obj.x change to:" << obj->position[0].pose.position.x
            << std::endl;
        std::stringstream message2;
        cpu_load = 0.0;
        cpu_load = get_cpu_load_mem(::getpid());
        _vmrss =get_proc_mem(::getpid());
        cpu_avg += cpu_load;
        mem_avg += _vmrss;
        message2 << "using 'memcpy' copy cost CPU load [%]: "
            << std::fixed << std::setprecision(5)
            << (std::isfinite(cpu_load) ? cpu_load : 0.0)
            << " VmRSS is "
            << std::fixed << std::setprecision(2)
            << (std::isfinite(_vmrss/1024.0) ? _vmrss/1024.0 : 0)
            << "[Kb]";
        std::cout << message2.str() << std::endl;
        obj.get_deleter();

        std::cout << std::endl;        

    })
    .or_else([](auto& result) {
        if (result != iox::popo::ChunkReceiveResult::NO_CHUNK_AVAILABLE)
        {
            std::cout << "Error receiving chunk." << std::endl;
        }
    });
}



template <typename T = WaypointVec150>
void onSampleReceived(std::string id, std::string topicn = "1")
{
    // initialize runtime
    iox::cxx::string<100U> APP_NAME = "iox-test-a2b-sub";

    iox::runtime::PoshRuntime::initRuntime(APP_NAME);

    std::cout << "type size is[MB]:" << sizeof(T)/1024.0/1024.0 << std::endl;

    iox::popo::SubscriberOptions subscriberOptions;
    subscriberOptions.queueFullPolicy = iox::popo::QueueFullPolicy::DISCARD_OLDEST_DATA;
    subscriberOptions.queueCapacity = static_cast<uint64_t>(1);
    iox::popo::Subscriber<T> subscriber({"IceoryxTest", "a2b", "topic"}, subscriberOptions);

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
    timecost_avg = timecost_avg * 1000 / counter;
    std::cout << "'memcpy': avg cpu load is[%]:" << std::fixed << std::setprecision(5) << cpu_avg 
        << " avg mem is[Kb]:" << std::fixed << std::setprecision(2) << mem_avg 
        << " avg copy time is[ms]:" << std::fixed << std::setprecision(5) << timecost_avg
        << std::endl;

    cpu_avg_eql = cpu_avg_eql / counter;
    mem_avg_eql = mem_avg_eql / counter / 1024.0;
    timecost_avg_eql = timecost_avg_eql * 1000 / counter;
    std::cout << "'=': avg cpu load is[%]:" << std::fixed << std::setprecision(5) << cpu_avg_eql 
        << " avg mem is[Kb]:" << std::fixed << std::setprecision(2) << mem_avg_eql 
        << " avg copy time is[ms]:" << std::fixed << std::setprecision(5) << timecost_avg_eql
        << std::endl;

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
        }
    }else if(argc == 4){
        if(argv[i] == use_waypoint150){
            onSampleReceived<WaypointVec150>(argv[i+1], argv[i+2]);
        }else if(argv[i] == use_waypoint1k){
            onSampleReceived<WaypointVec1k>(argv[i+1], argv[i+2]);
        }else if(argv[i] == use_waypoint2k){
            onSampleReceived<WaypointVec2k>(argv[i+1], argv[i+2]);
        }
    }
    
    return (EXIT_SUCCESS);
}