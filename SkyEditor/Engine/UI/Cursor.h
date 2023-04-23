// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Engine Includes //
#include "UIImage.h"

class Cursor : public UIImage
{
public:
	Cursor(const char* _CursorImage);

	~Cursor();

	void Update() override;

	void SetInvisibleRange(glm::vec2 _v2NewRange);

	void SetVisibleRange(glm::vec2 _v2NewRange);

	glm::vec2 m_v2InvisibleRange = {0, 0};
	glm::vec2 m_v2VisibleRange = {0, 0};
};
