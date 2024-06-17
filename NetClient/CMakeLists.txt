cmake_minimum_required(VERSION 3.8)
project("NetClient" LANGUAGES CXX)

add_definitions("-DUNICODE" "-D_UNICODE")

add_executable(NetClient WIN32
        NetClient.cpp
)

# 检测平台
if (WIN32)
    # 设置链接器标志以使用 Windows 子系统
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
elseif (UNIX)
    # Linux 平台下的链接器标志（默认情况不需要特别设置）
endif()
#target_link_libraries(NetClient EngineDLL )



#if(MSVC)
#    target_link_options(NetClient PUBLIC /DEBUG)
#endif()

include_directories (${Common_INCLUDE})

if(MSVC)
    list(APPEND CMAKE_CXX_FLAGS_DEBUG /MTd)
    set_property(TARGET NetClient APPEND PROPERTY LINK_FLAGS_DEBUG /DEBUG)
	  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUGEDITOR} /ZI /MP")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUGEDITOR} /MDd  /LIBPATH:\"${ATLDIR}\"")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUGGAME} /MDd")
endif()

target_link_directories(NetClient PRIVATE 
    ${ATLDIR}
)

if (UNIX)
target_link_libraries(NetClient pthread)
endif()
