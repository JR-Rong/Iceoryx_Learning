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
    dds_sequence_octet() noexcept
    {
        srand( (unsigned)time( NULL ));
        _maximum = static_cast<uint32_t>(rand());
        _length = static_cast<uint32_t>(rand());
        _release = true;
        imdt = imgdata();
    }
    uint32_t _maximum;
    uint32_t _length;
    imgdata imdt;
    bool _release;
}; 

struct ImageData
{
    ImageData(const uint64_t curtime) noexcept
    {
        header.stamp.nsec = curtime;
    }
    ImageData() noexcept
    {
        srand( (unsigned)time( NULL ));
        height = static_cast<uint32_t>(rand());
        width = static_cast<uint32_t>(rand());
        encoding = "abcd";
        is_bigendian = static_cast<uint8_t>(rand());
        step = static_cast<uint32_t>(rand());
        data = dds_sequence_octet();
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
