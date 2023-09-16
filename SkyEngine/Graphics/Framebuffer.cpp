// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Framebuffer.h"

#include "Core/Application.h"

TPointer<IFramebuffer> IFramebuffer::Create(const SFramebufferSpecification& Specification)
{
	return GetGraphicsAPI()->CreateFramebuffer(Specification);
}
