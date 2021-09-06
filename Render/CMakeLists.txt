cmake_minimum_required (VERSION 3.8)
project(Render)

message(STATUS "-----> Current Project: ${PROJECT_NAME}")
message(STATUS "-----> Current SOURCE directory: ${PROJECT_SOURCE_DIR}")
message(STATUS "-----> Current BUILD directory: ${CMAKE_CURRENT_BINARY_DIR}")

set(HEADERS

)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/Window.cpp
    ${PROJECT_SOURCE_DIR}/src/Color.cpp
    ${PROJECT_SOURCE_DIR}/src/Shader.cpp
    ${PROJECT_SOURCE_DIR}/src/VertexBuffer.cpp
    ${PROJECT_SOURCE_DIR}/src/IndexBuffer.cpp
    ${PROJECT_SOURCE_DIR}/src/VertexArray.cpp
    ${PROJECT_SOURCE_DIR}/src/Texture.cpp
    ${PROJECT_SOURCE_DIR}/src/Transform.cpp
    ${PROJECT_SOURCE_DIR}/src/Camera.cpp
    ${PROJECT_SOURCE_DIR}/src/InputManager.cpp
    ${PROJECT_SOURCE_DIR}/src/Logger.cpp
    ${PROJECT_SOURCE_DIR}/src/Renderer.cpp
    ${PROJECT_SOURCE_DIR}/src/FrameBuffer.cpp
    ${PROJECT_SOURCE_DIR}/src/RenderBuffer.cpp
)

set(LIBS
    opengl32 
    glfw3
    GLEW::GLEW
    spdlog::spdlog
    stb::stb
)

add_library( ${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME}  ${LIBS})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include  
)

