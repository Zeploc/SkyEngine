// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "ssAnimatedModel.h"

#include "Render/Shaders/ShaderManager.h"

#include <soil/SOIL2.h>
#include <glm/gtx/transform.hpp>

#include "Core/Application.h"
#include "Render/Shaders/Shader.h"
#include "Render/Textures/Texture.h"

#define POSITION_LOCATION	  0
#define TEX_COORD_LOCATION	  1
#define NORMAL_LOCATION		  2
#define BONE_ID_LOCATION	  3
#define BONE_WEIGHT_LOCATION  4

void ssAnimatedModel::VertexBoneData::addBoneData(int BoneID, float Weight)
{
	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++)
	{
		if (Weights[i] == 0.0)
		{
			IDs[i] = BoneID;
			Weights[i] = Weight;
			return;
		}
	}

	//assert(0); //more bones than we have space for
}

ssAnimatedModel::ssAnimatedModel(std::string modelFilname, std::string texFilename)
{
	//camera = _camera;
	//light = _light;
	//textureID = loadTexture(texFilename);
	program = ShaderManager::GetShader("AnimatedModel")->GetShaderProgram(); // _program;

	this->Transform.Scale = SVector(0.1f, 0.1f, 0.1f);
	this->Transform.Position = SVector(0.0, 0.0, 0.0);
	// TODO: Verify 1 yaw?
	this->Transform.Rotation = SRotator(1.0f, 0.0f, 0.0f);

	m_VAO = 0;
	ZERO_MEM(m_Buffers);
	m_NumBones = 0;
	m_pScene = nullptr;

	loadMesh(modelFilname);
}

ssAnimatedModel::~ssAnimatedModel()
{
	Clear();
}

bool ssAnimatedModel::loadMesh(std::string fileName)
{
	Clear();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//create buffers for vertex attributes
	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

	bool ret = false;

	m_pScene = m_Importer.ReadFile(fileName.c_str(), ASSIMP_LOAD_FLAGS);

	startFrame = 31;
	endFrame = 50;
	animFps = static_cast<int>(m_pScene->mAnimations[0]->mChannels[0]->mNumPositionKeys) / static_cast<int>(m_pScene->mAnimations[0]->mDuration);

	animStartTime = startFrame / static_cast<float>(animFps);
	animEndtime = endFrame / static_cast<float>(animFps);
	animationTime = animStartTime;
	animSpeed = 0.500f;

	animTick = 1.0f / static_cast<float>(animFps) * animSpeed;

	if (m_pScene)
	{
		m_GlobalInverseTransform = m_pScene->mRootNode->mTransformation; // scene space
		m_GlobalInverseTransform.Inverse(); //= glm::inverse(m_GlobalInverseTransform); //scene inverse space

		InitFromScene(m_pScene, fileName);
	}
	else
	{
		printf("Error parsing '%s': '%s'\n", fileName.c_str(), m_Importer.GetErrorString());
	}

	const aiAnimation* pAnimation = m_pScene->mAnimations[0];

	for (size_t i = 0; i < pAnimation->mNumChannels; i++)
	{
		aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
		std::string nodeName = pNodeAnim->mNodeName.data;

		printf("ChannelName : %s \n", nodeName.c_str());

		// stored in the m_boneinfo vector 
		if (boneHierarchy.find(nodeName) == boneHierarchy.end())
		{
			boneHierarchy[nodeName] = pNodeAnim;
		}
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);

	return ret;
}

bool ssAnimatedModel::InitFromScene(const aiScene* pScene, std::string Filename)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<VertexBoneData> bones;
	std::vector<GLuint> indices;

	GLuint numVertices = 0;
	GLuint numIndices = 0;

	//count number of vertices and indices

	for (size_t i = 0; i < m_Entries.size(); i++)
	{
		m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
		m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
		m_Entries[i].BaseVertex = numVertices;
		m_Entries[i].BaseIndex = numIndices;

		numVertices += pScene->mMeshes[i]->mNumVertices;
		numIndices += m_Entries[i].NumIndices;
	}

	//reserve space in vectors for the vertex attributes and indices
	positions.reserve(numVertices);
	normals.reserve(numVertices);
	texcoords.reserve(numVertices);
	bones.resize(numVertices);
	indices.reserve(numIndices);

	// initialize the meshes in the scene one by one
	for (uint32_t i = 0; i < (uint32_t)m_Entries.size(); i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		initMesh(i, paiMesh, positions, normals, texcoords, bones, indices);
	}

	if (!initMaterials(pScene, Filename))
	{
		return false;
	}

	//generate and populate the buffers with vertex attributes and indices
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords[0]) * texcoords.size(), &texcoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEX_COORD_LOCATION);
	glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), &bones[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(BONE_ID_LOCATION);
	glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), static_cast<const GLvoid*>(0));
	glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
	glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	return GLCheckError();
}

