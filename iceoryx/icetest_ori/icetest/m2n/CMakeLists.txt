# Copyright (c) 2021 - 2022 by Apex.AI Inc. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# SPDX-License-Identifier: Apache-2.0

# Build icehello example
cmake_minimum_required(VERSION 3.16)
project(icetext_m2n)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fpermissive -O3 -l c")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fpermissive -stdlib=libc++ -lsocket -O3 -l c")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fpermissive -O3 -l c")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fpermissive -stdlib=libc++ -O3 ")


include(GNUInstallDirs)

include_directories("/home/tusen/iceoryx/iceoryx/build/install/prefix/include/iceoryx/v2.90.0")
link_directories("/home/tusen/iceoryx/iceoryx/build/install/prefix/lib")

set(iceoryx_posh_DIR "/home/tusen/iceoryx/iceoryx/build/install/prefix/lib/cmake/iceoryx_posh/")
set(iceoryx_hoofs_DIR "/home/tusen/iceoryx/iceoryx/build/install/prefix/lib/cmake/iceoryx_hoofs/")

find_package(iceoryx_posh CONFIG REQUIRED)
find_package(iceoryx_hoofs CONFIG REQUIRED)

get_target_property(ICEORYX_CXX_STANDARD iceoryx_posh::iceoryx_posh CXX_STANDARD)

include(IceoryxPlatform)
include(IceoryxPackageHelper)

iox_add_executable(
    TARGET  iox-test-sub
    FILES   src/iox_test_sub.cpp src/top.c src/ttyin.c src/procfs_util.c 
    STACK_SIZE  100000000
    LIBS    iceoryx_posh::iceoryx_posh
)

iox_add_executable(
    TARGET  iox-test-pub
    FILES   src/iox_test_pub.cpp src/top.c src/ttyin.c src/procfs_util.c 
    STACK_SIZE  100000000
    LIBS    iceoryx_posh::iceoryx_posh
)