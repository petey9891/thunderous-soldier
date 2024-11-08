# tests/CMakeLists.txt
set(TEST_SOURCES main.spec.cpp)

# Add only the necessary source files, excluding main.cpp
file(GLOB SRC_FILES "../src/minimax.cpp" "../src/minimax2Player.cpp" "../src/router.cpp" "../src/basic.cpp")

# Create the test executable
add_executable(run_tests ${TEST_SOURCES} ${SRC_FILES})

# Link the test executable with pthread
target_link_libraries(run_tests PRIVATE pthread)

# Include the include directory
target_include_directories(run_tests PRIVATE ../include)

# Copy JSON file to the build directory
add_custom_command(
        TARGET run_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/exampleMove.json ${CMAKE_CURRENT_BINARY_DIR}/exampleMove.json
)

# Add the test to CTest
add_test(NAME BattlesnakeTests COMMAND run_tests)
