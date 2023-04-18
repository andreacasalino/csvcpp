file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(LIB_NAME csvpp)

if(LIB_OPT)
    add_library(${LIB_NAME} SHARED ${SRC_FILES})
else()
    add_library(${LIB_NAME} STATIC ${SRC_FILES})
endif()

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
