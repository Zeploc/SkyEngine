// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2018 Media Design School
//
// File Name    	:    TessMesh.h
// Description    	:    Header file outlining the Class
// Author       	:    Alex Coultas
// Mail         	:    alex.cou7417@mediadesign.school.nz
//

#pragma once
// This Includes //
#include "Engine\Mesh.h"

class TessMesh : public Mesh
{
public:
	TessMesh(float fWidth, float fHeight, glm::vec4  _Colour);
	~TessMesh();

	virtual void Render(Utils::Transform Newtransform) override;

	virtual void Rebind() override; // Will replace if texture exists

	void BindTess();
};

