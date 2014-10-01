cmake_minimum_required(VERSION 2.6)

# print message
message("cmake test project")
message("root path =" ${CMAKE_SOURCE_DIR} )

# Project Name
set( PROJECT_NAME test )
project(${PROJECT_NAME})

# output path
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

# preprocessor define
if( MSVC )
	message("define MSVC")
	set( DEFINE ${DEFINE} -DMSVC 
	                      -DWIN32 )
elseif( XCODE )
	message("define xcode")
	set( DEFINE ${DEFINE} -DMACOS )
elseif( UNIX )
        message("define unix")
        set( DEFINE ${DEFINE} -DUNIX )
endif( MSVC )
add_definitions( ${DEFINE} )


# include path
set( INCLUDE_PATH ./ )
include_directories( ${INCLUDE_PATH} )

# library link 
#set(LIB_PATH /usr/local/lib/)
#link_directories( ${LIB_PATH} )

# source 
set( SOURCE main.cpp
                       CpuInformation.cpp )

# head
#set( HEAD ${CMAKE_SOURCE_DIR}/src/numeric/vector.h )

# exec file
add_executable( ${PROJECT_NAME} ${SOURCE}  )

# link libs
#set( LIB numeric_vector )
		 
#target_link_libraries( ${PROJECT_NAME} ${LIB} )   # 未來改成用迴圈方式
