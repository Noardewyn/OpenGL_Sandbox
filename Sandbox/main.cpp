#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Render/Window.h"
#include "Render/Shader.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/VertexArray.h"
#include "Render/VertexBufferLayout.h"

GLfloat vertices[] = {
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
};

GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
};

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main()
{
	glfwSetErrorCallback(glfw_error_callback);

	Renderer::Window window(800, 800, "OpenGL Sandbox");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.getHandle(), true);
	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);

	Renderer::VertexArray vao;
	Renderer::VertexBuffer vbo(vertices, sizeof(vertices));
	Renderer::VertexBufferLayout layout;
	layout.push<float>(3);

	vao.addBuffer(vbo, layout);
	vao.unbind();

	Renderer::IndexBuffer ibo(indices, sizeof(indices));

	Renderer::Shader plain_shader("./assets/shaders/plain.frag", "./assets/shaders/plain.vs");
	
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	while (window.isOpen()) {
		window.pollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			ImGui::Begin("Hello, world!");
			ImGui::ColorEdit3("clear color", (float*)&clear_color);

			ImGui::End();
		}

		ImGui::Render();
		window.clear(Renderer::Color(clear_color.x, clear_color.y, clear_color.z, clear_color.w));
		window.display_start();

		plain_shader.bind();
		plain_shader.setUniform4f("color", 1.0f, 0.0f, 1.0f, 1.0f);

		vao.bind();
		ibo.bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		window.display_finish();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return 0;
}
