// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Model.h"

// Engine Includes //
#include "Render/Lighting.h"
#include "Render/Shader.h"

// Library Includes //
#include <map>
#include <glm/gtc/type_ptr.hpp>

#include "ModelObject.h"

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
	MeshMaterial.Colour = _Colour;
	MeshMaterial.Texture.Path = ModelSource;
	// NOTE: Not currently used when drawing this model
	MeshMaterial.ShaderProgram = Shader::Programs["BaseProgram"];//Shader::Programs["ModelProgram"];// 
	//m_iIndicies = 36;
	BindModel();
	m_eShape = EMESHTYPE::MODEL;
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
	for (auto& it : Shader::Models)
	{
		if (it.first == MeshMaterial.Texture.Path)
		{
			pModelObject = it.second;
			return;
		}
	}
	pModelObject = std::make_shared<ModelObject>(MeshMaterial.Texture.Path);
	Shader::Models.insert(std::pair<std::string, Pointer<ModelObject>>(MeshMaterial.Texture.Path, pModelObject));
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

void Model::SetLit(bool _bIsLit)
{
	Mesh::SetLit(_bIsLit);
	// TODO: Not currently used properly
	if (MeshMaterial.bIsLit)
	{
		MeshMaterial.ShaderProgram = Shader::Programs["ModelProgramLit"];
		pModelObject->program = MeshMaterial.ShaderProgram;
	}
	else
	{
		MeshMaterial.ShaderProgram = Shader::Programs["ModelProgram"];
		pModelObject->program = MeshMaterial.ShaderProgram;
	}
}

/************************************************************
#--Description--#:	Render Current Mesh to the screen
#--Author--#: 		Alex Coultas
#--Parameters--#: 	NA
#--Return--#: 		NA
************************************************************/
void Model::Render(FTransform Newtransform)
{
	// TODO: Review/improve
	glFrontFace(GL_CCW);
	glUseProgram(MeshMaterial.ShaderProgram);
	if (MeshMaterial.bIsLit)
	{
		Lighting::PassLightingToShader(MeshMaterial.ShaderProgram, MeshMaterial.LightProperties, Newtransform);
	}
	glm::vec4 Vec = MeshMaterial.Colour;
	glUniform4fv(glGetUniformLocation(MeshMaterial.ShaderProgram, "fragcolor"), 1, value_ptr(Vec));
	pModelObject->Render(Newtransform);
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
