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

include_directories(
        deps/python/include
        deps
)

link_directories(
        deps/python
        )

link_libraries(
        Hid.lib
        Setupapi.lib
        User32.lib
        #        Comctl32.lib
        python38.lib
        Ws2_32.lib)


add_executable(DAS
        trayicon.cpp
        hid.cpp
        keyboard.cpp
        DAS4Q.cpp
        rest_api.cpp
        Config.cpp
        plugins.cpp
        rs.rc
        kb.ico
        main.ico
        main.cxx)




