cmake_minimum_required(VERSION 3.14)
project(file_signature)

set(CMAKE_CXX_STANDARD 17)

ADD_COMPILE_OPTIONS(-Wall -Wextra -pedantic)
ADD_COMPILE_OPTIONS("$<$<CONFIG:RELEASE>:-O3>")
ADD_COMPILE_OPTIONS("$<$<CONFIG:DEBUG>:-O0;-ggdb>")
ADD_COMPILE_OPTIONS("$<$<CONFIG:RELWITHDEBINFO>:-O2;-ggdb;-pg;-fno-omit-frame-pointer>")

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Build type is not specified, set to Release")
    set(CMAKE_BUILD_TYPE "Release")
else()
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

add_executable(file_signature src/main.cpp src/reader.cpp src/reader.hpp src/hasher.cpp src/hasher.hpp src/processor.hpp src/writer.cpp src/writer.hpp src/common/sync_queue.hpp src/common/semaphore.cpp src/common/semaphore.hpp src/data.hpp)
target_compile_options(file_signature PRIVATE -Werror)
target_compile_features(file_signature PRIVATE cxx_std_17)
target_link_libraries(file_signature pthread boost_program_options stdc++fs)

add_subdirectory(test)
