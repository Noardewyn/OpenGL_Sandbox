using namespace std;

#include "Render/Window.h"
#include "Render/Shader.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/VertexArray.h"

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

int main()
{
	Renderer::Window window(800, 800, "OpenGL Sandbox");

	Renderer::VertexArray vao;
	Renderer::VertexBuffer vbo(vertices, sizeof(vertices));
	Renderer::IndexBuffer ibo(indices, sizeof(indices));

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	vao.unbind();

	Renderer::Shader plain_shader("./assets/shaders/plain.frag", "./assets/shaders/plain.vs");
	
	while (window.isOpen()) {
		window.pollEvents();
		window.clear(Renderer::Color::Black);
		window.display_start();

		plain_shader.bind();
		plain_shader.setUniform4f("color", 1.0f, 1.0f, 0.0f, 1.0f);

		vao.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		window.display_finish();
	}

	return 0;
}
