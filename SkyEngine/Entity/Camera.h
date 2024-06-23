// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Entity.h"
#include "Core/Core.h"

enum class EProjectionMode
{
	Orthographic = 1,
	Perspective
};

class ENGINE_API Camera : public Entity
{
public:
	Camera(const STransform& InTransform);
	std::string GetEntityClassName() override;
	static std::string GetStaticName();

	EProjectionMode ProjectionMode = EProjectionMode::Perspective;
	Matrix4 Projection;
	float FOV = 45.0f;
	
	// TODO: Check only for orthographic or apply to perspective
	float WindowScale = 200;
	
	float MaxViewClipping = 2000.0f;

protected:
	void Serialize(std::ostream& os) override;
	void Deserialize(std::istream& is) override;
};
