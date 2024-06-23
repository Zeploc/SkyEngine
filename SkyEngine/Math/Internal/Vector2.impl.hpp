// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "SEPCH.h"
#include "Vector2.decl.h"
#include "Math/Rotator.h"

template <typename T>
template <class U, typename>
TVector2<T>& TVector2<T>::operator+=(U scalar)
{
	AddValues(scalar, scalar);
	return *this;
}

template <typename T>
template <typename U, typename>
TVector2<T>& TVector2<T>::operator+=(TVector2<U> other)
{
	AddValues(other.x, other.y);
	return *this;
}

template <typename T>
template <typename U, typename>
TVector2<T>& TVector2<T>::operator+=(glm::tvec2<U> other)
{
	AddValues(other.x, other.y);
	return *this;
}

template <typename T>
template <class U, typename>
TVector2<T>& TVector2<T>::operator-=(U scalar)
{
	MinusValues(scalar, scalar);
	return *this;
}

template <typename T>
template <typename U, typename>
TVector2<T>& TVector2<T>::operator-=(TVector2<U> other)
{
	MinusValues(other.x, other.y);
	return *this;
}

template <typename T>
template <typename U, typename>
TVector2<T>& TVector2<T>::operator-=(glm::tvec2<U> other)
{
	MinusValues(other.x, other.y);
	return *this;
}

template <typename T>
SRotator TVector2<T>::ToRotator() const
{
	// TODO:
	return SRotator();
}

template <typename T>
template <typename U>
void TVector2<T>::MinusValues(U InX, U InY)
{
	this->x -= static_cast<T>(InX);
	this->y -= static_cast<T>(InY);
}

template <typename T>
template <typename U>
void TVector2<T>::AddValues(U InX, U InY)
{
	this->x += static_cast<T>(InX);
	this->y += static_cast<T>(InY);
}
