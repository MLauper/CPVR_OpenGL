// simple stripify fragment shader (core profile)

#version 400

vec4 toonify(in float intensity) 
{
	vec4 color;

	color = vec4(0.0, 0.0, sin(200.0 * intensity), 1.0);

	return(color);
}