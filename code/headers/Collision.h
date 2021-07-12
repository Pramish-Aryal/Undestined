#pragma once

#include "utils.h"

struct Rect
{
	Vec2f pos;
	Vec2f size;
};

class CollisionRect
{
	public:
	Rect rect;
	
	bool point_vs_rect(const Vec2f& p, const Rect& r);
	bool rect_vs_rect (const Rect& r1, const Rect& r2);
};