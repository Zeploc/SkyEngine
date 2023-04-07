// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// OpenGL Library Includes //
#include <glew.h>

// Engine Includes //
#include "UIElement.h"
#include "Engine/System/Utils.h"

class UIImage : public UIElement
{
public:
	UIImage(glm::vec2 _Position, Utils::EANCHOR _anchor, float _fRotation, glm::vec4 _Colour, int _iWidth, int _iHeight);

	UIImage(glm::vec2 _Position, Utils::EANCHOR _anchor, float _fRotation, glm::vec4 _Colour, int _iWidth, int _iHeight, const char* TextureSource, int _DrawMode);

	~UIImage();

	void DrawUIElement() override;

	Utils::EANCHOR GetAnchor() { return ImageAnchor; };
	int GetWidth() { return iWidth; };
	int GetHeight() { return iHeight; };

	static glm::vec2 GetPositionFromAnchor(glm::vec2 _Position, Utils::EANCHOR _anchor, int _iWidth, int _iHeight);

private:
	GLuint VAO;
	GLuint texture;

	bool bHasTexture = false;
	Utils::EANCHOR ImageAnchor;
	int iWidth;
	int iHeight;
};
