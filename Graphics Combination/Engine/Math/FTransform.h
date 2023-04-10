// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Vector.h"
#include <string>

struct FTransform
{
	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;

	std::string ToString();
	void FromString(std::string sTransform);

	friend std::istream& operator>>(std::istream& is, FTransform& transform)
	{
		int x, y, z, roll, yaw, pitch, sx, sy, sz;
		std::string PosX, PosY, PosZ, Roll, Yaw, Pitch, ScaleX, ScaleY, ScaleZ;
		is >> PosX >> PosY >> PosZ >> Roll >> Yaw >> Pitch >> ScaleX >> ScaleY >> ScaleZ;
		x = std::stoi(PosX.substr(5));
		y = std::stoi(PosY);
		z = std::stoi(PosZ);
		roll = std::stoi(Roll.substr(5));
		yaw = std::stoi(Yaw);
		pitch = std::stoi(Pitch);
		sx = std::stoi(ScaleX.substr(5));
		sy = std::stoi(ScaleY);
		sz = std::stoi(ScaleZ);
		transform.Position = glm::vec3(x, y, z);
		transform.Rotation = glm::vec3(roll, yaw, pitch);
		transform.Scale = glm::vec3(sx, sy, sz);
		return is;
	}

};
