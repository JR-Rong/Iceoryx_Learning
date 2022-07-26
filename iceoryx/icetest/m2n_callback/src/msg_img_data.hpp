#ifndef MSG_IMAGE_DATA_TYPE_HPP
#define MSG_IMAGE_DATA_TYPE_HPP

#include "iceoryx_hoofs/cxx/vector.hpp"
#include "msg_img.hpp"

struct imgdata{

    iox::cxx::vector<uint8_t, 6400000> data1;
    
};

#endif