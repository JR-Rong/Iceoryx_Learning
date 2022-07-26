
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
    }
    WaypointVec2k(uint64_t curtime) noexcept
    {
        header.stamp.nsec = curtime;
    }
    Header header;
    iox::cxx::vector<Waypoint, 2000> position;
};

#endif