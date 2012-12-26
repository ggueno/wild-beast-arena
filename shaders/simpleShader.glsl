// lightingShader.glsl
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr

#version 150


// Uniforms : data shared by every shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 color;
uniform sampler2D textureUnitDiffuse;


#ifdef _VERTEX_

// Attributes : per vertex data
in vec4 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexColor;
in vec2 vertexUv;

// Varyings : data to transmit to fragments
out vec2 uv;

void main()
{
    uv = vertexUv;
    gl_Position = projection * view * model * vertexPosition;
}

#endif




#ifdef _FRAGMENT_


// Varyings : data receved and interpolated from the vertex shaders
in vec2 uv;

// Final output
out vec4 fragColor;

void main()
{
    vec3 diffuse = texture(textureUnitDiffuse, uv).rgb;
    fragColor = vec4(diffuse, 1.0);
}

#endif
