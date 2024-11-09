cmake_minimum_required (VERSION 3.10)
project (emp-min)
set(NAME "emp-min")
find_path(CMAKE_FOLDER NAMES cmake/emp-tool-config.cmake)
include(${CMAKE_FOLDER}/cmake/emp-base.cmake)

find_package(emp-tool REQUIRED)
find_package(emp-ot REQUIRED)
find_package(emp-sh2pc REQUIRED)



add_executable(Millionaire src/millionaire.cc)

target_link_libraries(
  Millionaire
  ${EMP-TOOL_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${Boost_LIBRARIES})

target_include_directories(
  Millionaire PUBLIC
  ${EMP-TOOL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS})


add_executable(GCD src/gcd.cc)

target_link_libraries(
  GCD
  ${EMP-TOOL_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${Boost_LIBRARIES})

target_include_directories(
  GCD PUBLIC
  ${EMP-TOOL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS})


add_executable(CircuitFile src/circuitfile.cc)

target_link_libraries(
  CircuitFile
  ${EMP-TOOL_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${Boost_LIBRARIES})

target_include_directories(
  CircuitFile PUBLIC
  ${EMP-TOOL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS})


add_executable(Set_Intersection src/set_intersection.cc)

target_link_libraries(
  Set_Intersection
  ${EMP-TOOL_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${Boost_LIBRARIES})

target_include_directories(
  Set_Intersection PUBLIC
  ${EMP-TOOL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS})


add_executable(Testing_Arr src/testing_arr.cc)

target_link_libraries(
  Testing_Arr
  ${EMP-TOOL_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${Boost_LIBRARIES})

target_include_directories(
  Testing_Arr PUBLIC
  ${EMP-TOOL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS})


