#pragma once

#include "shape.h"

class sphere :
    public shape {
public:
    sphere() : shape(), r(1.0f) {
        update_bounding_box();
    }
    sphere(pos3 position, float radius) : 
        shape(position),
        r(radius) {
        update_bounding_box();
    }
    sphere(pos3 position, float radius, material mat) : 
        shape(position, mat),
        r(radius) {
        update_bounding_box();
    }

    inline float radius() const { return r; }
    void set_radius(float radius);

    intersect_info ray_intersects(const ray& ray) const;
    bool is_inside(const pos3& point) const;
    void update_bounding_box();
protected:
    float r;
};

