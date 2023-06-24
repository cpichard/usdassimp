add_executable(usdiffs main.cpp)
target_link_libraries(usdiffs PRIVATE ${PXR_LIBRARIES})
target_include_directories(usdiffs PRIVATE ${PXR_INCLUDE_DIRS})