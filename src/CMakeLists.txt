##
# @author Vitalii Chernookyi
#                           
# @copyright BSD

add_library(object-pool STATIC
	object-pool.cc)
set_target_properties(object-pool PROPERTIES
	PREFIX ""
	CXX_STANDARD 14
	COMPILE_OPTIONS "-O2;-pthread;-Wall;-Wextra;-std=gnu++14"
	LINK_FLAGS "-O2 -pthread")

target_include_directories(object-pool PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/..)

################################
# GTest
################################
#include(GoogleTest)
enable_testing()
include_directories(${gtest_SOURCE_DIR})

################################
# Unit Tests
################################
# Add test cpp file
add_executable( object-pool_tests object-pool_tests.cc)
target_link_libraries(object-pool_tests gtest object-pool rt)
target_include_directories(object-pool_tests PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/..)

set_target_properties(object-pool_tests
	PROPERTIES
	CXX_STANDARD 14
	COMPILE_OPTIONS "-g;-O2;-pthread;-Wall;-Wextra;-std=gnu++14"
	LINK_FLAGS "-g -O2 -pthread -uRound")

execute_process(COMMAND date "+%F_%T"
	OUTPUT_VARIABLE VALGRIND_TESTS_TIMESTAMP
	OUTPUT_STRIP_TRAILING_WHITESPACE)

#gtest_discover_tests(object-pool_tests)

add_test( NAME runUnitTests
	COMMAND valgrind
	"--log-file=valgrind/object-pool_tests.${VALGRIND_TESTS_TIMESTAMP}.valgrind"
	-v
	--read-var-info=yes
	--track-origins=yes
	--show-reachable=yes
	--leak-check=full
	--show-leak-kinds=all
		$<TARGET_FILE:object-pool_tests> )