void ssAnimatedModel::initMesh(GLuint meshIndex,
                               const aiMesh* paiMesh,
                               std::vector<glm::vec3>& positions,
                               std::vector<glm::vec3>& normals,
                               std::vector<glm::vec2>& texcoords,
                               std::vector<VertexBoneData>& bones,
                               std::vector<GLuint>& indices)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	//populate the vertex attribute vectors
	for (size_t i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormals = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		normals.push_back(glm::vec3(pNormals->x, pNormals->y, pNormals->z));
		texcoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
	}

	//load bones 
	loadBones(meshIndex, paiMesh, bones);

	// populate the index buffer
	for (size_t i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace& face = paiMesh->mFaces[i];
		assert(face.mNumIndices == 3);

		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
}

void ssAnimatedModel::loadBones(int meshIndex,
                                const aiMesh* pMesh,
                                std::vector<VertexBoneData>& bones)
{
	for (uint32_t i = 0; i < (uint32_t)pMesh->mNumBones; i++)
	{
		int BoneIndex = 0;
		std::string BoneName(pMesh->mBones[i]->mName.data);

		//gets the initial offset matrix for each bone
		// stored in the m_boneinfo vector 
		if (m_BoneMapping.find(BoneName) == m_BoneMapping.end())
		{
			// Allocate an index for a new bone
			BoneIndex = m_NumBones;
			m_NumBones++;
			BoneInfo bi;
			m_BoneInfo.push_back(bi);
			m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix; // is the inverse bind pose matrix
			m_BoneMapping[BoneName] = BoneIndex;
		}
		else
		{
			BoneIndex = m_BoneMapping[BoneName];
		}

		//gets the bone index and bone weight of each bone
		// stored in bones vector
		for (uint32_t j = 0; j < (uint32_t)pMesh->mBones[i]->mNumWeights; j++)
		{
			const int VertexID = m_Entries[meshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
			const float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
			bones[VertexID].addBoneData(BoneIndex, Weight);
		}
	}
}

bool ssAnimatedModel::initMaterials(const aiScene* pScene, const std::string filename)
{
	// Extract the directory part from the file name
	std::string::size_type SlashIndex = filename.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos)
	{
		Dir = ".";
	}
	else if (SlashIndex == 0)
	{
		Dir = "/";
	}
	else
	{
		Dir = filename.substr(0, SlashIndex);
	}

	bool Ret = true;

	// Initialize the materials
	for (GLuint i = 0; i < pScene->mNumMaterials; i++)
	{
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_Textures[i] = nullptr;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
			{
				std::string p(Path.data);

				if (p.substr(0, 2) == ".//")
				{
					p = p.substr(2, p.size() - 2);
				}

				std::string FullPath = Dir + "/" + p;

				TAssetObjectPointer<CTexture> Texture = CTextureUtils::FindTextureBySource(FullPath.c_str());
				m_Textures[i] = Texture;

				if (!m_Textures[i]->IsValid())
				{
					printf("Error loading texture '%s'\n", FullPath.c_str());
				}
				else
				{
					printf("[Animated Model] %d - loaded texture '%s'\n", i, FullPath.c_str());
				}
			}
		}
	}

	return Ret;
}

GLuint ssAnimatedModel::loadTexture(std::string texFileName)
{
	GLuint mtexture;

	//** load texture
	glGenTextures(1, &mtexture);
	glBindTexture(GL_TEXTURE_2D, mtexture);

	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//** loadImage and create texture
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(texFileName.c_str(), &width, &height, nullptr, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);

	printf("\n [loadTexture] fileName %s \n", texFileName.c_str());

	return mtexture;
}

//movement
void ssAnimatedModel::move(float moveSpeed)
{
	currentPlayerSpeed = moveSpeed;
}

void ssAnimatedModel::rotate(float rotSpeed)
{
	currentRotationSpeed = rotSpeed;
}

