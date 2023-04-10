// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Engine Includes //
#include "UIButton.h"
#include "UIElement.h"
#include "UIImage.h"

class UISlider : public UIElement
{
public:
	UISlider(glm::vec2 _Position, float _fRotation, glm::vec4 _HandleColour, glm::vec4 _SliderColour, int _iSliderLength, int _iSliderThickness, int _iHandleHeight, int _iHandleWidth, EANCHOR _Anchor, std::string _sTitle);

	~UISlider();

	void DrawUIElement() override;

	void Update() override;

	void SetActive(bool _bIsActive) override;

	float GetValue();

	UIButton SliderBar;
	UIButton SliderHandle;

	std::string sTitle;

	float GetMinimumPosition() { return fMinimumPosition; };

	void SetMinimumPosition(float _fMinimumPosition);

	void SetMaximumPosition(float _fMaximumPosition);

	void SetStartPosition(float _fStartPosition);

	void SetLockSize(float _fLockSize);

private:
	float fMinimumPosition = 0.0f;
	float fMaximumPosition = 1.0f;
	float fLockSize = 0.01f;
};
