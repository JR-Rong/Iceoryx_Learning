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
project(icetext_one2one)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fpermissive -l c")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fpermissive -stdlib=libc++")

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
    STACK_SIZE  500000000
    LIBS    iceoryx_posh::iceoryx_posh
)

iox_add_executable(
    TARGET  iox-test-pub
    FILES   src/iox_test_pub.cpp src/top.c src/ttyin.c src/procfs_util.c 
    STACK_SIZE  500000000
    LIBS    iceoryx_posh::iceoryx_posh
)

# add_executable(iox-test-pub src/iox_test_pub.cpp src/top.c src/ttyin.c src/procfs_util.c )
# target_link_libraries(iox-test-pub iceoryx_posh iceoryx_hoofs iceoryx_platform iceoryx_posh_roudi iceoryx_posh_config)
# target_include_directories(iox-test-pub PRIVATE ${IOX_INCLUDE_DIRECTORIES})
# target_link_libraries(iox-test-pub  ${IOX_LIBS})
# target_link_libraries(iox-test-pub ${IOX_LIBS_QNX})
# target_compile_options(iox-test-pub  PRIVATE ${IOX_WARNINGS} ${ICEORYX_SANITIZER_FLAGS})
# target_link_options(iox-test-pub BEFORE PRIVATE -Wl,-z,stack-size=${IOX_STACK_SIZE})
# set_target_properties(iox-test-pub PROPERTIES POSITION_INDEPENDENT_CODE ON )
# foreach(INTERFACE ${IOX_BUILD_INTERFACE})
#         target_include_directories(iox-test-pub
#             PUBLIC
#             $<BUILD_INTERFACE:${INTERFACE}>
#         )
#     endforeach()

#     foreach(INTERFACE ${IOX_INSTALL_INTERFACE})
#         target_include_directories(iox-test-pub
#             PUBLIC
#             $<INSTALL_INTERFACE:${INTERFACE}>
#         )
#     endforeach()

# add_executable(iox-test-sub src/iox_test_sub.cpp src/top.c src/ttyin.c src/procfs_util.c )
# target_link_libraries(iox-test-sub iceoryx_posh iceoryx_hoofs iceoryx_platform iceoryx_posh_roudi)
# target_include_directories(iox-test-sub PRIVATE ${IOX_INCLUDE_DIRECTORIES})
# target_link_libraries(iox-test-sub  ${IOX_LIBS})
# target_link_libraries(iox-test-sub ${IOX_LIBS_QNX})
# target_compile_options(iox-test-sub  PRIVATE ${IOX_WARNINGS} ${ICEORYX_SANITIZER_FLAGS})
# target_link_options(iox-test-sub BEFORE PRIVATE -Wl,-z,stack-size=100000000)
# set_target_properties(iox-test-sub PROPERTIES POSITION_INDEPENDENT_CODE ON )
# foreach(INTERFACE ${IOX_BUILD_INTERFACE})
#         target_include_directories(iox-test-sub
#             PUBLIC
#             $<BUILD_INTERFACE:${INTERFACE}>
#         )
#     endforeach()

#     foreach(INTERFACE ${IOX_INSTALL_INTERFACE})
#         target_include_directories(iox-test-sub
#             PUBLIC
#             $<INSTALL_INTERFACE:${INTERFACE}>
#         )
#     endforeach()

# # set_source_files_properties(./src/top.c ./src/ttyin.c ./src/procfs_util.c PROPERTIES LANGUAGE C)
# # set_target_properties(
# #     iox-test-pub
# #     PROPERTIES
# #     C_STANDARD_REQUIRED ON
# #     C_STANDARD 11
# # )

# set_source_files_properties(./src/top.c ./src/ttyin.c ./src/procfs_util.c PROPERTIES LANGUAGE C)
# set_target_properties(
#     iox-test-sub
#     PROPERTIES
#     C_STANDARD_REQUIRED ON
#     C_STANDARD 11
# )

# # set_source_files_properties(./src/iox_test_pub.cpp PROPERTIES LANGUAGE CXX)
# # set_target_properties(
# #     iox-test-pub
# #     PROPERTIES
# #     CXX_STANDARD_REQUIRED ON
# #     CXX_STANDARD ${ICEORYX_CXX_STANDARD}
# # )

# set_source_files_properties(./src/iox_test_sub.cpp PROPERTIES LANGUAGE CXX)
# set_target_properties(
#     iox-test-sub
#     PROPERTIES
#     CXX_STANDARD_REQUIRED ON
#     CXX_STANDARD ${ICEORYX_CXX_STANDARD}
# )