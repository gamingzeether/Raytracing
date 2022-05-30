#include "material.h"

float material::material_roughness() const {
	return rough;
}

void material::set_material_roughness(float roughness) {
	rough = roughness;
}

float material::material_reflectivity() const {
	return reflect;
}

void material::set_material_reflectivity(float reflectivity) {
	reflect = reflectivity;
}

color material::material_color() const {
	return c;
}

void material::set_material_color(color col) {
	c = col;
}
