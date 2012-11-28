#version 330

in vec2 vFragModelPosition;

out vec4 fFragColor;

uniform vec3 uParticleColor;

float computeAttenuation(float distance) {
    return exp(-distance * distance * 3.f);
}

void main() {
    float distance = length(vFragModelPosition);
    float attenuation = computeAttenuation(distance);
    fFragColor = vec4(uParticleColor, attenuation);
}
