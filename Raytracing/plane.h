#pragma once

#include "shape.h"

class plane :
    public shape {
public:
    plane() : shape(), w(1.0f), h(1.0f), half_w(w / 2), half_h(h / 2) {
        update_bounding_box();
    }
    plane(pos3 position, float width, float height) : shape(position), w(width), h(height), half_w(width / 2), half_h(height / 2) {
        update_bounding_box();
    }
    plane(pos3 position, float width, float height, material mat) : shape(position, mat), w(width), h(height), half_w(width / 2), half_h(height / 2) {
        update_bounding_box();
    }

    inline float width() const { return w; }
    void set_width(float width);
    inline float height() const { return h; }
    void set_height(float height);

    intersect_info ray_intersects(const ray& ray) const;
    bool is_inside(const pos3& point) const;
    void update_bounding_box();
protected:
    float w;
    float h;
    float half_w;
    float half_h;
};

