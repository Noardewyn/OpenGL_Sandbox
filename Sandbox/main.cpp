using namespace std;

#include "Render/Window.h"

int main()
{
	Window window(800, 600, "OpenGL Sandbox");
	
	
	while (window.isOpen()) {
		window.pollEvents();
		window.clear(Color::Magenta);
		window.display();
	}

	return 0;
}
