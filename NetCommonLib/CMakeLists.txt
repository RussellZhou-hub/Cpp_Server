cmake_minimum_required(VERSION 3.8)
project("NetCommonLib" LANGUAGES CXX)


add_definitions("-DUNICODE" "-D_UNICODE")


add_library(NetCommonLib STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/framework.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MMO_Common.h
	${CMAKE_CURRENT_SOURCE_DIR}/net_client.h		
	${CMAKE_CURRENT_SOURCE_DIR}/net_common.h
	${CMAKE_CURRENT_SOURCE_DIR}/net_connection.h
	${CMAKE_CURRENT_SOURCE_DIR}/net_message.h
	${CMAKE_CURRENT_SOURCE_DIR}/net_server.h
	${CMAKE_CURRENT_SOURCE_DIR}/net_tsqueue.h
	${CMAKE_CURRENT_SOURCE_DIR}/NetCommon.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/olc_net.h
)

SET_TARGET_PROPERTIES(NetCommonLib
        PROPERTIES
#        CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BINARY_ROOT_DIR}/lib"   #static library output
#        CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BINARY_ROOT_DIR}/DLL"   #dynamic library output
#        CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BINARY_ROOT_DIR}"
        OUTPUT_NAME "NetCommonLib"
        CLEAN_DIRECT_OUTPUT 1
)

target_link_directories(NetCommonLib PRIVATE 
	${ATLDIR}
	)

if(MSVC)
    set_property(TARGET NetCommonLib APPEND PROPERTY LINK_FLAGS_DEBUG /DEBUG)
	set_property(TARGET NetCommonLib APPEND PROPERTY LINK_FLAGS_DEBUG /SUBSYSTEM:WINDOWS)
	set_property(TARGET NetCommonLib APPEND PROPERTY LINK_FLAGS_DEBUG /DEBUG)
	set_property(TARGET NetCommonLib APPEND PROPERTY LINK_FLAGS_DEBUG /SUBSYSTEM:WINDOWS)
	set (CMAKE_CXX_FLAGS_DEBUGE "${CMAKE_CXX_FLAGS_DEBUGEDITOR} /Zi /MDd /Od /MP /LIBPATH:\"${ATLDIR}\"")
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUGGAME} /Zi /MP")
endif()

target_include_directories(NetCommonLib PRIVATE 
	${ATL_INCLUDE}
)

