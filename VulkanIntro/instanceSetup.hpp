#include <vulkan/vulkan.h>
#include <vector>
#include <GLFW/glfw3.h>

namespace instanceSetup {

	class InstanceSetup {
	public:
		InstanceSetup(std::vector<const char*> _validationLayers);

		InstanceSetup(const InstanceSetup&) = delete;
		InstanceSetup& operator=(const InstanceSetup&) = delete;

		VkInstance instance;
		VkSurfaceKHR surface;
		bool enableValidationLayers;

		void createInstance();
		void createSurface(GLFWwindow& glfwWindow);
		bool checkValidationLayerSupport();

		const std::vector<const char*> validationLayers;
	};
}