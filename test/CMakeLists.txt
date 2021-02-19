set(target logging-test)

add_executable(${target} main.cpp log_manager_test.cpp logger_test.cpp)

find_package(GTest CONFIG REQUIRED)

target_link_libraries(${target}
  logging
   GTest::gtest
  )

set(cmd "${CMAKE_CURRENT_BINARY_DIR}/${target}")

add_test(NAME ${target} COMMAND ${cmd} --gtest_output=xml:${CMAKE_CURRENT_BINARY_DIR}/test_results/gtest-${target}.xml)

