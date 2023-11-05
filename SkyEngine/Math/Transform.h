// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Rotator.h"
#include "Vector.h"

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

struct ENGINE_API STransform
{
	STransform()
	{
		Scale = SVector(1.0f);
	}
	
	STransform(SVector InPosition, SRotator InRotation, SVector InScale)
	: Position(InPosition), Rotation(InRotation), Scale(InScale)
	{}
	
	STransform(SVector InPosition, SRotator InRotation)
	: Position(InPosition), Rotation(InRotation)
	{}
	
	SVector Position;
	SRotator Rotation;
	SVector Scale;
	
	bool operator==(const STransform& other) const
	{
		return (Position == other.Position &&
			Rotation == other.Rotation &&
			Scale == other.Scale);
	}
	bool operator!=(const STransform& other) const
	{
		return (Position != other.Position ||
			Rotation != other.Rotation ||
			Scale != other.Scale);
	}
	bool operator==(STransform& other) const
	{
		return (Position == other.Position &&
			Rotation == other.Rotation &&
			Scale == other.Scale);
	}
	bool operator!=(STransform& other) const
	{
		return (Position != other.Position ||
			Rotation != other.Rotation ||
			Scale != other.Scale);
	}

	std::string ToString() const;
	bool FromMatrix(glm::mat4 Matrix);
	void FromString(std::string sTransform);
	
	glm::mat4 GetModelMatrix() const;
	
	friend std::ostream& operator<<(std::ostream& os, const STransform& InTransform);
	friend std::istream& operator>>(std::istream& is, STransform& OutTransform);
};
