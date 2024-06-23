#include "SEPCH.h"
#include "Camera.h"

Camera::Camera(const STransform& InTransform)
: Entity(InTransform, "Camera")
{
}

std::string Camera::GetEntityClassName()
{
	return GetStaticName();
}

std::string Camera::GetStaticName()
{
	return "Camera";
}

void Camera::Serialize(std::ostream& os)
{
	Entity::Serialize(os);
}

void Camera::Deserialize(std::istream& is)
{
	Entity::Deserialize(is);
}
