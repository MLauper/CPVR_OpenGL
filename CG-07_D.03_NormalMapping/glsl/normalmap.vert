// Vertex shader for normal mapping in tangent space coordinates
// (Based on OpenGL Shading Language book example)


#version 400

uniform mat4 matModelView;
uniform mat4 matProjection;
uniform mat4 matTexture;

uniform vec4 vecLightPosition;

layout(location = 0) in vec3 vecPosition;
layout(location = 1) in vec3 vecNormal;
layout(location = 2) in vec2 vecTexCoord;

out vec3 lightDir;    // interpolated surface local coordinate light direction 
out vec3 viewDir;     // interpolated surface local coordinate view direction
out vec2 texCoord;


void main(void)
{
    // calculate normal transformation matrix
    mat3 normal_matrix = transpose(inverse(mat3(matModelView)));

    // transform the input vectors and calculate base of tangent space
    vec3 n = normalize(normal_matrix * vecNormal);
    vec3 t = normalize(vec3(1.0, 1.0, -((n.x + n.y)/n.z)));
    vec3 b = normalize(cross(n, t)); // Binormal

    // transform vertex position into surface local coordinates
    vec3 v;
    vec3 pos = vec3(matModelView * vec4(vecPosition, 1.0));
    v.x = dot(pos, t);
    v.y = dot(pos, b);
    v.z = dot(pos, n);
    viewDir = normalize(v);

    // transform light position into surface local coordinates
    v.x = dot(vecLightPosition.xyz-pos, t);
    v.y = dot(vecLightPosition.xyz-pos, b);
    v.z = dot(vecLightPosition.xyz-pos, n);
    lightDir = normalize(v);

    // do standard vertex texture setup, considering texture matrix
    texCoord = vec2(matTexture * vec4(vecTexCoord, 0.0, 1.0));

    // set final vertex position
    gl_Position = matProjection * matModelView * vec4(vecPosition, 1.0);
}
