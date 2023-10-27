// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
// #include "Render/Materials/MaterialType.h"
// #include "Render/Materials/Material.h"

class CMaterialInterface;
class IRenderer;

// TODO: STDL warnings with exporting class with vector
#pragma warning (disable : 4251)

class ENGINE_API CShader 
{
	// TODO: static assert for shader params
	// static_assert(ShaderParameters::value, "Must have shader parameters");
    
public:
	virtual ~CShader() = default;
	CShader() {}
	CShader(const std::string& InShaderName, const std::string& InVertexShaderPath, const std::string& InFragmentShaderPath);
	CShader(const std::string& InShaderName, const std::string& InComputeShaderPath);
	void SetGeometryShader(const std::string& InGeometryShaderPath);
	void SetTessShader(const std::string& InTessControlShaderPath, const std::string& InTessEvalShaderPath);

	virtual bool CompileShader();
	void BindMaterial(TPointer<CMaterialInterface> InMaterial);
	virtual void BindShader();


	std::string GetShaderName() const { return ShaderName; }
	// const std::map<std::string, MaterialAttribute>& GetShaderAttributes() { return ShaderAttributes; }

	// TODO: Remove once not needed
	uint32_t GetShaderProgram() const { return ShaderProgram; }
	
	// Flags
	bool bTwoSided = false;
	bool bDepthTest = true;
	// TODO: Blend mode (translucent)
	
protected:
	// void DefineAttribute(const std::string& AttributeName, float Number);
	// void DefineAttribute(const std::string& AttributeName, int Integer);
	// void DefineAttribute(const std::string& AttributeName, bool Boolean);
	// void DefineAttribute(const std::string& AttributeName, SVector Vector3);
	// void DefineAttribute(const std::string& AttributeName, SVector4 Vector4);
	// void DefineAttribute(const std::string& AttributeName, TPointer<CTexture> Texture);
	// void DefineAttribute(const std::string& AttributeName, MaterialAttribute Attribute);
	
	std::string VertexShaderPath;
	std::string FragmentShaderPath;
	std::string GeometryShaderPath;
	std::string TessControlShaderPath;
	std::string TessEvalShaderPath;
	std::string ComputeShaderPath;
	std::string ShaderName;
	uint32_t ShaderProgram;

	// std::map<std::string, MaterialAttribute> ShaderAttributes;
	// std::map<std::string, int32_t> UniformLocations;
};

#define DefineShaderFloatParam(ParamName, DefaultValue)\
	float ParamName = DefaultValue;\
	int32_t ParamName##Location = -1;\
	std::string ParamName##Name = #ParamName;

#define DefineShaderVector4Param(ParamName, DefaultValue)\
	SVector4 ParamName = DefaultValue;\
	int32_t ParamName##Location = -1;\
	std::string ParamName##Name = #ParamName;

#define DefineShaderTextureParam(ParamName, DefaultValue)\
	TPointer<CTexture> ParamName = DefaultValue;\
	int32_t ParamName##Location = -1;\
	int32_t Has##ParamName##Location = -1;\
	std::string ParamName##Name = #ParamName;


/* We want to reduce switching shaders as much as we can, so we will create a shader program for the base shader from file
 * When we want to render multiple materials, we will batch all objects that use that material together, switch to this shader program, then
 * pass uniforms to update all those variables to match the material
 */
