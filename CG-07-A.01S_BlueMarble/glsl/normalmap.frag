// Fragment shader for normal mapping in tangent space coordinates
// (Based on OpenGL Shading Language book example)

#version 400

uniform sampler2D NormalMap;
uniform float DiffuseFactor;
uniform float SpecularFactor;

in vec3 lightDir;  // interpolated surface local coordinate light direction 
in vec3 viewDir;   // interpolated surface local coordinate view direction
in vec2 texCoord;


void doNormalMapping(inout vec3 color)
{
    // fetch normal from normal map and compute diffuse reflection component
    vec3 norm = vec3(texture(NormalMap, texCoord.st));
    norm = (norm - 0.5) * 2.0;
    norm = normalize(norm);
    float intensity = max(dot(normalize(lightDir), norm), 0.0) * DiffuseFactor;

    // compute specular reflection component
    vec3 reflDir = reflect(normalize(lightDir), norm);
    float   spec = pow(max(dot(reflDir, normalize(viewDir)), 0.0) , 6.0) * SpecularFactor;
    intensity   += min(spec, 2.0);

    // compute color value to return
    color = clamp(color * intensity, 0.0, 1.0);
}
