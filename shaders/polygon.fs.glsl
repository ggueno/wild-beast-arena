#version 330

uniform vec3 uPolygonColor;

out vec3 fFragColor;

void main() {
	fFragColor = uPolygonColor;
}