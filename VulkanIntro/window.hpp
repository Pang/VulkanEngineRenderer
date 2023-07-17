#pragma once
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <string>

namespace graphicsEngine {

	class Window {
	public:
		Window(int w, int h, std::string name);
		~Window();

		Window(const Window&) = delete;
		Window &operator=(const Window&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(glfwWindow); }
		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

		bool wasWindowResized() { return framebufferResized; }
		void resetWindowResizeFlag() { framebufferResized = true; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		void initWindow();
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

		int width;
		int height;
		bool framebufferResized;

		std::string windowName;
		GLFWwindow *glfwWindow;
	};
}
