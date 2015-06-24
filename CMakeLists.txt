cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(srnp_wrapper)

include_directories(include)

add_library(srnp_wrapper SHARED src/srnp_wrapper.cpp)
target_link_libraries(srnp_wrapper srnp_kernel srnp_meta_pair_callback)


###################################################
### Test a program with the original peiskernel ###
###################################################

add_executable (test_peisk src/simple_tests/simple_publisher.c)
target_link_libraries (test_peisk peiskernel_mt peiskernel pthread)

#####################################
### Test it now with the new peis ###
#####################################

add_executable (test_srnpk src/simple_tests/simple_publisher_srnp.c)
target_link_libraries (test_srnpk srnp_wrapper)

add_executable (test_callback src/simple_tests/simple_callback.c)
target_link_libraries (test_callback srnp_wrapper pthread)

add_executable (test_meta_callback src/simple_tests/simple_meta_callback.c)
target_link_libraries (test_meta_callback srnp_wrapper pthread)

install (DIRECTORY include/srnp DESTINATION "include")
install (TARGETS srnp_wrapper DESTINATION "lib")

########### Add uninstall target ###############

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake") 
