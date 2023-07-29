// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "Model.h"

// Engine Includes //
#include "Lighting.h"
#include "Shader.h"

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
	Colour = _Colour;
	TextureSource = ModelSource;
	bHasTexture = true;
	// NOTE: Not currently used when drawing this model
	program = Shader::Programs["BaseProgram"];//Shader::Programs["ModelProgram"];// 
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
		if (it.first == TextureSource)
		{
			pModelObject = it.second;
			return;
		}
	}
	pModelObject = std::make_shared<ModelObject>(TextureSource);
	Shader::Models.insert(std::pair<std::string, Pointer<ModelObject>>(TextureSource, pModelObject));
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
	if (bIsLit)
	{
		program = Shader::Programs["ModelProgramLit"];
		pModelObject->program = program;
	}
	else
	{
		program = Shader::Programs["ModelProgram"];
		pModelObject->program = program;
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
	glUseProgram(program);
	if (bIsLit)
	{
		Lighting::PassLightingToShader(program, LightProperties, Newtransform);
	}
	glUniform4fv(glGetUniformLocation(program, "fragcolor"), 1, value_ptr(Colour));
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
