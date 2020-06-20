cmake_minimum_required(VERSION 3.16)
project(DAS)

#set(CMAKE_CXX_STANDARD 14)
if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++latest" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()

include_directories(.)

#configure_file(
#        index.html
#        rs.rc
#)

add_executable(DAS
        hid.cpp
        keyboard.cpp
        rs.rc
        main.cxx)

target_link_libraries(DAS PUBLIC
        Hid.lib
        Setupapi.lib
        User32.lib
        Ws2_32.lib)
