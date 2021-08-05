#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Render/Window.h"
#include "Render/InputManager.h"

#include "examples/ClearColorExample.h"
#include "examples/ColoredQuadExample.h"
#include "examples/TextureExample.h"
#include "examples/FragmentCircleExample.h"
#include "examples/Camera3DExample.h"

#include "examples/ExamplesMenu.h"

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main() {
	glfwSetErrorCallback(glfw_error_callback);

	Renderer::Window window(1000, 1000, "OpenGL Sandbox");
	tools::InputManager::init(window);

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
	menu.registerScene<Sandbox::FragmentCircleExample>("Fragment shader circle", "");
	menu.registerScene<Sandbox::Camera3DExample>("3d camera", "");

	menu.startScene(menu.scenesCount()-1);

	while (window.isOpen()) {
		window.pollEvents();
		
		if (tools::InputManager::instance()->keyDown(GLFW_KEY_ESCAPE)) {
			window.close();
		}

		if (tools::InputManager::instance()->keyDown(GLFW_KEY_F3)) {
			static bool wireframe_enabled = false;
			if (wireframe_enabled = !wireframe_enabled)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		if (tools::InputManager::instance()->keyDown(GLFW_KEY_F2)) {
			static bool cursor_enabled = true;
			window.setCursorEnabled(cursor_enabled = !cursor_enabled);
		}

		menu.getCurrentScene()->onUpdate(window.getDeltaTime());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		menu.getCurrentScene()->onImGuiRender();
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
