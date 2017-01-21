// Vertex shader for drawing the earth with three textures
// (Based on OpenGL Shading Language book example)


#version 400

void setupNormalMapping(in mat3 normal_matrix);

uniform mat4 matModelView;
uniform mat4 matModelViewLight;
uniform mat4 matProjection;
uniform mat4 matTexture;

uniform vec4 vecLightPosition;
uniform bool DoNormalMap;

layout(location = 0) in vec3 vecPosition;
layout(location = 1) in vec3 vecNormal;
layout(location = 2) in vec2 vecTexCoord;

out float Diffuse;
out vec3 Specular;
out vec2 texCoord;


void main(void)
{
    // calculate normal transformation matrix
    mat3 normal_matrix = transpose(inverse(mat3(matModelView)));

    if (DoNormalMap) setupNormalMapping(normal_matrix);

    vec4 light_pos  = matModelViewLight * vecLightPosition;

    vec3 ecPosition = vec3(matModelView * vec4(vecPosition, 1.0));
    vec3 tnorm      = normalize(normal_matrix * vecNormal);
    vec3 lightVec   = normalize(light_pos.xyz - ecPosition);
    vec3 reflectVec = reflect(lightVec, tnorm);
    vec3 viewVec    = normalize(-ecPosition);

    float spec      = clamp(dot(reflectVec, viewVec), 0.0, 1.0);
    spec            = pow(spec, 8.0);
    Specular        = vec3(spec) * vec3(1.0, 0.941, 0.898) * 0.3;

    Diffuse         = max(dot(lightVec, tnorm), 0.0);

    // do standard vertex texture setup, considering texture matrix
    texCoord = vec2(matTexture * vec4(vecTexCoord, 0.0, 1.0));

    // set final vertex position
    gl_Position = matProjection * matModelView * vec4(vecPosition, 1.0);
}
