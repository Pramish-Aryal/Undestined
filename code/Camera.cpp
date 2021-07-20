#include "Camera.h"

Camera::Camera()
{
	pos = {0.f, 0.f};
}

Camera& Camera::get_instance()
{
	static Camera camera;
	return camera;
}

Vec2f& Camera::get_pos()
{
	return pos;
}