cmake_minimum_required (VERSION 3.14)
project(UsdAssimpPlugin)
set(CMAKE_CXX_STANDARD 14)

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
    URL https://github.com/assimp/assimp/archive/refs/tags/v5.2.4.tar.gz 
)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)
set(ASSIMP_BUILD_DOCS OFF)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_ASSIMP_VIEW OFF)
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

install(TARGETS usdAssimp
        RUNTIME DESTINATION plugin
        LIBRARY DESTINATION plugin
        ARCHIVE DESTINATION plugin
)
install(DIRECTORY src/usdAssimp DESTINATION plugin/usd )
install(FILES src/plugInfo.json DESTINATION plugin/usd)

