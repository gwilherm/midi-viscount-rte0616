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

# Calculator test
add_executable(testone
        ${testsuite}/testone.cpp)
# target_include_directories(calculator_test PUBLIC
#         ${calculator_dir}
#         ${mocks})

target_link_libraries(testone gtest gtest_main gmock_main)
add_test(TEST_ONE testone ${GTEST_RUN_FLAGS})
