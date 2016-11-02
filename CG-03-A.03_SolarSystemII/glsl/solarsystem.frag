// simple modeling fragment shader code (core profile)

#version 400


uniform vec3 vecColor;

out vec4 fragColor; // fragment’s final color

void main()
{
	fragColor = vec4(vecColor, 1.0);
}

