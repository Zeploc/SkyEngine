// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

class CShader;
template<class S>
class TMaterial;
// Do not use directly, instead use material interface
using CMaterial = TMaterial<CShader>;
