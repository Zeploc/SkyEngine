// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Mesh.h"

class FrameBuffer : public Mesh
{
public:
	FrameBuffer();

	~FrameBuffer();

	void BindFrameBuffer();

	void Rebind() override;

	void Render();

	GLuint renderTexture;
	GLuint framebuffer;
};
