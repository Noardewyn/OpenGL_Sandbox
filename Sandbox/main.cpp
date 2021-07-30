using namespace std;

#include "Render/Window.h"
#include "Render/Shader.h"

int main()
{
	Renderer::Window window(800, 600, "OpenGL Sandbox");

	Renderer::Shader plain_shader("./assets/shaders/plain.frag", "./assets/shaders/plain.vs");

	plain_shader.setUniform4f("color", 1.0f, 0.0f, 0.0f, 1.0f);
	plain_shader.bind();

	while (window.isOpen()) {
		window.pollEvents();
		window.clear(Renderer::Color::Magenta);
		
		window.display();
	}

	return 0;
}
