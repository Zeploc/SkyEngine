// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <glew/glew.h>

#include "Graphics/Framebuffer.h"

class GLFramebuffer : public IFramebuffer
{
public:
	explicit GLFramebuffer(const SFramebufferSpecification& Specification);
	~GLFramebuffer() override;

	/* Implies needs updating */
	void Invalidate();
	const SFramebufferSpecification& GetSpecification() const override { return Specification; }
	void Bind() override;
	void Unbind() override;
	uint32_t GetColorAttachmentRendererID() const override { return ColorAttachment; }


private:
	SFramebufferSpecification Specification;
	uint32_t RendererID;
	/* Manual for now */
	uint32_t ColorAttachment;
	uint32_t DepthAttachment;
};
