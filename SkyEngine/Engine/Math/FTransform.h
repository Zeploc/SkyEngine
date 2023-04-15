// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Rotator.h"
#include "Vector.h"
#include <string>
#include <sstream>


struct FTransform
{
	Vector3 Position;
	Rotator Rotation;
	Vector3 Scale;

	std::string ToString();
	void FromString(std::string sTransform);

	friend std::istream& operator>>(std::istream& is, FTransform& transform)
	{
		// TODO:
		float x, y, z, roll, yaw, pitch, sx, sy, sz;
		std::string PosX, PosY, PosZ, Roll, Pitch, Yaw, ScaleX, ScaleY, ScaleZ;
		is >> PosX >> PosY >> PosZ >> Pitch >> Yaw >> Roll >> ScaleX >> ScaleY >> ScaleZ;
		x = std::stof(PosX.substr(1, PosX.length() - 1));
		y = std::stof(PosY.substr(0, PosY.length() - 1));
		z = std::stof(PosZ.substr(0, PosZ.length() - 1));
		pitch = std::stof(Pitch.substr(1, Pitch.length() - 1));
		yaw = std::stof(Yaw.substr(0, Yaw.length() - 1));
		roll = std::stof(Roll.substr(0, Roll.length() - 1));
		sx = std::stof(ScaleX.substr(1, ScaleX.length() - 1));
		sy = std::stof(ScaleY.substr(0, ScaleY.length() - 1));
		sz = std::stof(ScaleZ.substr(0, ScaleZ.length() - 1));
		transform.Position = Vector3(x, y, z);
		transform.Rotation = Rotator(pitch, yaw, roll);
		transform.Scale = Vector3(sx, sy, sz);
		return is;
	}

};
