#include "SEPCH.h"
#include "GLFramebuffer.h"

#include <glew/glew.h>

GLFramebuffer::GLFramebuffer(const SFramebufferSpecification& Specification)
: Specification(Specification)
{
	Invalidate();
}

GLFramebuffer::~GLFramebuffer()
{
	glDeleteFramebuffers(1, &RendererID);
}

void GLFramebuffer::Invalidate()
{
	glCreateFramebuffers(1, &RendererID);
	glBindFramebuffer(GL_TEXTURE_2D, RendererID);
		
	glCreateTextures(GL_TEXTURE_2D, 1, &ColorAttachment);
	/* Tex image can be sampled in shader, texture storage can be used if that's not needed*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)Specification.Size.X, (GLsizei)Specification.Size.Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	// Down scaling and upscaling interpretation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &DepthAttachment);
	glBindTexture(GL_TEXTURE_2D, DepthAttachment);
	glTextureStorage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, (GLsizei)Specification.Size.X, (GLsizei)Specification.Size.Y);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, (float)Specification.Widget, (float)Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
	
	glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0);

	const bool bBufferComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	ASSERT(bBufferComplete, "Frame buffer did not complete!");
	
	glBindFramebuffer(GL_TEXTURE_2D, 0);
}

void GLFramebuffer::Bind()
{
	glBindFramebuffer(GL_TEXTURE_2D, RendererID);
}

void GLFramebuffer::Unbind()
{
	glBindFramebuffer(GL_TEXTURE_2D, 0);
}
