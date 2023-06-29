#include "app.hpp"

namespace graphicsEngine {
	void App::run() {
        while (!window.shouldClose()) {
            glfwPollEvents();
        }
	}
}
