#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <string>

namespace graphicsEngine {

	class Window {
	public:
		Window(int w, int h, std::string name);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;


		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
		bool shouldClose() { return glfwWindowShouldClose(glfwWindow); }

	private:
		void initWindow();

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow *glfwWindow;

	};
}