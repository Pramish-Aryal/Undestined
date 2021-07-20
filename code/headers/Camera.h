#pragma once

#include "utils.h"

class Camera
{
	Vec2f pos;
	Camera();
	public:
	
	Camera(const Camera&) = delete;
	
	static Camera& get_instance();
	
	Vec2f& get_pos();
};