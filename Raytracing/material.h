#pragma once

#include "vec3.h"

class material {
public:
	material() : rough(1), reflect(1), c(color()) {}
	material(float roughness, float reflectivity, color col) : rough(roughness), reflect(reflectivity), c(col) {}

	float material_roughness() const;
	void set_material_roughness(float roughness);
	float material_reflectivity() const;
	void set_material_reflectivity(float reflectivity);
	color material_color() const;
	void set_material_color(color col);
private:
	float rough;
	float reflect;
	color c;
};

