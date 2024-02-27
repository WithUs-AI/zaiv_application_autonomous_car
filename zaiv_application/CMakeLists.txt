cmake_minimum_required (VERSION 3.0)

project (zaiv-application)

option(USE_STANDALONE_ASIO "set ON to use standalone Asio instead of Boost.Asio" OFF)
option(BUILD_TESTING "set ON to build library tests" OFF)

if(NOT MSVC)
    add_compile_options(-std=c++17 -Wall -Wextra -Wsign-conversion -lstdc++fs -Wall -lpthread)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-Wthread-safety)
    endif()
else()
    add_compile_options(/W1)
endif()

add_library(zaiv-application INTERFACE)

target_include_directories(zaiv-application INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Threads REQUIRED)
target_link_libraries(zaiv-application INTERFACE ${CMAKE_THREAD_LIBS_INIT})

# TODO 2020 when Debian Jessie LTS ends:
# Remove Boost system, thread, regex components; use Boost::<component> aliases; remove Boost target_include_directories
if(USE_STANDALONE_ASIO)
    target_compile_definitions(zaiv-application INTERFACE USE_STANDALONE_ASIO)
    find_path(ASIO_PATH asio.hpp)
    if(NOT ASIO_PATH)
        message(FATAL_ERROR "Standalone Asio not found")
    else()
        target_include_directories(zaiv-application INTERFACE ${ASIO_PATH})
    endif()
else()
    find_package(Boost 1.54.0 COMPONENTS system thread REQUIRED)
    target_link_libraries(zaiv-application INTERFACE ${Boost_LIBRARIES})
    target_include_directories(zaiv-application INTERFACE ${Boost_INCLUDE_DIR})
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
        target_compile_definitions(zaiv-application INTERFACE USE_BOOST_REGEX)
        find_package(Boost 1.54.0 COMPONENTS regex REQUIRED)
        target_link_libraries(zaiv-application INTERFACE ${Boost_LIBRARIES})
        target_include_directories(zaiv-application INTERFACE ${Boost_INCLUDE_DIR})
    endif()
endif()
if(WIN32)
    target_link_libraries(zaiv-application INTERFACE ws2_32 wsock32)
endif()


# opencv
find_package( OpenCV REQUIRED )
target_include_directories(zaiv-application INTERFACE ${OpenCV_INCLUDE_DIRS})

# rapidjson
include("${CMAKE_SOURCE_DIR}/src/extlibs/rapidjson.cmake")
target_include_directories(zaiv-application INTERFACE ${RAPIDJSON_INCLUDE_DIR})


if(APPLE)
    set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
endif()
find_package(OpenSSL REQUIRED)
target_link_libraries(zaiv-application INTERFACE ${OPENSSL_LIBRARIES})
target_include_directories(zaiv-application INTERFACE ${OPENSSL_INCLUDE_DIR})

###############################################################################
# hailort
###############################################################################

find_library(
  LIBHAILORT
  NAMES libhailort.so
  HINTS /usr/lib /usr/lib64 /usr/local/lib
  REQUIRED)


target_link_libraries(zaiv-application INTERFACE ${LIBHAILORT})

# If zaiv-application is not a sub-project:
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    add_executable(zaiv_application
    src/main.cpp
    src/configuration.cpp
    src/postprocess/yolo/yolo_output.cpp
    src/postprocess/yolo/yolo_postprocess.cpp
    src/interfaces/json/JSONBase.cpp
    )
    add_dependencies(zaiv_application rapidjson)
    target_link_libraries(zaiv_application ${OpenCV_LIBS} )
    target_link_libraries(zaiv_application zaiv-application)

    include_directories(
        ${RAPIDJSON_INCLUDE_DIR}
    )
	
	#set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "-lhailort")
	#set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -lhailort)
	#set(CMAKE_EXE_LINKER_FLAGS "-lhailort")
	

    install(FILES
    src/configuration.h
    src/interfaces/websocket/asio_compatibility.hpp
    src/interfaces/json/JSONBase.h
    src/interfaces/json/JSONIncludes.h
    src/interfaces/websocket/server_ws.hpp
    src/interfaces/websocket/client_ws.hpp
    src/interfaces/websocket/server_wss.hpp
    src/interfaces/websocket/client_wss.hpp
    src/interfaces/websocket/crypto.hpp
    src/interfaces/websocket/utility.hpp
    src/interfaces/websocket/status_code.hpp
    src/interfaces/websocket/mutex.hpp
    DESTINATION include/zaiv-application)
endif()
