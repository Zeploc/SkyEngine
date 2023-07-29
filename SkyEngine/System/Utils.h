// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Math/Vector.h"

// Library Includes //
#include <memory>
#include <string>
#include <vector>

// OpenGL Library Includes //
#include <glm/gtx/string_cast.hpp>

#include "EnumTypes.h"

// Forward Declarations //
class Entity;

class ENGINE_API Utils
{
public:
	// TODO: Move structs and enums out of class
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

	static Vector3 LinePlaneIntersect(Vector3 RayStart, Vector3 RayDirection, Vector3 PlanePos, Vector3 PlaneNormal);

	static glm::vec3 GetAncoredPosition(Vector3 position, Vector3 Dimensions, EANCHOR _AnchorType);

	static glm::vec3 GetAncoredPosition2D(glm::vec2 position, glm::vec2 Dimensions, EANCHOR _AnchorType);

	static glm::vec3 GetTextAncoredPosition(glm::vec2 position, glm::vec2 Dimensions, EANCHOR _AnchorType);

	static bool isColliding2D(Pointer<Entity> Entity1, Pointer<Entity> Entity2);

	//static bool isColliding2D(Entity* Entity1, Pointer<Entity> Entity2);

	static bool CheckCollision2D(Pointer<Entity> Entity1, Pointer<Entity> Entity2, glm::vec2 Movement);

	//static bool CheckCollision2D(Entity* Entity1, Pointer<Entity> Entity2, glm::vec2 Movement);

	static glm::vec2 GetDistance2D(Pointer<Entity> Entity1, Pointer<Entity> Entity2);

	//static glm::vec2 GetDistance2D(Entity* Entity1, Pointer<Entity> Entity2);
	//static glm::vec2 FindTextDimenions(std::string text);

	static glm::vec2 GetDifference2D(Pointer<Entity> Entity1, Pointer<Entity> Entity2);

	static int AddEntityID();

	static std::vector<std::string> SeparateString(std::string _string, char _seperator);

	// TODO: Convert to type from string
	static Vector3 StringToVec3(std::string _string);
	static Rotator StringToRotator(std::string _string);

	static bool CheckFaceHit(Vector3 BottomLeftOffset, Vector3 TopRightOffset, Vector3 RayStart, Vector3 RayDirection, Pointer<Entity> EntityCheck, Vector3& HitPos);

	static bool CheckSphereHit(Vector3 RayStart, Vector3 RayDirection, Vector3 SphereCenter, float SphereRadius, Vector3& HitPos);

	static bool CheckCubeHit(Vector3 RayStart, Vector3 RayDirection, Vector3 CubeDimensions, Pointer<Entity> EntityCheck, Vector3& HitPos);

	static bool CheckPlaneEntityHit(Vector3 RayStart, Vector3 RayDirection, Pointer<Entity> EntityCheck, Vector3& HitPos);
	
	static Pointer<Entity> WorldCubeMap;

private:
	static int iEntityNumber;


};
