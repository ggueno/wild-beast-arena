// lightingShader.glsl
// Template for OpenGL 3.*
// N. Dommanget dommange@univ-mlv.fr


#version 150

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2D textureUnitDiffuse;
uniform sampler2D textureUnitSpecular;
uniform float Time;
uniform bvec4 filledData;
uniform int life;
uniform vec4 color;

// Uniforms : data shared by every shader

#ifdef _VERTEX_



// Attributes : per vertex data
in vec3 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexColor;
in vec2 vertexUv;

// Varyings : data to transmit to fragments
out vec2 uv;
out vec3 normal;
out vec3 position;
out vec4 localColor;

void main()
{
    uv = vertexUv;
    normal = vec3(model * vec4(vertexNormal, 1.0));
    position = vec3(model * vec4(vertexPosition,1.0));
    gl_Position = projection * view * model * vec4(vertexPosition,1.0);
}

#endif




#ifdef _FRAGMENT_


// Varyings : data receved and interpolated from the vertex shaders
in vec2 uv;
in vec3 position;
in vec3 normal;
in vec4 localColor;

// Final output
out vec4 fragColor;

vec3 pointLight(in vec3 lcolor, in float intensity, in vec3 lpos, in vec3 n, in vec3 fpos, vec3 diffuse, float spec, vec3 cpos)
{
    vec3 l =  lpos - fpos;
    vec3 v = fpos - cpos;
    vec3 h = normalize(l-v);
    float n_dot_l = clamp(dot(n, l), 0, 1.0);
    float n_dot_h = clamp(dot(n, h), 0, 1.0);
    float d = distance(l, position);
    float att = clamp( 1.0 / ( 1 + 0.1 * (d*d)), 0.0, 1.0);
    vec3 color1 = lcolor * intensity * att * (diffuse * n_dot_l  + spec * vec3(1.0, 1.0, 1.0) *  pow(n_dot_h, spec * 100.0));
    return color1;
}

vec3 directionalLight(in vec3 lcolor, in float intensity, in vec3 ldir, in vec3 n, in vec3 fpos, vec3 diffuse, float spec, vec3 cpos)
{
    vec3 l = ldir;
    vec3 v = fpos - cpos;
    vec3 h = normalize(l-v);
    float n_dot_l = clamp(dot(n, -l), 0, 1.0);
    float n_dot_h = clamp(dot(n, h), 0, 1.0);
    float d = distance(l, position);
    vec3 color1 = lcolor * intensity * (diffuse * n_dot_l + spec * vec3(1.0, 1.0, 1.0) *  pow(n_dot_h, spec * 100.0));
    return color1;
}

vec3 spotLight(in vec3 lcolor, in float intensity, in vec3 ldir, in vec3 lpos, in vec3 n, in vec3 fpos, vec3 diffuse, float spec, vec3 cpos)
{
    vec3 l =  lpos - fpos;
    float cosTs = dot( normalize(-l), normalize(ldir) );
    float thetaP =  radians(30.0);
    float cosTp = cos(thetaP);
    vec3 v = fpos - cpos;
    vec3 h = normalize(l-v);
    float n_dot_l = clamp(dot(n, l), 0, 1.0);
    float n_dot_h = clamp(dot(n, h), 0, 1.0);
    float d = distance(l, fpos);
    vec3 color = vec3(0.0, 0.0, 0.0);
    if (cosTs > cosTp)
        color = pow(cosTs, 30.0) * lcolor * intensity * (diffuse * n_dot_l + spec * vec3(1.0, 1.0, 1.0) *  pow(n_dot_h, spec * 100.0));
    return color;
}


void main()
{
    vec3 diffuse = texture(textureUnitDiffuse, uv).rgb;
    float spec = texture(textureUnitSpecular, uv).r;
    vec3 n = normalize(normal);

    if(color.xyz != vec3(1.0f,1.0f,1.0f))
        diffuse = mix(color.xyz,diffuse,0.5);

    float limite = 23.;
    float hauteur = 1.;
    float intensity1 =  cos(mod(Time,10))* 0.05;
    float intensity2 = 0.8;
    float c = cos(mod(Time,10)) * 0.1;
    float s = sin(mod(Time,10)) * 0.1;

    vec3 cspotlight1 = spotLight(vec3(1.0, 1.0, 1.0), intensity1, vec3(1., -1., 0.), vec3( -limite, limite, hauteur), n, position, diffuse, spec, vec3(0.,0.,55.) );

    vec3 cspotlight2 = spotLight(vec3(1.0, 1.0, 0.0), intensity2, vec3(-1., 1., 0.), vec3( limite, -limite, hauteur), n, position, diffuse, spec, vec3(0.,0.,55.) );

    vec3 cspotlight3 = spotLight(vec3(0.0, 0.0, 1.0), 1., vec3(-1., -1., 0.), vec3( limite, limite, hauteur), n, position, diffuse, spec, vec3(0.,0.,55.) );

    vec3 cspotlight5 = spotLight(vec3(1.0, 1.0, 1.0), 1., vec3(-1., -1., 0.), vec3( limite, limite, hauteur), n, position, diffuse, spec, vec3(0.,0.,55.) );

    vec3 cspotlight4 = spotLight(vec3(1.0, 1.0, 1.0), 1.0, vec3(1., 1., 0.), vec3( -limite, -limite, hauteur), n, position, diffuse, spec, vec3(0.,0.,55.) );

    vec3 directionalLight = directionalLight(vec3(1.0, 1.0, 1.0), .1, vec3( 0.0, 0.0, 3.0), n, position, diffuse, spec, vec3(0.,0.,55.));

    vec3 pointLight = pointLight(vec3(1.0, 1.0, 1.0), 1., vec3( 0.0, 0.0, 3.0), n, position, diffuse, spec, vec3(0.,0.,55.));



    fragColor = vec4(pointLight + cspotlight5 + cspotlight1 + cspotlight2 + cspotlight3 + cspotlight4 , 1.0);
}

#endif

