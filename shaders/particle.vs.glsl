#version 330

layout(location = 0) in vec2 aVertexPosition;

uniform vec2 uParticlePosition;
uniform float uParticleScale;

out vec2 vFragModelPosition;

void main() {
    vFragModelPosition = aVertexPosition;
    gl_Position = vec4(uParticlePosition + uParticleScale * aVertexPosition, 0.f, 1.f);
}
