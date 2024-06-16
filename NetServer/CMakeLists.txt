cmake_minimum_required(VERSION 3.8)
project("NetServer" LANGUAGES CXX)

add_definitions("-DUNICODE" "-D_UNICODE")

add_executable(NetServer WIN32
        NetServer.cpp
)

set ( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS" ) 
target_link_libraries(NetServer EngineLib EngineDLL)
#target_link_libraries(NetServer EngineDLL )

list(APPEND CMAKE_CXX_FLAGS_DEBUG /MTd)

#if(MSVC)
#    target_link_options(NetServer PUBLIC /DEBUG)
#endif()

if(MSVC)
    set_property(TARGET NetServer APPEND PROPERTY LINK_FLAGS_DEBUG /DEBUG)
	  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUGEDITOR} /ZI /MP")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUGEDITOR} /MDd  /LIBPATH:\"${ATLDIR}\"")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUGGAME} /MDd")
endif()

target_link_directories(NetServer PRIVATE ${ATLDIR})
