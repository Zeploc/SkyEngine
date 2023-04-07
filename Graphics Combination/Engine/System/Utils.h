// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// Library Includes //
#include <istream>
#include <memory>
#include <string>
#include <vector>

// OpenGL Library Includes //
#include <glm/gtx/string_cast.hpp>

// Forward Declarations //
class Entity;

class Utils
{
public:
	struct AnimInfo
	{
		glm::vec2 v2FrameSize;
		glm::vec2 v2FrameCount;
		glm::vec2 v2CurrentFrame = {0, 0};
		glm::vec2 v2StartFrame = {0, 0};
		glm::vec2 v2EndFrame = {0, 0};
		int iFPS = 30;

		void Advance()
		{
			v2CurrentFrame.x++;
			if (v2CurrentFrame.x > v2FrameCount.x || v2CurrentFrame.x > v2EndFrame.x)
			{
				v2CurrentFrame.x = 0;
				v2CurrentFrame.y++;
				if (v2CurrentFrame.y > v2FrameCount.y || v2CurrentFrame.y > v2EndFrame.y)
				{
					v2CurrentFrame = v2StartFrame;
				}
			}
		}
	};

	using Col2DI = struct CollisionInfo2D
	{
		float fHeight;
		float fWidth;
		glm::vec2 v2Offset = glm::vec2();
	};

	struct Transform
	{
		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;

		std::string ToString()
		{
			std::string sTransform;
			sTransform += to_string(Position) + " ";
			sTransform += to_string(Rotation) + " ";
			sTransform += to_string(Scale);
			return sTransform;
		}

		friend std::istream& operator>>(std::istream& is, Transform& transform)
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

		void FromString(std::string sTransform)
		{
			std::vector<std::string> Seperated = SeparateString(sTransform, ' ');
			if (Seperated.size() >= 3)
			{
				Position = StringToVec3(Seperated[0]);
				Rotation = StringToVec3(Seperated[1]);
				Scale = StringToVec3(Seperated[2]);
			}
		}
	};

	enum EANCHOR
	{
		TOP_LEFT,
		TOP_CENTER,
		TOP_RIGHT,
		CENTER_LEFT,
		CENTER,
		CENTER_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_CENTER,
		BOTTOM_RIGHT
	};

	enum EMESHTYPE
	{
		NONE,
		LINE,
		TRIANGLE,
		PLANE,
		PYRAMID,
		CUBE,
		SPHERE,
		MODEL,
		GEOMETRY
	};

	static glm::vec3 LinePlaneIntersect(glm::vec3 RayStart, glm::vec3 RayDirection, glm::vec3 PlanePos, glm::vec3 PlaneNormal);

	static glm::vec3 GetAncoredPosition(glm::vec3 position, glm::vec3 Dimensions, EANCHOR _AnchorType);

	static glm::vec3 GetAncoredPosition2D(glm::vec2 position, glm::vec2 Dimensions, EANCHOR _AnchorType);

	static glm::vec3 GetTextAncoredPosition(glm::vec2 position, glm::vec2 Dimensions, EANCHOR _AnchorType);

	static bool isColliding2D(std::shared_ptr<Entity> Entity1, std::shared_ptr<Entity> Entity2);

	//static bool isColliding2D(Entity* Entity1, std::shared_ptr<Entity> Entity2);

	static bool CheckCollision2D(std::shared_ptr<Entity> Entity1, std::shared_ptr<Entity> Entity2, glm::vec2 Movement);

	//static bool CheckCollision2D(Entity* Entity1, std::shared_ptr<Entity> Entity2, glm::vec2 Movement);

	static glm::vec2 GetDistance2D(std::shared_ptr<Entity> Entity1, std::shared_ptr<Entity> Entity2);

	//static glm::vec2 GetDistance2D(Entity* Entity1, std::shared_ptr<Entity> Entity2);
	//static glm::vec2 FindTextDimenions(std::string text);

	static glm::vec2 GetDifference2D(std::shared_ptr<Entity> Entity1, std::shared_ptr<Entity> Entity2);

	static bool CheckHit(glm::vec3 RayStart, glm::vec3 RayDirection, std::shared_ptr<Entity> EntityCheck, glm::vec3& HitPos);

	static int AddEntityID();

	static std::vector<std::string> SeparateString(std::string _string, char _seperator);

	static glm::vec3 StringToVec3(std::string _string);

	static std::shared_ptr<Entity> WorldCubeMap;

private:
	static int iEntityNumber;

	static bool CheckSphereEntityHit(glm::vec3 RayStart, glm::vec3 RayDirection, std::shared_ptr<Entity> EntityCheck, glm::vec3& HitPos);

	static bool CheckCubeEntityHit(glm::vec3 RayStart, glm::vec3 RayDirection, std::shared_ptr<Entity> EntityCheck, glm::vec3& HitPos);

	static bool CheckPlaneEntityHit(glm::vec3 RayStart, glm::vec3 RayDirection, std::shared_ptr<Entity> EntityCheck, glm::vec3& HitPos);

	static bool CheckFaceHit(glm::vec3 BottomLeftOffset, glm::vec3 TopRightOffset, glm::vec3 RayStart, glm::vec3 RayDirection, std::shared_ptr<Entity> EntityCheck, glm::vec3& HitPos);
};
