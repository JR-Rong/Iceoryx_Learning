#ifndef MSG_IMAGE_TYPE_HPP
#define MSG_IMAGE_TYPE_HPP

#include <time.h>
#include <unistd.h>
#include "iceoryx_hoofs/cxx/vector.hpp"
#include "iceoryx_hoofs/cxx/string.hpp"
#include "msg.hpp"
#include "msg_img_data.hpp"

struct dds_sequence_octet
{
    uint32_t _maximum;
    uint32_t _length;
    // iox::cxx::vector<iox::cxx::vector<iox::cxx::vector<uint8_t, 163> , 5> , 8> _buffer;
    // iox::cxx::vector<iox::cxx::vector<uint8_t, 163> , 2> _buffer;
    // iox::cxx::vector<iox::cxx::vector<uint8_t, 1024> , 380> _buffer1;
    imgdata imdt;
    bool _release;
}; 

struct ImageData
{
    ImageData(const uint64_t curtime) noexcept
    {
        header.stamp.nsec = curtime;
    }
    Header header;
    uint32_t height;
    uint32_t width;
    iox::cxx::string<20> encoding;
    uint8_t is_bigendian;
    uint32_t step;
    dds_sequence_octet data;
};

#endif