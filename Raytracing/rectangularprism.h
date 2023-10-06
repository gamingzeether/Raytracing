#pragma once

#include "shape.h"

class rectangularprism :
    public shape {

public:
    rectangularprism() : shape(pos3(0, 0, 0)), corners{ pos3(0, 0, 0), pos3(0, 0, 0) } {
        width = 0;
        height = 0;
        depth = 0;
        update_bounding_box();
    }
    rectangularprism(pos3 min, pos3 max, material mat) : shape((min + max) / 2, mat), corners{ min, max } {
        vec3 dimensions = max - min;
        width = dimensions.x();
        height = dimensions.y();
        depth = dimensions.z();
        update_bounding_box();
    }
    rectangularprism(pos3 pos, float w, float h, float d, material mat) : shape(pos, mat), width(w), height(h), depth(d) {
        vec3 half_direction = vec3(width / 2, height / 2, depth / 2);
        corners[0] = pos - half_direction;
        corners[1] = pos + half_direction;
        update_bounding_box();
    }

    intersect_info ray_intersects(const ray& ray) const;
    void update_bounding_box();
protected:
    pos3 corners[2]; // World coordinates
    float width, height, depth;
};

