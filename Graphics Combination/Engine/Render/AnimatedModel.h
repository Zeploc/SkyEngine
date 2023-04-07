// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

// This Includes //
#include "Mesh.h"

// Engine Includes //
#include "Engine/System/Math_3d.h"

// Library Includes //
#include <vector>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
// Note: Used for ASSIMP_LOAD_FLAGS
#include <assimp/postprocess.h>

#define ZERO_MEM(a) memset(a, 0, sizeof(a))

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)
#define GLCheckError() (glGetError() == GL_NO_ERROR)

class AnimatedModel : public Mesh
{
public:
	AnimatedModel(std::string modelFilename, std::string texFilename);

	~AnimatedModel();

	void Render(Utils::Transform Newtransform) override;

	void Update() override;

	void Rebind() override;

private:
#define INVALID_MATERIAL 0xFFFFFFFF
#define NUM_BONES_PER_VERTEX 4

	enum VB_TYPES
	{
		INDEX_BUFFER,
		POS_VB,
		NORMAL_VB,
		TEXCOORD_VB,
		BONE_VB,
		NUM_VBs
	};

	struct MeshEntry
	{
		MeshEntry()
		{
			NumIndices = 0;
			BaseVertex = 0;
			BaseIndex = 0;
			MaterialIndex = INVALID_MATERIAL;
		}

		GLuint NumIndices;
		GLuint BaseVertex;
		GLuint BaseIndex;
		GLuint MaterialIndex;
	};

	struct BoneInfo
	{
		Matrix4f BoneOffset;
		Matrix4f FinalTransformation;

		BoneInfo()
		{
			BoneOffset.SetZero(); // = glm::mat4(); //-->setzero
			FinalTransformation.SetZero(); // = glm::mat4();  //--> setzero
		}
	};

	struct VertexBoneData
	{
		int IDs[NUM_BONES_PER_VERTEX];
		float Weights[NUM_BONES_PER_VERTEX];

		VertexBoneData()
		{
			reset();
		}

		void reset()
		{
			ZERO_MEM(IDs);
			ZERO_MEM(Weights);
		}

		int boneID;
		float weight;

		void addBoneData(int BoneID, float Weight);
	};

	Assimp::Importer m_Importer;
	const aiScene* m_pScene;

	// Binding
	void Clear();

	void BindMesh(std::string fileName);

	bool InitFromScene(const aiScene* pScene, std::string Filename);

	void initMesh(GLuint meshIndex, const aiMesh* paiMesh, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals,
	              std::vector<glm::vec2>& texcoords, std::vector<VertexBoneData>& bones, std::vector<GLuint>& indices);

	void loadBones(int meshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& bones);

	void setShaderEffectVariables();

	void boneTransforms(std::vector<Matrix4f>& transforms);

	void setCurrentAnimation(int startFrameNum, int endFramNum);

	bool initMaterials(const aiScene* pScene, std::string filename);

	GLuint loadTexture(std::string texFileName);

	// Bone
	GLuint m_Buffers[NUM_VBs];
	std::map<std::string, GLuint> m_BoneMapping;
	std::map<std::string, aiNodeAnim*> boneHierarchy;
	std::vector<BoneInfo> m_BoneInfo;
	GLuint m_NumBones;

	std::vector<MeshEntry> m_Entries;
	std::vector<GLuint> m_Textures;

	Matrix4f m_GlobalInverseTransform;
	GLuint m_boneLocation[100];

	// Model
	LightInfo ModelLightInfo;

	// Animation
	float animTick;
	int startFrame;
	int endFrame;
	int currentFrame;
	float animSpeed;

	int animFps;
	float animStartTime;
	float animEndtime;

	float animationTime;

	// Animation
	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform);

	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, std::string NodeName);

	void CalcInterpolatedPosition(aiVector3D& out, float AnimationTime, const aiNodeAnim* pNodeAnim);

	GLuint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

	void CalcInterpolatedRotation(aiQuaternion& out, float AnimationTime, const aiNodeAnim* pNodeAnim);

	GLuint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);

	void CalcInterpolatedScaling(aiVector3D& out, float AnimationTime, const aiNodeAnim* pNodeAnim);

	GLuint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
};
