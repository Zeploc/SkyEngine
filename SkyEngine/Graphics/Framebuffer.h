// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Math/Vector2.h"

struct SFramebufferSpecification
{
	SVector2ui Size;
	uint32_t Samples = 1;

	/* Will render to screen (not an offscreen buffer) */
	bool bSwapChainTarget = false;

	// TODO: Format (int/float), Alpha, 8 vs 16, etc
};

class IFramebuffer
{
public:
	virtual ~IFramebuffer() = default;
	virtual const SFramebufferSpecification& GetSpecification() const = 0;
	virtual uint32_t GetColorAttachmentRendererID() const = 0;

	/* Implies needs updating */
	virtual void Invalidate()  = 0;
	
	/* Remove later to renderer */
	virtual void Bind() = 0;
	virtual void Unbind() = 0;

	/* Creates a frame buffer with the current graphics API*/
	static TPointer<IFramebuffer> Create(const SFramebufferSpecification& Specification);
};
