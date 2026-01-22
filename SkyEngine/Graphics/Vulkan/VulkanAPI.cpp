#include "SEPCH.h"
#include "VulkanAPI.h"
#include "imgui_impl_vulkan.h"
#include "VkFramebuffer.h"
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
	CLogManager::Get()->DisplayError(std::format("[vulkan] Error: VkResult = {}\n", static_cast<int32_t>(Err)));
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
	// TODO: Allow requesting extra extensions externally before calling init?
	std::vector<const char*> InstanceExtensions;
	uint32_t ExtensionsCount = 0;
	const char** GlfwExtensions = glfwGetRequiredInstanceExtensions(&ExtensionsCount);
	for (uint32_t i = 0; i < ExtensionsCount; i++)
		InstanceExtensions.push_back(GlfwExtensions[i]);

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
			{
				if (Queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					QueueFamily = i;
					break;
				}
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
			DeviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(std::size(QueueInfo));
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
	SetupVulkanWindow(Surface, Width, Height);

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
	InitInfo.ImageCount = ImGuiWindow.ImageCount;
	InitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	InitInfo.Allocator = Allocator;
	InitInfo.CheckVkResultFn = VKCheck;
	ImGui_ImplVulkan_Init(&InitInfo, ImGuiWindow.RenderPass);

	// Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    // Upload Fonts
    {
        // Use any command queue
        const VkCommandPool CommandPool = ImGuiWindow.Frames[ImGuiWindow.FrameIndex].CommandPool;
        const VkCommandBuffer CommandBuffer = ImGuiWindow.Frames[ImGuiWindow.FrameIndex].CommandBuffer;

        Err = vkResetCommandPool(Device, CommandPool, 0);
        VKCheck(Err);
        VkCommandBufferBeginInfo BeginInfo = {};
        BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        BeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        Err = vkBeginCommandBuffer(CommandBuffer, &BeginInfo);
        VKCheck(Err);

        ImGui_ImplVulkan_CreateFontsTexture(CommandBuffer);

        VkSubmitInfo EndInfo = {};
        EndInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        EndInfo.commandBufferCount = 1;
        EndInfo.pCommandBuffers = &CommandBuffer;
        Err = vkEndCommandBuffer(CommandBuffer);
        VKCheck(Err);
        Err = vkQueueSubmit(Queue, 1, &EndInfo, VK_NULL_HANDLE);
        VKCheck(Err);

        Err = vkDeviceWaitIdle(Device);
        VKCheck(Err);
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

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

void CVulkanAPI::SetupVulkanWindow(VkSurfaceKHR Surface, int Width, int Height)
{
	ImGuiWindow.Surface = Surface;

	// Check for WSI support
	VkBool32 Res;
	vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice, QueueFamily, ImGuiWindow.Surface, &Res);
	if (Res != VK_TRUE)
	{
		ASSERT(false, "Error no WSI support on physical device 0\n");
	}

	// Select Surface Format
	constexpr VkFormat RequestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
	constexpr VkColorSpaceKHR RequestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	ImGuiWindow.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(PhysicalDevice, ImGuiWindow.Surface, RequestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(RequestSurfaceImageFormat), RequestSurfaceColorSpace);

	// Select Present Mode
	#ifdef IMGUI_UNLIMITED_FRAME_RATE
	VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
	constexpr VkPresentModeKHR PresentModes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
	ImGuiWindow.PresentMode = ImGui_ImplVulkanH_SelectPresentMode(PhysicalDevice, ImGuiWindow.Surface, &PresentModes[0], IM_ARRAYSIZE(PresentModes));
	//printf("[vulkan] Selected PresentMode = %d\n", ImGuiWindow.PresentMode);

	// Create SwapChain, RenderPass, Framebuffer, etc.
	ASSERT(MinImageCount >= 2, "MinImageCount must be >= 2");
	ImGui_ImplVulkanH_CreateOrResizeWindow(Instance, PhysicalDevice, Device, &ImGuiWindow, QueueFamily, Allocator, Width, Height, MinImageCount);
}


std::string CVulkanAPI::GetGraphicsDisplayName()
{
	//TODO: Implement
	return "";
}

bool CVulkanAPI::GenerateTexture(TAssetObjectPointer<CTexture> Texture, bool bAA)
{
	//TODO: Implement
	return false;
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

TSharedPointer<IFramebuffer> CVulkanAPI::CreateFramebuffer(const SFramebufferSpecification& Specification)
{
	return CreatePointer<CVkFramebuffer>(Specification);
}

unsigned CVulkanAPI::CreateVertexBuffer(const CMesh& MeshData)
{
	//TODO: Implement
	return -1;
}

void CVulkanAPI::BindVertexArray(const std::vector<float>& vertices, const std::vector<uint32_t>& indices,
	unsigned& vao)
{
	//TODO: Implement
}

void CVulkanAPI::RenderMesh(ISceneVisual* SceneVisual)
{
	//TODO: Implement
}

void CVulkanAPI::CleanupMesh(ISceneVisual* SceneVisual)
{
	//TODO: Implement
}

void CVulkanAPI::ApplyMVP(uint32_t Program, Matrix4 View, Matrix4 Projection, STransform Transform)
{
	//TODO: Implement
}

void CVulkanAPI::RenderImGui()
{
	const VkSemaphore ImageAcquiredSemaphore  = ImGuiWindow.FrameSemaphores[ImGuiWindow.SemaphoreIndex].ImageAcquiredSemaphore;
	const VkSemaphore RenderCompleteSemaphore = ImGuiWindow.FrameSemaphores[ImGuiWindow.SemaphoreIndex].RenderCompleteSemaphore;
    VkResult Err = vkAcquireNextImageKHR(Device, ImGuiWindow.Swapchain, UINT64_MAX, ImageAcquiredSemaphore,
                                         VK_NULL_HANDLE, &ImGuiWindow.FrameIndex);
    if (Err == VK_ERROR_OUT_OF_DATE_KHR || Err == VK_SUBOPTIMAL_KHR)
    {
    	// TODO: Handle swap chain rebuild
        ENSURE(false, "vkAcquireNextImageKHR failed");
        return;
    }
    VKCheck(Err);

	const ImGui_ImplVulkanH_Frame* Frame = &ImGuiWindow.Frames[ImGuiWindow.FrameIndex];
    {
        Err = vkWaitForFences(Device, 1, &Frame->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
        VKCheck(Err);

        Err = vkResetFences(Device, 1, &Frame->Fence);
        VKCheck(Err);
    }
    {
        Err = vkResetCommandPool(Device, Frame->CommandPool, 0);
        VKCheck(Err);
        VkCommandBufferBeginInfo Info = {};
        Info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        Info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        Err = vkBeginCommandBuffer(Frame->CommandBuffer, &Info);
        VKCheck(Err);
    }
    {
        VkRenderPassBeginInfo Info = {};
        Info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        Info.renderPass = ImGuiWindow.RenderPass;
        Info.framebuffer = Frame->Framebuffer;
        Info.renderArea.extent.width = ImGuiWindow.Width;
        Info.renderArea.extent.height = ImGuiWindow.Height;
        Info.clearValueCount = 1;
        Info.pClearValues = &ImGuiWindow.ClearValue;
        vkCmdBeginRenderPass(Frame->CommandBuffer, &Info, VK_SUBPASS_CONTENTS_INLINE);
    }

    // Record dear imgui primitives into command buffer
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), Frame->CommandBuffer);

    // Submit command buffer
    vkCmdEndRenderPass(Frame->CommandBuffer);
    {
        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo Info = {};
        Info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        Info.waitSemaphoreCount = 1;
        Info.pWaitSemaphores = &ImageAcquiredSemaphore;
        Info.pWaitDstStageMask = &wait_stage;
        Info.commandBufferCount = 1;
        Info.pCommandBuffers = &Frame->CommandBuffer;
        Info.signalSemaphoreCount = 1;
        Info.pSignalSemaphores = &RenderCompleteSemaphore;

        Err = vkEndCommandBuffer(Frame->CommandBuffer);
        VKCheck(Err);
        Err = vkQueueSubmit(Queue, 1, &Info, Frame->Fence);
        VKCheck(Err);
    }
}

void CVulkanAPI::ImGuiNewFrame()
{
	ImGui_ImplVulkan_NewFrame();
}

void CVulkanAPI::SwapBuffers()
{
	const VkSemaphore RenderCompleteSemaphore = ImGuiWindow.FrameSemaphores[ImGuiWindow.SemaphoreIndex].RenderCompleteSemaphore;
	VkPresentInfoKHR Info = {};
	Info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	Info.waitSemaphoreCount = 1;
	Info.pWaitSemaphores = &RenderCompleteSemaphore;
	Info.swapchainCount = 1;
	Info.pSwapchains = &ImGuiWindow.Swapchain;
	Info.pImageIndices = &ImGuiWindow.FrameIndex;
	VkResult Err = vkQueuePresentKHR(Queue, &Info);
	if (Err == VK_ERROR_OUT_OF_DATE_KHR || Err == VK_SUBOPTIMAL_KHR)
	{
		ENSURE(false, "vkQueuePresentKHR failed");
		return;
	}
	VKCheck(Err);
	ImGuiWindow.SemaphoreIndex = (ImGuiWindow.SemaphoreIndex + 1) % ImGuiWindow.ImageCount; // Now we can use the next set of semaphores
}

void CVulkanAPI::RenderLines(ISceneVisual* SceneVisual, float Thickness)
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

void CVulkanAPI::ApplyMaterialFlags(TSharedPointer<CMaterialInterface> InMaterial)
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

void CVulkanAPI::PassAttributeToShader(int32_t ShaderLocation, TAssetObjectPointer<CTexture> Attribute)
{
	//TODO: Implement
}
