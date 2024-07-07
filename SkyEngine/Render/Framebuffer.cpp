// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Framebuffer.h"

#include "Core/Application.h"

THardPointer<IFramebuffer> IFramebuffer::Create(const SFramebufferSpecification& Specification)
{
	THardPointer<IFramebuffer> Framebuffer = GetGraphicsAPI()->CreateFramebuffer(Specification);	
	Framebuffer->Invalidate();
	return Framebuffer;
}
