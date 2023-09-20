#pragma once

#include "shape.h"

class triangle : 
	public shape {
public:
    triangle() : shape(), points{vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 0, 0)} {
        update_bounding_box();
    }
    triangle(pos3 position, vec3 p1, vec3 p2, vec3 p3) : shape(position), points { p1, p2, p3 } {
        update_bounding_box();
    }
    triangle(pos3 position, vec3 p1, vec3 p2, vec3 p3, material mat) : shape(position, mat), points{ p1, p2, p3 } {
        update_bounding_box();
    }

    inline vec3 point(int i) const { return pos + ori.rotate(points[i]); }
    void set_point(int i, vec3 p);
    void set_point(int i, vec2 p);

    intersect_info ray_intersects(const ray& ray) const;
    bool is_inside(const pos3& point) const;
    void update_bounding_box();
protected:
    vec3 points[3];
};

