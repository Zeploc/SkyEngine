#include "SEPCH.h"
#include "GLFramebuffer.h"

#include <glew/glew.h>

#include "Core/Application.h"
#include "Render/Renderer.h"
#include "Platform/Window/EngineWindow.h"

GLFramebuffer::GLFramebuffer(const SFramebufferSpecification& Specification)
: Specification(Specification)
{
}

GLFramebuffer::~GLFramebuffer()
{
	glDeleteFramebuffers(1, &RendererID);
}

void GLFramebuffer::Invalidate()
{
	if (RendererID)
	{
		glDeleteFramebuffers(1, &RendererID);
		glDeleteTextures(1, &ColorAttachment);
		glDeleteTextures(1, &ColorAttachment);
	}
	
	glCreateFramebuffers(1, &RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, RendererID);
		
	glCreateTextures(GL_TEXTURE_2D, 1, &ColorAttachment);
	glBindTexture(GL_TEXTURE_2D, ColorAttachment);
	/* Tex image can be sampled in shader, texture storage can be used if that's not needed*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)Specification.Size.X, (GLsizei)Specification.Size.Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	// Down scaling and upscaling interpretation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorAttachment, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &DepthAttachment);
	glBindTexture(GL_TEXTURE_2D, DepthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, (GLsizei)Specification.Size.X, (GLsizei)Specification.Size.Y);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, (float)Specification.Size.X, (float)Specification.Size.Y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, DepthAttachment, 0);

	const bool bBufferComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	ASSERT(bBufferComplete, "Frame buffer did not complete!");
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

void GLFramebuffer::UpdateSize(SVector2ui NewSize)
{
	Specification.Size = NewSize;
	Invalidate();
}

void GLFramebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, RendererID);
}

void GLFramebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
