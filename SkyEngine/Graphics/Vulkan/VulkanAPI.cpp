#include "SEPCH.h"
#include "VulkanAPI.h"
#include "imgui_impl_vulkan.h"
#include "Core/Application.h"
#include "Math/Matrix.h"
#include "Math/Transform.h"
#include "Platform/Window/GLFW/GLFWWindow.h"
#include "Platform/Window/GLFW/imgui_impl_glfw.h"
#include "System/LogManager.h"

// TODO: Review - should this be a method? Or do we assume only VulkanAPI.cpp will call raw vulkan?
void VKCheck(VkResult Err)
{
	if (Err == 0)
	{
		return;
	}

	// TODO: Remove duplicate logging after assert / ensure outputs proper logs
	CLogManager::GetInstance()->DisplayLogError(std::format("[vulkan] Error: VkResult = {}\n", static_cast<int32_t>(Err)));
	ASSERT(Err == 0, std::format("[vulkan] Error: VkResult = {}\n", static_cast<int32_t>(Err)).c_str());
}

bool IsExtensionAvailable(const std::vector<VkExtensionProperties>& Properties, const char* Extensions)
{
	for (const VkExtensionProperties& P : Properties)
		if (strcmp(P.extensionName, Extensions) == 0)
			return true;
	return false;
}

CVulkanAPI::~CVulkanAPI()
{
	
}

void CVulkanAPI::Init()
{
	// TODO: Allow requesting extra extensions before calling init?
	std::vector<const char*> extensions;
	uint32_t ExtensionsCount = 0;
	const char** GlfwExtensions = glfwGetRequiredInstanceExtensions(&ExtensionsCount);
	for (uint32_t i = 0; i < ExtensionsCount; i++)
		extensions.push_back(GlfwExtensions[i]);

	VkResult Err;

	// Create Vulkan Instance
	{
		VkInstanceCreateInfo InstanceCreateInfo = {};
		InstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

		// Enumerate available extensions
		uint32_t InstPropertiesCount;
		std::vector<VkExtensionProperties> InstProperties;
		vkEnumerateInstanceExtensionProperties(nullptr, &InstPropertiesCount, nullptr);
		InstProperties.resize(InstPropertiesCount);
		Err = vkEnumerateInstanceExtensionProperties(nullptr, &InstPropertiesCount, InstProperties.data());
		VKCheck(Err);

		// Enable required extensions
		std::vector<const char*> InstanceExtensions;
		if (IsExtensionAvailable(InstProperties, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
			InstanceExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#ifdef VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
		if (IsExtensionAvailable(InstProperties, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME))
		{
			InstanceExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
			InstanceCreateInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
		}
#endif

		// Enabling validation layers
		#ifdef IMGUI_VULKAN_DEBUG_REPORT
		const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
		create_info.enabledLayerCount = 1;
		create_info.ppEnabledLayerNames = layers;
		instance_extensions.push_back("VK_EXT_debug_report");
#endif

		// Create Vulkan Instance
		InstanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(InstanceExtensions.size());
		InstanceCreateInfo.ppEnabledExtensionNames = InstanceExtensions.data();
		Err = vkCreateInstance(&InstanceCreateInfo, Allocator, &Instance);
		VKCheck(Err);

		// Setup the debug report callback
#ifdef IMGUI_VULKAN_DEBUG_REPORT
		auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkCreateDebugReportCallbackEXT");
		IM_ASSERT(vkCreateDebugReportCallbackEXT != nullptr);
		VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
		debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
		debug_report_ci.pfnCallback = debug_report;
		debug_report_ci.pUserData = nullptr;
		err = vkCreateDebugReportCallbackEXT(g_Instance, &debug_report_ci, g_Allocator, &g_DebugReport);
		check_vk_result(err);
#endif

		// Select Physical Device (GPU)
		PhysicalDevice = SetupVulkan_SelectPhysicalDevice();

		// Select graphics queue family
		{
			uint32_t Count;
			vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &Count, nullptr);
			auto* Queues = static_cast<VkQueueFamilyProperties*>(malloc(sizeof(VkQueueFamilyProperties) * Count));
			vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevice, &Count, Queues);
			for (uint32_t i = 0; i < Count; i++)
				if (Queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					QueueFamily = i;
					break;
				}
			free(Queues);
			ASSERT(QueueFamily != static_cast<uint32_t>(-1), "Invalid QueueFamily");
		}

		// Create Logical Device (with 1 queue)
		{
			std::vector<const char*> DeviceExtensions;
			DeviceExtensions.push_back("VK_KHR_swapchain");

			// Enumerate physical device extension
			uint32_t DevicePropertiesCount;
			std::vector<VkExtensionProperties> DeviceProperties;
			vkEnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &DevicePropertiesCount, nullptr);
			DeviceProperties.resize(DevicePropertiesCount);
			vkEnumerateDeviceExtensionProperties(PhysicalDevice, nullptr, &DevicePropertiesCount, DeviceProperties.data());
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
			if (IsExtensionAvailable(Properties, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME))
				DeviceExtensions.push_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
#endif

			constexpr float QueuePriority[] = { 1.0f };
			VkDeviceQueueCreateInfo QueueInfo[1] = {};
			QueueInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			QueueInfo[0].queueFamilyIndex = QueueFamily;
			QueueInfo[0].queueCount = 1;
			QueueInfo[0].pQueuePriorities = QueuePriority;
			VkDeviceCreateInfo DeviceCreateInfo = {};
			DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			DeviceCreateInfo.queueCreateInfoCount = std::size(QueueInfo);
			DeviceCreateInfo.pQueueCreateInfos = QueueInfo;
			DeviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size());
			DeviceCreateInfo.ppEnabledExtensionNames = DeviceExtensions.data();
			Err = vkCreateDevice(PhysicalDevice, &DeviceCreateInfo, Allocator, &Device);
			VKCheck(Err);
			vkGetDeviceQueue(Device, QueueFamily, 0, &Queue);
		}

		// Create Descriptor Pool
		// The example only requires a single combined image sampler descriptor for the font image and only uses one descriptor set (for that)
		// If you wish to load e.g. additional textures you may need to alter pools sizes.
		{
			constexpr VkDescriptorPoolSize PoolSizes[] =
			{
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
			};
			VkDescriptorPoolCreateInfo PoolInfo = {};
			PoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			PoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			PoolInfo.maxSets = 1;
			PoolInfo.poolSizeCount = static_cast<uint32_t>(std::size(PoolSizes));
			Err = vkCreateDescriptorPool(Device, &PoolInfo, Allocator, &DescriptorPool);
			VKCheck(Err);
		}
	}
}

