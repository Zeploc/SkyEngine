#include "SEPCH.h"
#include "VkFramebuffer.h"

CVkFramebuffer::CVkFramebuffer(const SFramebufferSpecification& Specification)
	: Specification(Specification)
{
	
}

CVkFramebuffer::~CVkFramebuffer()
{
	
}

const SFramebufferSpecification& CVkFramebuffer::GetSpecification() const
{
	return Specification;
}

uint32_t CVkFramebuffer::GetColorAttachmentRendererID() const
{
	return -1;
}

void CVkFramebuffer::Invalidate()
{
	// TODO: Implement
}

void CVkFramebuffer::UpdateSize(SVector2ui NewSize)
{
	Specification.Size = NewSize;
	Invalidate();
}

void CVkFramebuffer::Bind()
{
	// TODO: Implement / refactor? How does this work in vulkan??
}

void CVkFramebuffer::Unbind()
{
	// TODO: Implement / refactor? How does this work in vulkan??
}
