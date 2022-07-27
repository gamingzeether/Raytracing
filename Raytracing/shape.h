#pragma once

#include <vector>

#include "aabb.h"
#include "material.h"
#include "ray.h"
#include "vec4.h"

class shape {
public:
	shape() : pos(pos3(0.0f, 0.0f, 0.0f)), id(next_id), ori(quaternion(1, 0, 0 ,0)) {
		next_id++;
	}
	shape(pos3 position) : pos(position), id(next_id), ori(quaternion(1, 0, 0, 0)) {
		next_id++;
	}
	shape(pos3 position, material mat) : pos(position), m(mat), id(next_id), ori(quaternion(1, 0, 0, 0)) {
		next_id++;
	}

	struct intersect_info {
		float distance;
		pos3 position;
		vec3 normal;
		int shape_id;

		intersect_info() : distance(-1), shape_id(-1) {}
		intersect_info(float d, pos3 p, vec3 n, int i) : distance(d), position(p), normal(n), shape_id(i) {}
	};

	inline const pos3& position() const { return pos; }
	void set_position(pos3 new_pos);
	inline const material& shape_material() const { return m; }
	void set_shape_material(material col);
	inline const aabb& bounding_box() const { return box; }
	void set_bounding_box(aabb bounding_box);
	inline const quaternion& orientation() const { return ori; }
	void set_orientation(quaternion orientation);

	virtual intersect_info ray_intersects(const ray& r) const;
	virtual bool is_inside(const pos3& point) const;
	virtual void update_bounding_box();
protected:
	pos3 pos;
	color c;
	material m;
	int id;
	aabb box;
	quaternion ori;

	static int next_id;
};

