#include "SEPCH.h"
#include "Camera.h"

Camera::Camera(const STransform& InTransform)
: Entity(InTransform, "Camera")
{
}
