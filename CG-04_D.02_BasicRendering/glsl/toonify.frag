// simple toonify rendering fragment shader code (core profile)

#version 400

vec4 toonify(in float intensity);

in vec3 lightDir;
in vec3 normal; 
out vec4 fragColor; // fragment’s final color


void main(void)
{
	float intensity;
	vec3 n;

	n = normalize(normal);
	intensity = max(dot(lightDir,n), 0.0); 

	fragColor = toonify(intensity);
}