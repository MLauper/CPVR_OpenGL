// Fragment shader for normal mapping in tangent space coordinates
// (Based on OpenGL Shading Language book example)

#version 400

uniform sampler2D NormalMap;
uniform float DiffuseFactor;
uniform float SpecularFactor;
uniform vec3  BaseColor;

in vec3 lightDir;  // interpolated surface local coordinate light direction 
in vec3 viewDir;   // interpolated surface local coordinate view direction
in vec2 texCoord;

layout (location = 0) out vec4 fragColor; // fragment’s final color

void main(void)
{
    // fetch normal from normal map and compute diffuse reflection component
    vec3 norm = vec3(texture(NormalMap, texCoord));
    norm = (norm - 0.5) * 2.0;
    // norm.x = -norm.x;    // set concave/convex
    norm = normalize(norm);
    float intensity = max(dot(lightDir, norm), 0.0) * DiffuseFactor;

    // compute specular reflection component
    vec3 reflDir = reflect(lightDir, norm);
    float   spec = pow(max(dot(reflDir, viewDir), 0.0) , 6.0) * SpecularFactor;
    intensity   += min(spec, 2.0);

    // compute color value to return
    vec3 color = clamp(BaseColor * intensity, 0.0, 1.0);
	 
    // write out final fragment color
    fragColor = vec4(color, 1.0);
}
