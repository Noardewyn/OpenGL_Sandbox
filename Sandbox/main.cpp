#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Render/Window.h"

#include "examples/ClearColorExample.h"
#include "examples/ColoredQuadExample.h"
#include "examples/TextureExample.h"

#include "examples/ExamplesMenu.h"

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main() {
	glfwSetErrorCallback(glfw_error_callback);

	Renderer::Window window(800, 800, "OpenGL Sandbox");

	// imgui init
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.getHandle(), true);
	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);

	Sandbox::ExamplesMenu menu(&window);
	menu.registerScene<Sandbox::ClearColorExample>("Clear color", "");
	menu.registerScene<Sandbox::ColoredQuadExample>("Colored quad", "");
	menu.registerScene<Sandbox::TextureExample>("Textured quad", "");

	menu.startScene(2);

	while (window.isOpen()) {
		window.pollEvents();
		menu.getCurrentScene()->onUpdate(window.getDeltaTime());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		menu.getCurrentScene()->onImGuiRender();

		if(menu.getCurrentScene() != &menu)
			menu.onImGuiRender();
		ImGui::Render();

		window.display_start();
		menu.getCurrentScene()->onRender();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		window.display_finish();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return 0;
}