void ssAnimatedModel::setShaderEffectVariables(TSharedPointer<Terrain> terrain)
{
	glUseProgram(this->program);

	// CameraManager::GetInstance()->SetMVP(Transform, program);
	
	// glm::mat4 model;
	//
	// glm::mat4 t = translate(glm::mat4(), this->Transform.Position);
	// glm::mat4 r = glm::rotate(glm::mat4(), glm::radians(Transform.Rotation.Pitch), glm::vec3(0.0f, 1.0f, 0.0f));
	// glm::mat4 s = glm::scale(glm::mat4(), this->Transform.Scale);
	//
	// model = t * r * s;
	//
	// //glm::mat4 vp = CameraManager::GetInstance()->projection * CameraManager::GetInstance()->view;
	// GLint vLoc = glGetUniformLocation(program, "view");
	// glUniformMatrix4fv(vLoc, 1, GL_FALSE, value_ptr(CameraManager::GetInstance()->View));
	// GLint pLoc = glGetUniformLocation(program, "proj");
	// glUniformMatrix4fv(pLoc, 1, GL_FALSE, value_ptr(CameraManager::GetInstance()->Projection));
	//
	// GLint modelLoc = glGetUniformLocation(program, "model");
	// glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

	// get uniform location for transforms
	for (unsigned int i = 0; i < ARRAY_SIZE(m_boneLocation); i++)
	{
		char name[128];
		memset(name, 0, sizeof(name));
		sprintf_s(name, "jointTransforms[%d]", i);
		m_boneLocation[i] = glGetUniformLocation(program, name);
	}

	std::vector<Matrix4> transforms; // = getJointTransforms();

	boneTransforms(transforms);

	for (int i = 0; i < transforms.size(); i++)
	{
		Matrix4 Transform = transforms[i];
		glUniformMatrix4fv(m_boneLocation[i], 1, GL_TRUE, static_cast<const GLfloat*>(Transform));
	}

	glUniform1i(glGetUniformLocation(program, "bIsLit"), true);
	// Lighting::PassLightingToShader(program);
}

void ssAnimatedModel::render(TSharedPointer<Terrain> terrain)
{
	setShaderEffectVariables(terrain);

	glBindVertexArray(m_VAO);

	for (GLuint i = 0; i < m_Entries.size(); i++)
	{
		GLuint MaterialIndex = m_Entries[i].MaterialIndex;

		assert(MaterialIndex < m_Textures.size());

		if (m_Textures[MaterialIndex]->IsValid())
		{
			//m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, m_Textures[MaterialIndex]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_Textures[MaterialIndex]->TextureID);
			glUniform1i(glGetUniformLocation(program, "tex"), 0);
		}

		//glDrawElementsBaseVertex(GL_TRIANGLES,
		//						m_Entries[i].NumIndices, 
		//						GL_UNSIGNED_INT,
		//						(void*)(sizeof(GLuint) * m_Entries[i].BaseIndex),
		//						m_Entries[i].BaseVertex);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, nullptr);
	}

	// Make sure the VAO is not changed from the outside    
	glBindVertexArray(0);
}

void ssAnimatedModel::boneTransforms(std::vector<Matrix4>& transforms)
{
	Matrix4 Identity;
	Identity.InitIdentity();

	//float animDuration = (float)m_pScene->mAnimations[0]->mDuration;
	//float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
	//float TimeInTicks = (timeInSeconds) * TicksPerSecond;
	//float AnimationTime = fmod(TimeInTicks, animDuration);

	animationTime += animTick;
	if (animationTime >= animEndtime)
	{
		animationTime = animStartTime;
	}

	ReadNodeHeirarchy(animationTime, m_pScene->mRootNode, Identity);

	transforms.resize(m_NumBones);

	for (GLuint i = 0; i < m_NumBones; i++)
	{
		transforms[i] = m_BoneInfo[i].FinalTransformation;
	}
}

void ssAnimatedModel::setCurrentAnimation(int startFrameNum, int endFramNum)
{
	animStartTime = startFrameNum / static_cast<float>(animFps);
	animEndtime = endFramNum / static_cast<float>(animFps);

	animationTime = animStartTime;
}

//////////////////////////////////////////////
// Helper functions
//////////////////////////////////////////////

void ssAnimatedModel::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4& ParentTransform)
{
	std::string NodeName(pNode->mName.data);

	const aiAnimation* pAnimation = m_pScene->mAnimations[0];

	Matrix4 NodeTransformation(pNode->mTransformation); //initalizing with some value
	//NodeTransformation.InitIdentity(); // can initialize with identity but them model is 10 times bigger

	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim)
	{
		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		Matrix4 TranslationM;
		TranslationM.SetTranslationTransform(Translation.x, Translation.y, Translation.z);
		//glm::mat4 TranslationM = glm::translate(glm::mat4(), glm::vec3(Translation.x, Translation.y, Translation.z));

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		Matrix4 RotationM = Matrix4(RotationQ.GetMatrix());
		//glm::quat myQuat = glm::quat(RotationQ.x, RotationQ.y, RotationQ.z, RotationQ.w);
		//glm::mat4 RotationM = glm::toMat4(myQuat);

		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		Matrix4 ScalingM;
		ScalingM.InitIdentity();
		ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);
		//glm::mat4 ScalingM = glm::scale(glm::mat4(), glm::vec3(Scaling.x, Scaling.y, Scaling.z));

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	Matrix4 modalAnimParentTransform = ParentTransform.ToGLM() * //modalSpace parentBone animTransform
		NodeTransformation.ToGLM(); //boneSpace currentbone animTransform

	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end())
	{
		size_t BoneIndex = m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * // sceneSpace Transform 
			modalAnimParentTransform * // modelspace anim Transform for current bone
			m_BoneInfo[BoneIndex].BoneOffset; // modelSpace inverse pose Transform for current bone
	}

	for (size_t i = 0; i < pNode->mNumChildren; i++)
	{
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], modalAnimParentTransform);
	}
}

