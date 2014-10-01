# // 程式：dlcall.c , 編譯指令：gcc -o dl_call dl_call.c –ldl
# http://ccckmit.wikidot.com/lk:dynamiclinking
# https://gitorious.org/cmoduler/cmoduler/source/10dd22948e1aa3fe67a46e41058516707c711508:example/FindSqlite3.cmake


cmake_minimum_required(VERSION 2.6)

# sub project
#include(${CMAKE_SOURCE_DIR}/cmake/numeric_vector.cmake)


# print message
message("cmake sqlite_test project")
message("root path =" ${CMAKE_SOURCE_DIR} )

# Project Name
set( PROJECT_NAME sqlite_test )
project(${PROJECT_NAME})

# output path
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

# preprocessor define
if( UNIX )
        message("define unix")
        set( DEFINE ${DEFINE} -DUNIX )
endif( UNIX )
add_definitions( ${DEFINE} )

# include path
set( INCLUDE_PATH /usr/local/include/
                                   ../sqlite/
                                   ../src/ )
include_directories( ${INCLUDE_PATH} )

# library link 
set(LIB_PATH /usr/local/lib/)
link_directories( ${LIB_PATH} )

# source 
set( SOURCE main.cpp 
                        ../sqlite/sqlite3.c
                        ../src/SQLiteControl.cpp )

# head
#set( HEAD ${CMAKE_SOURCE_DIR}/src/numeric/vector.h )

# exec file
add_executable( ${PROJECT_NAME} ${SOURCE}  )

# add lib files.
set( LIBS dl 
                 pthread )

target_link_libraries( ${PROJECT_NAME} ${LIBS} )