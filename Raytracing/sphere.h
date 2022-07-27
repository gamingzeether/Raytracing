#pragma once

#include "shape.h"

class sphere :
    public shape {
public:
    sphere() : shape(), rad(1.0f) {
        update_bounding_box();
    }
    sphere(pos3 position, float radius) : 
        shape(position),
        rad(radius) {
        update_bounding_box();
    }
    sphere(pos3 position, float radius, material mat) : 
        shape(position, mat),
        rad(radius) {
        update_bounding_box();
    }

    inline float radius() const { return rad; }
    void set_radius(float radius);

    intersect_info ray_intersects(const ray& r) const;
    bool is_inside(const pos3& point) const;
    void update_bounding_box();
protected:
    float rad;
};

