cmake_minimum_required(VERSION 3.8)
project(sqrt_ekf)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED)

add_executable(example src/sim/simulate.cpp src/util/thread_util.h)

target_include_directories(example PRIVATE ${EIGEN3_INCLUDE_DIR})
target_link_libraries(example ${EIGEN3_LIBRARY})

target_include_directories(example PRIVATE ${BOOST_INCLUDE_DIR})
target_link_libraries(example ${BOOST_LIBRARY})


