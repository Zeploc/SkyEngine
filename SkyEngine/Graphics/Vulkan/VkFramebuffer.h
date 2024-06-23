// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Render/Framebuffer.h"

class CVkFramebuffer : public IFramebuffer
{
public:
	CVkFramebuffer(const SFramebufferSpecification& Specification);
	~CVkFramebuffer() override;
	
	const SFramebufferSpecification& GetSpecification() const override;
	uint32_t GetColorAttachmentRendererID() const override;
	void Invalidate() override;
	void UpdateSize(SVector2ui NewSize) override;
	void Bind() override;
	void Unbind() override;

private:
	SFramebufferSpecification Specification;
};
