cmake_minimum_required(VERSION 3.13)

project(motor_control)

set(CMAKE_CXX_FLAGS "-g -std=c++0x")

# Provide the path to the vsomeip installation directory
set(vsomeip3_DIR "/usr/local/include/compat/vsomeip")


find_package(vsomeip3 3.4.10 REQUIRED)
find_package(Boost 1.55 COMPONENTS system thread log REQUIRED)

include_directories(
    ${Boost_INCLUDE_DIR}
    ${VSOMEIP_INCLUDE_DIRS}
)

add_executable(service-test ../service/motor.cpp)
target_link_libraries(service-test vsomeip3 ${Boost_LIBRARIES})

add_executable(client-test ../client/cluster.cpp)
target_link_libraries(client-test vsomeip3 ${Boost_LIBRARIES})