cmake_minimum_required(VERSION 2.6)

project(drill)

set (PROJECT_VERSION_MAJOR 1) 
set (PROJECT_VERSION_MINOR 0)

include(${PROJECT_SOURCE_DIR}/cmake/UserOption.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/CheckCompiler.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/Checkheaders.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/Configure.cmake)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR}) #for config.h

add_subdirectory(drill)

#add_subdirectory(walle/unit)
#add_subdirectory(walle/sys)
#add_subdirectory(walle/net)

#########################################
# build test
if(test)
#	add_subdirectory(lib/gtest-1.6.0)
#	enable_testing()
# Include the gtest library. gtest_SOURCE_DIR is available due to
# 'project(gtest)' above.  

	include_directories(${PROJECT_SOURCE_DIR}/lib/gtest-1.6.0/include)
	link_directories(${PROJECT_SOURCE_DIR}/lib/gtest-1.6.0/)
	 ##############  
	 # Unit Tests  
	 ##############
	 add_subdirectory(test/common)
endif()

#########################################

include(${PROJECT_SOURCE_DIR}/cmake/Package.cmake)