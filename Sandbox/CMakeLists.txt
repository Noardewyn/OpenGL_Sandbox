cmake_minimum_required (VERSION 3.8)
project ("Sandbox")

message(STATUS "-----> Current Project: ${PROJECT_NAME}")
message(STATUS "-----> Current SOURCE directory: ${PROJECT_SOURCE_DIR}")
message(STATUS "-----> Current BUILD directory: ${CMAKE_CURRENT_BINARY_DIR}")

set(HEADERS

)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp
)

add_executable (${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries( ${PROJECT_NAME} Render)

message(${PROJECT_BINARY_DIR})

# Copy assets to project binary dir
add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${PROJECT_SOURCE_DIR}/assets/
                ${CMAKE_BINARY_DIR}/bin/assets/)