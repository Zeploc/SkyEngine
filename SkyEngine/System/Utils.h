// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Math/Vector.h"

// Library Includes //

// OpenGL Library Includes //
#include <glm/gtx/string_cast.hpp>

#include "EnumTypes.h"
#include "Render/Meshes/MeshData.h"

struct STransform;
// Forward Declarations //
class Entity;

// TODO: Warnings with exporting class containing STDL
#pragma warning (disable : 4251)

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

	static SVector LinePlaneIntersect(SVector RayStart, SVector RayDirection, SVector PlanePos, SVector PlaneNormal);

	static glm::vec3 GetAncoredPosition(SVector position, SVector Dimensions, EANCHOR _AnchorType);

	static glm::vec3 GetAncoredPosition2D(glm::vec2 position, glm::vec2 Dimensions, EANCHOR _AnchorType);

	static glm::vec3 GetTextAncoredPosition(glm::vec2 position, glm::vec2 Dimensions, EANCHOR _AnchorType);

	static bool isColliding2D(THardPointer<Entity> Entity1, THardPointer<Entity> Entity2);

	//static bool isColliding2D(Entity* Entity1, Pointer<Entity> Entity2);

	static bool CheckCollision2D(THardPointer<Entity> Entity1, THardPointer<Entity> Entity2, glm::vec2 Movement);

	//static bool CheckCollision2D(Entity* Entity1, Pointer<Entity> Entity2, glm::vec2 Movement);

	static glm::vec2 GetDistance2D(THardPointer<Entity> Entity1, THardPointer<Entity> Entity2);

	//static glm::vec2 GetDistance2D(Entity* Entity1, Pointer<Entity> Entity2);
	//static glm::vec2 FindTextDimenions(std::string text);

	static glm::vec2 GetDifference2D(THardPointer<Entity> Entity1, THardPointer<Entity> Entity2);

	static std::vector<std::string> SeparateString(std::string _string, char _seperator);

	// TODO: Convert to type from string
	static SVector StringToVec3(std::string _string);
	static SRotator StringToRotator(std::string _string);

	static bool CheckMeshHit(STransform MeshTransform, const CMeshData& MeshData, SVector RayStart, SVector RayDirection, SVector& HitPos);
	
	static bool CheckFaceHit(SVector BottomLeftOffset, SVector TopRightOffset, SVector RayStart, SVector RayDirection, THardPointer<Entity> EntityCheck, SVector& HitPos);

	static bool CheckSphereHit(SVector RayStart, SVector RayDirection, SVector SphereCenter, float SphereRadius, SVector& HitPos);

	static bool CheckCubeHit(SVector RayStart, SVector RayDirection, SVector CubeDimensions, THardPointer<Entity> EntityCheck, SVector& HitPos);

	static bool CheckPlaneEntityHit(SVector RayStart, SVector RayDirection, THardPointer<Entity> EntityCheck, SVector& HitPos);
	
	static THardPointer<Entity> WorldCubeMap;

	// TODO: Move to custom array once setups
	/* Check if an element exists in a array */
	template<typename T>
	static bool ArrayContains(TArray<T> Array, T Element);
	/* Check if an element exists in a array */
	template<typename T>
	static int FindInArray(TArray<T> Array, T Element);
	/* Check if an element exists in a array */
	template<typename T>
	static bool RemoveFromArray(std::vector<T>& Array, T Element);

	/* Implicit convert the type of the array to a given type */
	template<typename T, typename U>
	static TArray<T> ArrayConvert(const TArray<U>& Array);
	
private:


};

template <typename T>
bool Utils::ArrayContains(TArray<T> Array, T Element)
{
	return std::find(Array.begin(), Array.end(), Element) != Array.end();
}

template <typename T>
int Utils::FindInArray(TArray<T> Array, T Element)
{
	for (auto it = Array.begin(); it != Array.end(); ++it)
	{
		if (*it == Element)
		{
			return it;
		}
	}
	return -1;
}

template <typename T>
bool Utils::RemoveFromArray(TArray<T>& Array, T Element)
{
	for (auto it = Array.begin(); it != Array.end(); ++it)
	{
		if (*it == Element)
		{
			Array.erase(it);
			return true;
		}
	}
	return false;
}

template <typename T, typename U>
TArray<T> Utils::ArrayConvert(const TArray<U>& Array)
{
	TArray<T> NewArray;
	for (const U& CurrentAsset : Array)
	{
		NewArray.push_back(CurrentAsset);
	}
	return NewArray;
}