const aiNodeAnim* ssAnimatedModel::FindNodeAnim(const aiAnimation* pAnimation, std::string NodeName)
{
	/*
	for (size_t i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];// a channel is a bone with all tranformation information
		if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}
	*/

	//optimized implementation of above code
	if (boneHierarchy.find(NodeName) != boneHierarchy.end())
	{
		const aiNodeAnim* pNodeAnim = boneHierarchy[NodeName];
		return pNodeAnim;
	}

	return nullptr;
}

void ssAnimatedModel::CalcInterpolatedPosition(aiVector3D& out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1)
	{
		out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	size_t positionIndex = FindPosition(AnimationTime, pNodeAnim);
	size_t NextPositionIndex = (positionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);

	float deltaTime = static_cast<float>(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[positionIndex].mTime);
	float factor = (AnimationTime - static_cast<float>(pNodeAnim->mPositionKeys[positionIndex].mTime)) / deltaTime;

	assert(factor >= 0.0f && factor <= 1.0f);

	const aiVector3D& start = pNodeAnim->mPositionKeys[positionIndex].mValue;
	const aiVector3D& end = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D delta = end - start;

	out = start + factor * delta;
}

GLuint ssAnimatedModel::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (uint32_t i = 0; i < (uint32_t)pNodeAnim->mNumPositionKeys - 1; i++)
	{
		if (AnimationTime < static_cast<float>(pNodeAnim->mPositionKeys[i + 1].mTime))
		{
			return i;
		}
	}

	assert(0);
	return 0;
}

void ssAnimatedModel::CalcInterpolatedRotation(aiQuaternion& out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1)
	{
		out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	size_t RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	size_t NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = static_cast<float>(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - static_cast<float>(pNodeAnim->mRotationKeys[RotationIndex].mTime)) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(out, StartRotationQ, EndRotationQ, Factor);
	out = out.Normalize();
}

GLuint ssAnimatedModel::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// TODO: Confirm mTime > 0 is correct (Previously checked if keys was more than 0, indicating length more than zero for below)
	// assert(pNodeAnim->mRotationKeys->mTime > 0.0);

	for (uint32_t i = 0; (uint32_t)pNodeAnim->mNumRotationKeys - 1; i++)
	{
		if (AnimationTime < static_cast<float>(pNodeAnim->mRotationKeys[i + 1].mTime))
		{
			return i;
		}
	}

	assert(0);
	return 0;
}

void ssAnimatedModel::CalcInterpolatedScaling(aiVector3D& out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	{
		if (pNodeAnim->mNumScalingKeys == 1)
		{
			out = pNodeAnim->mScalingKeys[0].mValue;
			return;
		}

		size_t ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
		size_t NextScalingIndex = (ScalingIndex + 1);
		assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
		float DeltaTime = static_cast<float>(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
		float Factor = (AnimationTime - static_cast<float>(pNodeAnim->mScalingKeys[ScalingIndex].mTime)) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
		const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
		aiVector3D Delta = End - Start;
		out = Start + Factor * Delta;
	}
}

GLuint ssAnimatedModel::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (uint32_t i = 0; i < (uint32_t)pNodeAnim->mNumScalingKeys - 1; i++)
	{
		if (AnimationTime < static_cast<float>(pNodeAnim->mScalingKeys[i + 1].mTime))
		{
			return i;
		}
	}

	assert(0);
	return 0;
}

void ssAnimatedModel::Clear()
{
	for (size_t i = 0; i < m_Textures.size(); i++)
	{
		//SAFE_DELETE(m_Textures[i]);
	}

	if (m_Buffers[0] != 0)
	{
		glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
	}

	if (m_VAO != 0)
	{
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
}

///////////////// getter and setters //////////////////////

void ssAnimatedModel::SetPosition(const SVector InPosition)
{
	this->Transform.Position = InPosition;
}

void ssAnimatedModel::SetRotation(const SRotator InRotation)
{
	this->Transform.Rotation = InRotation;
}

void ssAnimatedModel::SetScale(const SVector InScale)
{
	this->Transform.Scale = InScale;
}

void ssAnimatedModel::setSpeed(float _speed)
{
	//this->playerSpeed = _speed;
}
