// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include <stdio.h>
#ifdef WIN32
#define _USE_MATH_DEFINES
#include <cmath>
#else
#include <math.h>
#endif

#include <assimp/vector3.h>
#include <assimp/matrix3x3.h>
#include <assimp/matrix4x4.h>
#include <string.h>

#include <cassert>
#include <glm/mat4x4.hpp>
#include <glm/detail/type_vec3.hpp>

#define M_PI   3.14159265358979323846
#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))
#define ZERO_MEM(a) memset(a, 0, sizeof(a))

#ifdef WIN32
#define SNPRINTF _snprintf_s
#define VSNPRINTF vsnprintf_s
#define RANDOM rand
#define SRANDOM srand((unsigned)time(NULL))
#if (_MSC_VER == 1900)
#elif (_MSC_VER == 1800)
#else
//float fmax(float a, float b);
#endif
#else
#define SNPRINTF snprintf
#define VSNPRINTF vsnprintf
#define RANDOM random
#define SRANDOM srandom(getpid())
#endif

float RandomFloat();

//
//float RandomFloat()
//{
//    float Max = RAND_MAX;
//    return ((float)RANDOM() / Max);
//}