void CVulkanAPI::ImGuiInit()
{
	GLFWwindow* Window = std::static_pointer_cast<CGLFWWindow>(GetApplication()->GetApplicationWindow())->GetGlWindow();

	// Create Window Surface
	VkSurfaceKHR Surface;
	VkResult Err = glfwCreateWindowSurface(Instance, Window, Allocator, &Surface);
	VKCheck(Err);
	
	// Create Framebuffers
	int Width, Height;
	glfwGetFramebufferSize(Window, &Width, &Height);
	ImGui_ImplVulkanH_Window* Wd = &MainWindowData;
	SetupVulkanWindow(Wd, Surface, Width, Height);

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForVulkan(Window, true);
	ImGui_ImplVulkan_InitInfo InitInfo = {};
	InitInfo.Instance = Instance;
	InitInfo.PhysicalDevice = PhysicalDevice;
	InitInfo.Device = Device;
	InitInfo.QueueFamily = QueueFamily;
	InitInfo.Queue = Queue;
	InitInfo.PipelineCache = PipelineCache;
	InitInfo.DescriptorPool = DescriptorPool;
	InitInfo.Subpass = 0;
	InitInfo.MinImageCount = MinImageCount;
	InitInfo.ImageCount = Wd->ImageCount;
	InitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	InitInfo.Allocator = Allocator;
	InitInfo.CheckVkResultFn = VKCheck;
	ImGui_ImplVulkan_Init(&InitInfo, Wd->RenderPass);
}

VkPhysicalDevice CVulkanAPI::SetupVulkan_SelectPhysicalDevice() const
{
	uint32_t GPUCount;
	VkResult Err = vkEnumeratePhysicalDevices(Instance, &GPUCount, nullptr);
	VKCheck(Err);
	ASSERT(GPUCount > 0, "GPUCount cannot be zero");

	std::vector<VkPhysicalDevice> GPUs;
	GPUs.resize(GPUCount);
	Err = vkEnumeratePhysicalDevices(Instance, &GPUCount, GPUs.data());
	VKCheck(Err);

	// If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
	// most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
	// dedicated GPUs) is out of scope of this sample.
	for (const VkPhysicalDevice& GPU : GPUs)
	{
		VkPhysicalDeviceProperties Properties;
		vkGetPhysicalDeviceProperties(GPU, &Properties);
		if (Properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			return GPU;
	}

	// Use first GPU (Integrated) is a Discrete one is not available.
	if (GPUCount > 0)
		return GPUs[0];
	return VK_NULL_HANDLE;
}

void CVulkanAPI::SetupVulkanWindow(ImGui_ImplVulkanH_Window* Wd, VkSurfaceKHR Surface, int Width, int Height) const
{
	Wd->Surface = Surface;

	// Check for WSI support
	VkBool32 Res;
	vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, QueueFamily, Wd->Surface, &Res);
	if (Res != VK_TRUE)
	{
		ASSERT(false, "Error no WSI support on physical device 0\n");
	}

	// Select Surface Format
	constexpr VkFormat RequestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
	constexpr VkColorSpaceKHR RequestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	Wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(PhysicalDevice, Wd->Surface, RequestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(RequestSurfaceImageFormat), RequestSurfaceColorSpace);

	// Select Present Mode
	#ifdef IMGUI_UNLIMITED_FRAME_RATE
	VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
	constexpr VkPresentModeKHR PresentModes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
	Wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(PhysicalDevice, Wd->Surface, &PresentModes[0], IM_ARRAYSIZE(PresentModes));
	//printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

	// Create SwapChain, RenderPass, Framebuffer, etc.
	ASSERT(MinImageCount >= 2, "MinImageCount must be >= 2");
	ImGui_ImplVulkanH_CreateOrResizeWindow(Instance, PhysicalDevice, Device, Wd, QueueFamily, Allocator, Width, Height, MinImageCount);
}


