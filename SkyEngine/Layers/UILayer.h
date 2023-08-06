// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Layers/Layer.h"

class CUILayer : public CLayer
{
public:
	CUILayer();
	virtual ~CUILayer();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnRender() override;

protected:
};
