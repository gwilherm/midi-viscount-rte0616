include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.13.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)


# ====== Build
set(testsuite ${CMAKE_CURRENT_SOURCE_DIR}/testsuite)
set(mocks_dir ${CMAKE_CURRENT_SOURCE_DIR}/mocks)

# HardwareInterface test
add_executable(hw_int_test
        ${firmware_src_dir}/HardwareInterface.cpp
        ${mocks_dir}/Arduino.cpp
        ${mocks_dir}/ArduinoMock.cpp
        ${testsuite}/HardwareInterfaceTest.cpp)
target_include_directories(hw_int_test PUBLIC
        ${firmware_src_dir}
        ${mocks_dir})

target_link_libraries(hw_int_test gtest gtest_main gmock_main)
add_test(TEST_ONE hw_int_test ${GTEST_RUN_FLAGS})
