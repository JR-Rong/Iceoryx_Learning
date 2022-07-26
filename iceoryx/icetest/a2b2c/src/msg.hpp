
#ifndef MSG_WAYPOINT_TYPE_HPP
#define MSG_WAYPOINT_TYPE_HPP

#include <time.h>
#include <unistd.h>
#include "iceoryx_hoofs/cxx/vector.hpp"
#include "iceoryx_hoofs/cxx/string.hpp"
struct Time {
    uint64_t sec;
    uint64_t nsec;
};

struct Header {
    Time stamp;
    uint64_t pub_id;
    uint64_t frame_id;
};

struct Point { 
    double x; 
    double y; 
    double z; 
}; 

struct Pose { 
    double heading; 
    double curvature; 
    Point position;
    double speed;
    double acceleration;
    double jerk; 
}; 

struct Waypoint{ 
    Pose pose; 
}; 

struct WaypointVec150
{
    WaypointVec150() noexcept
    {
        srand( (unsigned)time( NULL ));
        position = iox::cxx::vector<Waypoint, 150>(static_cast<uint64_t>(150));
        for(int i=0;i<150;i++){
            position[i].pose.heading = static_cast<double>(rand());
            position[i].pose.curvature = static_cast<double>(rand());
            position[i].pose.speed = static_cast<double>(rand());
            position[i].pose.acceleration = static_cast<double>(rand());
            position[i].pose.jerk = static_cast<double>(rand());
            position[i].pose.position.x = static_cast<double>(rand());
            position[i].pose.position.y = static_cast<double>(rand());
            position[i].pose.position.z = static_cast<double>(rand());
        }
    }
    WaypointVec150(uint64_t curtime) noexcept
    {
        header.stamp.nsec = curtime;
    }
    Header header;
    iox::cxx::vector<Waypoint, 150> position;
};

struct WaypointVec1k
{
    WaypointVec1k() noexcept
    {
        srand( (unsigned)time( NULL ));
        position = iox::cxx::vector<Waypoint, 1000>(static_cast<uint64_t>(1000));
        for(int i=0;i<1000;i++){
            position[i].pose.heading = static_cast<double>(rand());
            position[i].pose.curvature = static_cast<double>(rand());
            position[i].pose.speed = static_cast<double>(rand());
            position[i].pose.acceleration = static_cast<double>(rand());
            position[i].pose.jerk = static_cast<double>(rand());
            position[i].pose.position.x = static_cast<double>(rand());
            position[i].pose.position.y = static_cast<double>(rand());
            position[i].pose.position.z = static_cast<double>(rand());
        }
    }
    WaypointVec1k(uint64_t curtime) noexcept
    {
        header.stamp.nsec = curtime;
    }
    Header header;
    iox::cxx::vector<Waypoint, 1000> position;
};

struct WaypointVec2k
{
    WaypointVec2k() noexcept
    {
        srand( (unsigned)time( NULL ));
        position = iox::cxx::vector<Waypoint, 2000>(static_cast<uint64_t>(2000));
        for(int i=0;i<2000;i++){
            position[i].pose.heading = static_cast<double>(rand());
            position[i].pose.curvature = static_cast<double>(rand());
            position[i].pose.speed = static_cast<double>(rand());
            position[i].pose.acceleration = static_cast<double>(rand());
            position[i].pose.jerk = static_cast<double>(rand());
            position[i].pose.position.x = static_cast<double>(rand());
            position[i].pose.position.y = static_cast<double>(rand());
            position[i].pose.position.z = static_cast<double>(rand());
        }
    }
    WaypointVec2k(uint64_t curtime) noexcept
    {
        header.stamp.nsec = curtime;
    }
    Header header;
    iox::cxx::vector<Waypoint, 2000> position;
};
#endif