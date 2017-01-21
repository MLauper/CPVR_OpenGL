// simple toonify rendering fragment shader code (core profile)

#version 400

in vec3 lightDir;
in vec3 normal; 
out vec4 fragColor; // fragment’s final color constant

vec4 toonify(in float intensity);

void main()
{
	float intensity;
	vec3 n;

	n = normalize(normal);
	intensity = max(dot(lightDir,n), 0.0); 

	fragColor = toonify(intensity);
}