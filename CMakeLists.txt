cmake_minimum_required(VERSION 3.8)
project(sqrt_ekf)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED)

add_executable(test_thread src/test/test_thread.cpp src/util/thread_util.h src/util/gaussian.hpp)
add_executable(test_gaussian src/test/test_gaussian.cpp src/util/thread_util.h src/util/gaussian.hpp) # src/util/unscented_transform.hpp

set(EXE_LIST test_thread test_gaussian)

foreach(EXE_VAR ${EXE_LIST})

  target_include_directories(${EXE_VAR} PRIVATE ${EIGEN3_INCLUDE_DIR})
  target_link_libraries(${EXE_VAR} ${EIGEN3_LIBRARY})

  target_include_directories(${EXE_VAR} PRIVATE ${BOOST_INCLUDE_DIR})
  target_link_libraries(${EXE_VAR} ${BOOST_LIBRARY})

endforeach()


