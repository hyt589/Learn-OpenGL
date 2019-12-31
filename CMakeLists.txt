cmake_minimum_required(VERSION 3.0.0)
project(2 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(CTest)
enable_testing()

file(GLOB SOURCE_FILES
    "src/main.cpp"
    "src/glad.c"
)

add_executable(2 ${SOURCE_FILES})

include_directories(include src)

# adding libraries
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(lib/glfw-3.3)
add_subdirectory(lib/json-3.7.3)

set(LIBS glfw nlohmann_json::nlohmann_json)

target_link_libraries(2 ${LIBS})
# ---------------------------------------
# ---------------------------------------

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
