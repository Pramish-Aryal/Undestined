#include "Collision.h"

bool CollisionRect::point_vs_rect(const Vec2f& p, const Rect& r)
{
	return p.x >= r.pos.x && p.y >= r.pos.y && p.x <= r.pos.x + r.size.w && p.y <= r.pos.y + r.size.h;
}

bool CollisionRect::rect_vs_rect (const Rect& r1, const Rect& r2)
{
	return r1.pos.x + r1.size.w >= r2.pos.x && r1.pos.x <= r2.pos.x + r2.size.w && 
		r1.pos.y + r1.size.h >= r2.pos.y && 
		r1.pos.y <= r2.pos.y + r2.size.h;
}