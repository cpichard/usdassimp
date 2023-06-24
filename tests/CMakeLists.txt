include(CTest)

# Simple tool to diff usd without python and specialized for testing usdassimp
add_subdirectory(usdiffs)

# Tests
add_test(NAME read_ply_001 WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} COMMAND usdiffs example1.ply example1.usda)

# TODO add environment with plugin and USD as well
set_tests_properties(read_ply_001 PROPERTIES
    ENVIRONMENT PXR_PLUGINPATH_NAME=${CMAKE_BINARY_DIR}/usd)