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