std::string CVulkanAPI::GetGraphicsDisplayName()
{
	//TODO: Implement
	return "";
}

TPointer<CTexture> CVulkanAPI::GetTexture(const std::string& TextureSource, bool bAA)
{
	//TODO: Implement
	return nullptr;
}

bool CVulkanAPI::CreateShaderProgram(uint32_t& ProgramID, const char* VertexShaderFilename,
	const char* FragmentShaderFilename, const char* GeometryShaderFilename)
{
	//TODO: Implement
	return false;
}

bool CVulkanAPI::CreateTessProgram(uint32_t& ProgramID, const char* VertexShaderFilename,
	const char* FragmentShaderFilename, const char* TessControlShaderFilename, const char* TessEvalShaderFilename)
{
	//TODO: Implement
	return false;
}

bool CVulkanAPI::CreateComputeProgram(uint32_t& ProgramID, const char* ComputeShaderFilename)
{
	//TODO: Implement
	return false;
}

TPointer<IFramebuffer> CVulkanAPI::CreateFramebuffer(const SFramebufferSpecification& Specification)
{
	//TODO: Implement
	return nullptr;
}

unsigned CVulkanAPI::CreateVertexBuffer(const MeshData& MeshData)
{
	//TODO: Implement
	return -1;
}

void CVulkanAPI::BindVertexArray(const std::vector<float>& vertices, const std::vector<uint32_t>& indices,
	unsigned& vao)
{
	//TODO: Implement
}

void CVulkanAPI::RenderMesh(TPointer<CMeshComponent> Mesh)
{
	//TODO: Implement
}

void CVulkanAPI::CleanupMesh(TPointer<CMeshComponent> Mesh)
{
	//TODO: Implement
}

void CVulkanAPI::ApplyMVP(uint32_t Program, Matrix4 View, Matrix4 Projection, STransform Transform)
{
	//TODO: Implement
}

void CVulkanAPI::RenderImGui()
{
	//TODO: Implement
}

void CVulkanAPI::BindShader(uint32_t ShaderProgramID)
{
	//TODO: Implement
}

void CVulkanAPI::Clear(SVector ClearColour)
{
	//TODO: Implement
}

void CVulkanAPI::SetRenderViewportSize(const SVector2i InViewportSize)
{
	//TODO: Implement
}

void CVulkanAPI::SetWireframeMode(bool bInWireframeEnabled)
{
	//TODO: Implement
}

void CVulkanAPI::ApplyMaterialFlags(TPointer<CMaterialInterface> InMaterial)
{
	//TODO: Implement
}

int32_t CVulkanAPI::GetAttributeLocation(const uint32_t ShaderProgram, std::string AttributeName)
{
	//TODO: Implement
	return -1;
}

void CVulkanAPI::PassAttributeToShader(int32_t ShaderLocation, float Attribute)
{
	//TODO: Implement
}

void CVulkanAPI::PassAttributeToShader(int32_t ShaderLocation, int Attribute)
{
	//TODO: Implement
}

void CVulkanAPI::PassAttributeToShader(int32_t ShaderLocation, bool Attribute)
{
	//TODO: Implement
}

void CVulkanAPI::PassAttributeToShader(int32_t ShaderLocation, SVector Attribute)
{
	//TODO: Implement
}

void CVulkanAPI::PassAttributeToShader(int32_t ShaderLocation, SVector4 Attribute)
{
	//TODO: Implement
}

void CVulkanAPI::PassAttributeToShader(int32_t ShaderLocation, Matrix4 Attribute)
{
	//TODO: Implement
}

void CVulkanAPI::PassAttributeToShader(int32_t ShaderLocation, TPointer<CTexture> Attribute)
{
	//TODO: Implement
}
