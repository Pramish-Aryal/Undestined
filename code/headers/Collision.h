#pragma once

#include "utils.h"

struct Rect
{
	Vec2f pos;
	Vec2f size;
};

class Collider
{
	public:
	Rect rect;
	
	bool point_vs_rect(const Vec2f& p, const Rect& r);
	bool rect_vs_rect(const Rect& r1, const Rect& r2);
	bool Collider::ray_vs_rect(const Vec2f& origin, const Vec2f& dirn, const Rect* rect, Vec2f& contact_point, Vec2f& contact_normal,types::r32& t_hit_near);
	bool Collider::dynamic_rect_vs_rect(const Rect* rect1, const Vec2f& vel, const Rect* rect2, Vec2f& contact_point, Vec2f& contact_normal, types::r32& contact_time, const types::r32& dt);
};