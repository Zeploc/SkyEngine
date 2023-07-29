#pragma once


#include "Vector2.decl.h"
#include "Math/Rotator.h"

template <typename T>
template <class U, typename>
Vector2x<T>& Vector2x<T>::operator+=(U scalar)
{
	AddValues(scalar, scalar);
	return *this;
}

template <typename T>
template <typename U, typename>
Vector2x<T>& Vector2x<T>::operator+=(Vector2x<U> other)
{
	AddValues(other.x, other.y);
	return *this;
}

template <typename T>
template <typename U, typename>
Vector2x<T>& Vector2x<T>::operator+=(glm::tvec2<U> other)
{
	AddValues(other.x, other.y);
	return *this;
}

template <typename T>
template <class U, typename>
Vector2x<T>& Vector2x<T>::operator-=(U scalar)
{
	MinusValues(scalar, scalar);
	return *this;
}

template <typename T>
template <typename U, typename>
Vector2x<T>& Vector2x<T>::operator-=(Vector2x<U> other)
{
	MinusValues(other.x, other.y);
	return *this;
}

template <typename T>
template <typename U, typename>
Vector2x<T>& Vector2x<T>::operator-=(glm::tvec2<U> other)
{
	MinusValues(other.x, other.y);
	return *this;
}

template <typename T>
Rotator Vector2x<T>::ToRotator() const
{
	// TODO:
	return Rotator();
}

template <typename T>
template <typename U>
void Vector2x<T>::MinusValues(U InX, U InY)
{
	this->x -= static_cast<T>(InX);
	this->y -= static_cast<T>(InY);
}

template <typename T>
template <typename U>
void Vector2x<T>::AddValues(U InX, U InY)
{
	this->x += static_cast<T>(InX);
	this->y += static_cast<T>(InY);
}
