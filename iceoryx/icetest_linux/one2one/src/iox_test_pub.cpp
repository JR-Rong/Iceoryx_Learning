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
#include "GetTime.hpp"
#include "msg.hpp"
#include "msg_img.hpp"
//for iceoryx
#include "iceoryx_hoofs/posix_wrapper/signal_watcher.hpp"
#include "iceoryx_posh/popo/publisher.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"

constexpr char APP_NAME[] = "iox-test-1to1-pub";
static bool flag = true;

template <typename T = WaypointVec150>
void sending()
{
    std::cout << "Pub started BBBBBBBBBBBBBBBBBBB" <<  std::endl;
    double cpu_avg = 0.0;
    double mem_avg = 0.0;

    std::cout << "Pub started CCCCCCCCCCCCCCCCCCC" <<  std::endl;

    iox::runtime::PoshRuntime::initRuntime(APP_NAME);
    std::cout << "Pub started DDDDDDDDDDDDDDDDDDD" <<  std::endl;
    iox::popo::Publisher<T> myPublisher({"IceoryxTest", "one2one", "topic"});
    std::cout << "Pub started EEEEEEEEEEEEEEEEEEEE" <<  std::endl;

    for (int32_t counter = 0; counter<1800; ++counter)
    {
        T obj = T(static_cast<uint64_t>(GetNs()));
        myPublisher.publishCopyOf(obj)
            .or_else([&](auto) { std::cout << "Failed sending: " << std::endl; });
        std::cout << "Sending waypoints with timer " 
        << obj.header.stamp.nsec 
        << " and data size is:" << sizeof(T) 
        << " and counter is:" << counter << std::endl;
         std::this_thread::sleep_for(std::chrono::milliseconds(100));
       
    }
    flag = false;
    
}

void sendingimg()
{
    std::cout << "Pub started BBBBBBBBBBBBBBBBBBB" <<  std::endl;
    double cpu_avg = 0.0;
    double mem_avg = 0.0;

    std::cout << "Pub started CCCCCCCCCCCCCCCCCCC" <<  std::endl;

    iox::runtime::PoshRuntime::initRuntime(APP_NAME);
    std::cout << "Pub started DDDDDDDDDDDDDDDDDDD" <<  std::endl;
    iox::popo::Publisher<ImageData> myPublisher({"IceoryxTest", "one2one", "topic"});
    std::cout << "Pub started EEEEEEEEEEEEEEEEEEEE" <<  std::endl;

    for (int32_t counter = 0; !iox::posix::hasTerminationRequested(); ++counter)
    {
        ImageData obj = ImageData(static_cast<uint64_t>(GetNs()));
        myPublisher.publishCopyOf(obj)
            .or_else([&](auto) { std::cout << "Failed sending: " << std::endl; });
        std::cout << "Sending waypoints with timer " 
        << obj.header.stamp.nsec 
        << " and data size is:" << sizeof(ImageData) 
        << " and counter is:" << counter << std::endl;

        
    }
    flag = false;

}

int main(int argc, char **argv)
{
    std::string use_waypoint150("-wp150");
    std::string use_waypoint1k("-wp1k");
    std::string use_waypoint2k("-wp2k");
    std::string use_image("-im");
    std::cout << "Img-vec size is[byte]:" << sizeof(ImageData) << std::endl;
    std::cout << "Pub started" <<  std::endl;
    int i = 1;
    if(argc == 1){
        sending<>();
    }
    while(i < argc){
        if(argv[i] == use_waypoint150){
            sending<WaypointVec150>();
        }else if(argv[i] == use_waypoint1k){
            sending<WaypointVec1k>();
        }else if(argv[i] == use_waypoint2k){
            sending<WaypointVec2k>();
        }else if(argv[i] == use_image){
            sendingimg();
        }  
        i++;
    }

    return (EXIT_SUCCESS);
}