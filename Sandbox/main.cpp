#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Render/Window.h"
#include "Render/InputManager.h"
#include "Render/Logger.h"

#include "engine/assets/AssetManager.h"

#include "examples/ExamplesMenu.h"

#include "examples/ClearColorExample.h"
#include "examples/ColoredQuadExample.h"
#include "examples/TextureExample.h"
#include "examples/FragmentCircleExample.h"
#include "examples/Camera3DExample.h"
#include "examples/Lighting3DExample.h"
#include "examples/AdvancedLighting3DExample.h"
#include "examples/SceneGraphExample.h"
#include "examples/StencilExample.h"
#include "examples/FramebufferExample.h"
#include "examples/StressTestExample.h"

int main() {
	engine::AssetManager::setAssetsPath("../../../../Sandbox/assets/");

	tools::Logger::Init(spdlog::level::trace);

	Renderer::Window window(1400, 1000, "OpenGL Sandbox");
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
	menu.registerScene<Sandbox::Lighting3DExample>("3d lighting", "");
	menu.registerScene<Sandbox::AdvancedLighting3DExample>("advanced 3d lighting", "");
    menu.registerScene<Sandbox::StencilExample>("Stencil buffer", "");
    menu.registerScene<Sandbox::FramebufferExample>("Frame buffer", "");
    menu.registerScene<Sandbox::StressTestExample>("Stress test", "");
    menu.registerScene<Sandbox::SceneGraphExample>("Sandbox", "");

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

        if (tools::InputManager::instance()->keyDown(GLFW_KEY_F4)) {
            static bool normal_map_enabled = false;
			menu.getCurrentScene()->setDebugMode(engine::DebugMode::ShowNormalMaps, normal_map_enabled = !normal_map_enabled);
        }

        if (tools::InputManager::instance()->keyDown(GLFW_KEY_F5)) {
            static int display_mode = 0;

			if(display_mode >= (uint32_t)engine::DebugDisplayMode::Count)
				display_mode = 0;
			else
				display_mode++;

            menu.getCurrentScene()->setDebugDisplayMode((engine::DebugDisplayMode)display_mode);
        }

		if (tools::InputManager::instance()->mouseKeyPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
			window.setCursorEnabled(false);
		}
		else {
			window.setCursorEnabled(true);
		}

		if (tools::InputManager::instance()->keyDown(GLFW_KEY_F10)) {
			static bool fullscreen = false;
			window.setFullscreen(fullscreen = !fullscreen);
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
