// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Model.h"

// Engine Includes //
#include "Render/Shaders/ShaderManager.h"

// Library Includes //
#include <map>
#include <glm/gtc/type_ptr.hpp>

#include "ModelObject.h"
#include "Render/Shaders/PBRShader.h"

/************************************************************
#--Description--#:  Constructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	Takes contructor values
#--Return--#: 		NA
************************************************************/
Model::Model(glm::vec4 _Colour, const char* ModelSource)
{
	m_fWidth = 0; // fWidth;
	m_fHeight = 0; // fHeight;
	m_fDepth = 0; // fDepth;
	ModelPath = ModelSource;
	
	MeshMaterial = std::make_shared<CMaterial>(ShaderManager::GetShader<CPBRShader>());
	// MeshMaterial->Colour = _Colour;
	BindModel();
}

/************************************************************
#--Description--#:  Destructor function
#--Author--#: 		Alex Coultas
#--Parameters--#:	NA
#--Return--#: 		NA
************************************************************/
Model::~Model()
{
	pModelObject = nullptr;
}

/************************************************************
#--Description--#: 	Binds pyramid with sepcified attributes
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Model::BindModel()
{
	// TODO: bind model
	// for (auto& it : ShaderManager::Models)
	// {
	// 	if (it.first == MeshMaterial->GetTextureData().Path)
	// 	{
	// 		pModelObject = it.second;
	// 		return;
	// 	}
	// }
	// pModelObject = std::make_shared<ModelObject>(MeshMaterial->GetTextureData().Path);
	// ShaderManager::Models.insert(std::pair<std::string, TPointer<ModelObject>>(ModelPath, pModelObject));
}

/************************************************************
#--Description--#: 	Rebinds the vao with the colour (not texture)
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Model::Rebind()
{
	BindModel();
}

/************************************************************
#--Description--#:	Render Current Mesh to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Model::Render(STransform Newtransform)
{
	// TODO: Review/improve
	// glFrontFace(GL_CCW);
	// glUseProgram(MeshMaterial.ShaderProgram);
	// if (MeshMaterial.bIsLit)
	// {
	// 	Lighting::PassLightingToShader(MeshMaterial.ShaderProgram, MeshMaterial.LightProperties, Newtransform);
	// }
	// glm::vec4 Vec = MeshMaterial.Colour;
	// glUniform4fv(glGetUniformLocation(MeshMaterial.ShaderProgram, "fragcolor"), 1, value_ptr(Vec));
	// pModelObject->Render(Newtransform);
}

/************************************************************
#--Description--#: 	Updated every frame
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Model::Update()
{
}
