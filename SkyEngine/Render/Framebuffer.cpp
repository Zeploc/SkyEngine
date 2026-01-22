// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Framebuffer.h"

#include "Core/Application.h"

TSharedPointer<IFramebuffer> IFramebuffer::Create(const SFramebufferSpecification& Specification)
{
	TSharedPointer<IFramebuffer> Framebuffer = GetGraphicsAPI()->CreateFramebuffer(Specification);	
	Framebuffer->Invalidate();
	return Framebuffer;
}
