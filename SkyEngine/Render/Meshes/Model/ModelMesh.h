// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <string>
#include <assimp/types.h>
#include <vector>

#include "Camera/CameraManager.h"

using namespace std;
// GL Includes

struct Vertex
{
	glm::vec3 Position; // Position	
	glm::vec3 Normal; // Normal	
	glm::vec2 TexCoords; // TexCoords
};

struct MeshTexture
{
	GLuint id;
	string type;
	aiString path;
};

class ModelMesh
{
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<MeshTexture> textures;

	/*  Functions  */
	// Constructor
	ModelMesh(vector<Vertex> vertices, vector<GLuint> indices, vector<MeshTexture> textures);

	// Render the mesh
	void Render(GLuint program, FTransform ModelTransform);

private:
	/*  Render data  */
	GLuint VAO, VBO, EBO;

	/*  Functions    */
	// Initializes all the buffer objects/arrays
	void setupMesh();
};
