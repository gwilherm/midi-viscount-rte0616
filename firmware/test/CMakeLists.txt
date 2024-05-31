include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.13.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

# ===========================

# GTest and GMock do not play nicely with these flags, so disable them
if(NOT MSVC)
        target_compile_options(gtest PRIVATE
                -Wno-ctor-dtor-privacy
                -Wno-missing-include-dirs
                -Wno-sign-promo)
        target_compile_options(gmock PRIVATE -Wno-pedantic)
endif()

function(configure_test testExecutable)
    # Link against gtest library
    target_link_libraries(${testExecutable} gtest gtest_main gmock_main)
    if(NOT MSVC)
        # Disable variadic macro warnings (can be a problem when compiling with Clang)
        target_compile_options(${testExecutable} PRIVATE -Wno-gnu-zero-variadic-macro-arguments)
        # Compile and link with coverage
        target_compile_options(${testExecutable} PRIVATE -O0 --coverage)
        target_link_options(${testExecutable} PRIVATE --coverage)
    endif()
    # Create test name as the capitalized form of the test executable
    string(TOUPPER ${testExecutable} testName)
    # Add executable to test suite
    add_test(${testName} ${testExecutable} ${GTEST_RUN_FLAGS})
endfunction(configure_test)

# ====== Build
set(testsuite ${CMAKE_CURRENT_SOURCE_DIR}/testsuite)
set(mocks_dir ${CMAKE_CURRENT_SOURCE_DIR}/mocks)


# HardwareInterface test
add_executable(hw_int_test
        ${firmware_src_dir}/HardwareInterface.cpp
        ${mocks_dir}/Arduino.cpp
        ${mocks_dir}/SerialPrinter.cpp
        ${testsuite}/HardwareInterfaceTest.cpp)
target_include_directories(hw_int_test PUBLIC
        ${firmware_src_dir}
        ${testsuite}
        ${mocks_dir})
target_compile_options(hw_int_test PRIVATE -g -DFW_DEBUG_VERBOSE_HWINT_PROCESS -DFW_DEBUG_VERBOSE_HWINT_READ)
configure_test(hw_int_test)


# PedalInterface test
add_executable(pedal_int_test
        ${firmware_src_dir}/PedalInterface.cpp
        ${mocks_dir}/Arduino.cpp
        ${mocks_dir}/SerialPrinter.cpp
        ${testsuite}/PedalInterfaceTest.cpp)
target_include_directories(pedal_int_test PUBLIC
        ${firmware_src_dir}
        ${testsuite}
        ${mocks_dir})
target_compile_options(pedal_int_test PRIVATE -g)
configure_test(pedal_int_test)


# MIDISysexService test
add_executable(sysex_service_test
        ${firmware_src_dir}/MIDISysexService.cpp
        ${mocks_dir}/SerialPrinter.cpp
        ${testsuite}/MIDISysexServiceTest.cpp)
target_include_directories(sysex_service_test PUBLIC
        ${firmware_src_dir}
        ${testsuite}
        ${mocks_dir})
target_compile_options(sysex_service_test PRIVATE -g -DFW_DEBUG -DFW_VERSION="1.2.3")
configure_test(sysex_service_test)


# MIDINotesService test
add_executable(notes_service_test
        ${firmware_src_dir}/MIDINotesService.cpp
        ${firmware_src_dir}/pdlbrdkeys.cpp
        ${mocks_dir}/SerialPrinter.cpp
        ${testsuite}/MIDINotesServiceTest.cpp)
target_include_directories(notes_service_test PUBLIC
        ${firmware_src_dir}
        ${testsuite}
        ${mocks_dir})
target_compile_options(notes_service_test PRIVATE -g -DFW_DEBUG)
configure_test(notes_service_test)


# MIDICCService test
add_executable(cc_service_test
        ${firmware_src_dir}/MIDICCService.cpp
        ${firmware_src_dir}/pdlbrdkeys.cpp
        ${mocks_dir}/SerialPrinter.cpp
        ${testsuite}/MIDICCServiceTest.cpp)
target_include_directories(cc_service_test PUBLIC
        ${firmware_src_dir}
        ${testsuite}
        ${mocks_dir})
target_compile_options(cc_service_test PRIVATE -g)
configure_test(cc_service_test)


# MemoryService test
add_executable(memory_service_test
        ${firmware_src_dir}/MemoryService.cpp
        ${mocks_dir}/SerialPrinter.cpp
        ${testsuite}/MemoryServiceTest.cpp)
target_include_directories(memory_service_test PUBLIC
        ${firmware_src_dir}
        ${testsuite}
        ${mocks_dir})
target_compile_options(memory_service_test PRIVATE -g -DFW_VERSION="1.2.3")
configure_test(memory_service_test)


# FullChain test
add_executable(full_chain_test
        ${firmware_src_dir}/MIDINotesService.cpp
        ${firmware_src_dir}/HardwareInterface.cpp
        ${firmware_src_dir}/pdlbrdkeys.cpp
        ${mocks_dir}/Arduino.cpp
        ${mocks_dir}/SerialPrinter.cpp
        ${testsuite}/FullChainTest.cpp)
target_include_directories(full_chain_test PUBLIC
        ${firmware_src_dir}
        ${testsuite}
        ${mocks_dir})
target_compile_options(full_chain_test PRIVATE -g -DFW_DEBUG)
configure_test(full_chain_test)
