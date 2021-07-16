#include "Collision.h"
#include <algorithm>
#include <functional>
#include <cmath>
using namespace types;

bool Collider::point_vs_rect(const Vec2f& p, const Rect& r)
{
	return p.x >= r.pos.x && p.y >= r.pos.y && p.x <= r.pos.x + r.size.w && p.y <= r.pos.y + r.size.h;
}

bool Collider::rect_vs_rect (const Rect& r1, const Rect& r2)
{
	return r1.pos.x + r1.size.w >= r2.pos.x && r1.pos.x <= r2.pos.x + r2.size.w && 
		r1.pos.y + r1.size.h >= r2.pos.y && 
		r1.pos.y <= r2.pos.y + r2.size.h;
}

bool Collider::ray_vs_rect(const Vec2f& origin, const Vec2f& dirn, const Rect* rect, Vec2f& contact_point, Vec2f& contact_normal,r32& t_hit_near)
{
	contact_point = {0,0};
	contact_normal = {0,0};
	
	Vec2f t_near = (rect->pos - origin) / dirn;
	Vec2f t_far = (rect->pos + rect->size - origin) / dirn;
	
	if(std::isnan(t_near.y) || std::isnan(t_near.x)) return false;
	if(std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
	
	if(t_near.x > t_far.x) swap(t_near.x, t_far.x);
	if(t_near.y > t_far.y) swap(t_near.y, t_far.y);
	
	if(t_near.x > t_far.y || t_near.y > t_far.x) return false;
	
	t_hit_near = MAX(t_near.x, t_near.y);
	r32 t_hit_far = MIN(t_far.x, t_far.y);
	
	if(t_hit_far < 0) return false;
	
	contact_point = origin + t_hit_near * dirn;
	
	if(t_near.x > t_near.y)
	{
		if(dirn.x < 0)
			contact_normal = {1, 0};
		else
			contact_normal = {-1, 0};
	}
	else if( t_near.x < t_near.y)
	{
		if(dirn.y < 0)
			contact_normal = {0, 1};
		else
			contact_normal = {0, -1};
	}
	return true;
}

bool Collider::dynamic_rect_vs_rect(const Rect* rect1, const Vec2f& vel, const Rect* rect2, Vec2f& contact_point, Vec2f& contact_normal, r32& contact_time, const r32& dt)
{
	if(vel.x == 0 && vel.y == 0) return false;
	
	Rect expanded_rect = {rect2->pos - rect1->size / 2, rect2->size + rect1->size};
	
	return (ray_vs_rect(rect1->pos  + rect1->size / 2, vel * dt, &expanded_rect, contact_point, contact_normal, contact_time) && contact_time >= 0 && contact_time <= 1.0f);
	
}