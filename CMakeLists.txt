cmake_minimum_required(VERSION 3.8)
project("CppServer" VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CMakeDependentOption)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "possible configurations" FORCE)
# In case the user does not setup CMAKE_BUILD_TYPE, assume it's RelWithDebInfo
# In c++ code you can write: #ifdef EDITOR
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "build type default to Debug" FORCE)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions("-DEDITOR")
    add_definitions("-DUSE_WITH_EDITOR")
    add_definitions("-g")
	list(APPEND CMAKE_CXX_FLAGS "-g")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions("-D_DEBUG")
endif()

#set(CMAKE_CXX_FLAGS_DEBUGEDITOR "${CMAKE_CXX_FLAGS_DEBUG} -g ")  # 调试信息
list(APPEND CMAKE_CXX_FLAGS_DEBUGEDITOR "-g")

message(${CMAKE_CXX_FLAGS})


SET(ASIO_INCLUDE 
    "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/asio_1_30_2/include"
)

SET(Common_INCLUDE
    "${CMAKE_CURRENT_SOURCE_DIR}/NetCommonLib"
)

SET(
    BINARY_ROOT_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
)


# [生成目录]
##禁止使用EXECUTABLE_OUTPUT_PATH、LIBRARY_OUTPUT_PATH的值，请使用Project_OUTPUT_DIR（因为VS环境与非VS环境不一样）
message(STATUS "[Project output directory]: ${BINARY_ROOT_DIR}")

if(${CMAKE_GENERATOR} MATCHES "Visual Studio*")
    #在Visual Studio生成器（即VS工程）下，会在EXECUTABLE_OUTPUT_PATH、EXECUTABLE_OUTPUT_PATH后面自动加一个${CMAKE_BUILD_TYPE}
    SET(EXECUTABLE_OUTPUT_PATH ${BINARY_ROOT_DIR}) #可执行文件输出目录
    set(ARCHIVE_OUTPUT_DIRECTORY ${BINARY_ROOT_DIR})     #静态库文件输出目录
    set(LIBRARY_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH})    #库文件输出目录
else()
    #在其他环境下，不会动，原来是什么就是什么
    SET(EXECUTABLE_OUTPUT_PATH ${BINARY_ROOT_DIR}${CMAKE_BUILD_TYPE})
    set(ARCHIVE_OUTPUT_DIRECTORY ${BINARY_ROOT_DIR}${CMAKE_BUILD_TYPE})
    set(LIBRARY_OUTPUT_PATH ${BINARY_ROOT_DIR}${CMAKE_BUILD_TYPE})
endif()


include_directories (${ASIO_INCLUDE})


add_subdirectory(NetCommonLib)
add_subdirectory(NetServer)
add_subdirectory(NetClient)