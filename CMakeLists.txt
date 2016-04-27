cmake_minimum_required(VERSION 2.6)

project(drill CXX C)

set (PROJECT_VERSION_MAJOR 1) 
set (PROJECT_VERSION_MINOR 0)

include(${PROJECT_SOURCE_DIR}/cmake/UserOption.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/CheckCompiler.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/Checkheaders.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/Configure.cmake)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR}) #for config.h

add_subdirectory(drill)


#########################################
# build test
###########################################################
#	add_subdirectory(lib/gtest-1.6.0)
#   it will cause cmake infinit loop by reset cache variables,
#   so we need manully make gtest before build this project
#   like this:
#   >cd lib/gtest-1.6.0
#   >cmake .
#   >make
##############################################################


if(test)

	include_directories(${PROJECT_SOURCE_DIR}/lib/gtest-1.6.0/include)
	link_directories(${PROJECT_SOURCE_DIR}/lib/gtest-1.6.0/)
	 ##############  
	 # Unit Tests  
	 ##############
	 #	enable_testing()
	 add_subdirectory(test/common)
	 add_subdirectory(test/http)
	 add_subdirectory(test/ext)
endif()

#########################################

include(${PROJECT_SOURCE_DIR}/cmake/Package.cmake)