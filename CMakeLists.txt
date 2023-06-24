cmake_minimum_required (VERSION 3.14)
project(UsdAssimpPlugin)
set(CMAKE_CXX_STANDARD 14)

# Xcode prefixes the lib with a 'd', forcing us to modify the pluginfo.json
# This forces the postfix to be empty
set(CMAKE_DEBUG_POSTFIX "")

# We need usd
find_package(pxr REQUIRED)
# we might need materialx is pxr was compiled with it (it's a pain)
find_package(MaterialX REQUIRED)

# We need the assimp library, so we download it
set(BUILD_SHARED_LIBS OFF)
set(FETCH_CONTENT_QUIET OFF)
include(FetchContent)
FetchContent_Declare(
    assimp
    URL https://github.com/assimp/assimp/archive/refs/tags/v5.2.5.tar.gz 
)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)
set(ASSIMP_BUILD_DOCS OFF)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_ASSIMP_VIEW OFF)
set(ASSIMP_WARNINGS_AS_ERRORS OFF) # to compile on macOS Ventura
#https://discourse.cmake.org/t/how-to-use-fetchcontent-correctly-for-building-external-dependencies/3686
#set_target_properties(assimp PROPERTIES EXCLUDE_FROM_ALL ON)
FetchContent_GetProperties(assimp)
if(NOT assimp_POPULATED)
   FetchContent_Populate(assimp)
    add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
#FetchContent_MakeAvailable(assimp)

# Show the folders in xcode and vsc++
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# test compilation
#add_executable(usdassimp "")

set(BUILD_SHARED_LIBS ON)
add_library(usdAssimp "")
set_target_properties(usdAssimp PROPERTIES PREFIX "")

# the code is in src
add_subdirectory(src)

target_compile_definitions(usdAssimp PRIVATE NOMINMAX)
#target_link_libraries(usdAssimp assimp ${PXR_LIBRARIES})
#target_link_libraries(usdAssimp PRIVATE assimp ${ASSIMP_LIBRARIES} ${PXR_LIBRARIES})
#target_include_directories(usdAssimp PRIVATE ${ASSIMP_INCLUDE_DIR} ${PXR_INCLUDE_DIRS})

target_link_libraries(usdAssimp PRIVATE assimp ${PXR_LIBRARIES})
target_include_directories(usdAssimp PRIVATE ${PXR_INCLUDE_DIRS})

# TODO flag with tests
enable_testing() 
add_subdirectory(tests)
# We copy the plugInfo.json in the build folder so the tests can load the plugin
file(COPY src/usdAssimp DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/usd)
file(COPY src/plugInfo.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/usd)

install(TARGETS usdAssimp
        RUNTIME DESTINATION plugin
        LIBRARY DESTINATION plugin
        ARCHIVE DESTINATION plugin
)

install(DIRECTORY src/usdAssimp DESTINATION plugin/usd)
install(FILES src/plugInfo.json DESTINATION plugin/usd)

# Special sauce to install the plugInfo.json with the correct library name
#set(TARGET_LIB_NAME $<TARGET_FILE_NAME:usdAssimp>)
#file(READ ${CMAKE_CURRENT_SOURCE_DIR}/src/usdAssimp/resources/plugInfo.json.in PLUGINFOJSON)
#install(CODE "
#cmake_policy(SET CMP0053 OLD)
#set(TARGET_LIB_NAME $<TARGET_FILE_NAME:usdAssimp>)
#file(CONFIGURE 
# OUTPUT ${CMAKE_INSTALL_PREFIX}/plugin/usd/usdAssimp/resources/plugInfo.json
# CONTENT \"${PLUGINFOJSON}\"
# NEWLINE_STYLE UNIX)")

