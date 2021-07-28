cmake_minimum_required (VERSION 3.8)
project(Render)

message(STATUS "-----> Current Project: ${PROJECT_NAME}")
message(STATUS "-----> Current SOURCE directory: ${PROJECT_SOURCE_DIR}")
message(STATUS "-----> Current BUILD directory: ${CMAKE_CURRENT_BINARY_DIR}")

set(HEADERS

)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/Render.cpp
)

add_library( ${PROJECT_NAME} STATIC
             "src/Render.cpp" )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include  
)

