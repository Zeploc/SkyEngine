// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "SEPCH.h"
#include "Texture.h"

#include "Core/Application.h"

CTexture::CTexture(const std::string InPath)
{
	Path = InPath;
}

bool CTexture::IsValid() const
{
	return !Path.empty() && IsBound();
}

bool CTexture::IsBound() const
{
	return bBound;
}

void CTexture::Bind(const uint32_t InTextureID)
{
	TextureID = InTextureID;
	bBound = true;
}

std::shared_ptr<CTexture> CTexture::shared_from_this()
{
	return std::static_pointer_cast<CTexture>(CAssetObject::shared_from_this());
}

TWeakPointer<CTexture> CTexture::weak_from_this()
{
	return std::static_pointer_cast<CTexture>(CAssetObject::weak_from_this().lock());
}

void CTexture::OnLoaded()
{	
	GetGraphicsAPI()->GenerateTexture(weak_from_this());
	GetTextureManager()->AddLoadedTexture(shared_from_this());
}

void CTexture::Serialize(std::ostream& os)
{
	os << Path << std::string("\n");
}

void CTexture::Deserialize(std::istream& is)
{
	is >> Path;
}

std::string CTexture::GetAssetClassName()
{
	return GetStaticName();
}

std::string CTexture::GetStaticName()
{
	return "Texture";
}

void CTexture::Open()
{
}
