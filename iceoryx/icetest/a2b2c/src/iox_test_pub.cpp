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
#include "iceoryx_posh/popo/publisher.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"
#include "iceoryx_hoofs/cxx/string.hpp"


static bool flag = true;

template <typename T = WaypointVec150>
void sending(std::string id, std::string topicn = "1")
{
     double cpu_avg = 0.0;
    double mem_avg = 0.0;
    int maxtimes = 1800;
    iox::cxx::string<100U> APP_NAME = "iox-test-a2b-pub";

    iox::runtime::PoshRuntime::initRuntime(APP_NAME);

    iox::popo::PublisherOptions publisherOptions;
    publisherOptions.subscriberTooSlowPolicy = iox::popo::ConsumerTooSlowPolicy::DISCARD_OLDEST_DATA;

    iox::popo::Publisher<T> myPublisher({"IceoryxTest", "a2b", "topic"}, publisherOptions);

    for (int32_t counter = 0; counter<maxtimes; ++counter)
    {

        myPublisher.loan().and_then([&](auto & sample) { 
           GetNs(*sample); // here the image data is written
           std::cout << "Sending waypoints with timer " 
                << sample->header.stamp.nsec 
                << " and data size is:" << sizeof(T) 
                << " and counter is:" << counter << std::endl;
           sample.publish(); });

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
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void sending_img(std::string id, std::string topicn = "1")
{
     double cpu_avg = 0.0;
    double mem_avg = 0.0;
    int maxtimes = 1800;
    iox::cxx::string<100U> APP_NAME = "iox-test-a2b-pub";

    iox::runtime::PoshRuntime::initRuntime(APP_NAME);

    iox::popo::PublisherOptions publisherOptions;
    publisherOptions.subscriberTooSlowPolicy = iox::popo::ConsumerTooSlowPolicy::DISCARD_OLDEST_DATA;

    iox::popo::Publisher<ImageData> myPublisher({"IceoryxTest", "a2b", "topic"}, publisherOptions);

    for (int32_t counter = 0; counter<maxtimes; ++counter)
    {

        myPublisher.loan().and_then([&](auto & sample) { 
            GetNs(*sample); // here the image data is written
            srand( (unsigned)time( NULL ));
            for(int i=0;i<6400000; i++){
                sample->data.imdt.data1.emplace_back(static_cast<uint8_t>(rand()));
            }
            std::cout << "Sending waypoints with timer " 
                    << sample->header.stamp.nsec 
                    << " and data size is:" << sizeof(ImageData) 
                    << " and counter is:" << counter << std::endl;
            sample.publish(); });

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
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main(int argc, char **argv)
{
    std::string use_waypoint150("-wp150");
    std::string use_waypoint1k("-wp1k");
    std::string use_waypoint2k("-wp2k");
    std::string use_image("-im");
    std::cout << "Pub started" <<  std::endl;
    int i = 1;
    if(argc == 1){
        sending<>("1");
    }else if(argc == 3){
        if(argv[i] == use_waypoint150){
            sending<WaypointVec150>(argv[i+1]);
        }else if(argv[i] == use_waypoint1k){
            sending<WaypointVec1k>(argv[i+1]);
        }else if(argv[i] == use_waypoint2k){
            sending<WaypointVec2k>(argv[i+1]);
        }else if(argv[i] == use_image){
            sending_img(argv[i+1]);
        }
        
    }else if(argc == 4){
        if(argv[i] == use_waypoint150){
            sending<WaypointVec150>(argv[i+1], argv[i+2]);
        }else if(argv[i] == use_waypoint1k){
            sending<WaypointVec1k>(argv[i+1], argv[i+2]);
        }else if(argv[i] == use_waypoint2k){
            sending<WaypointVec2k>(argv[i+1], argv[i+2]);
        }else if(argv[i] == use_image){
            sending_img(argv[i+1], argv[i+2]);
        }
    }

    return (EXIT_SUCCESS);
}