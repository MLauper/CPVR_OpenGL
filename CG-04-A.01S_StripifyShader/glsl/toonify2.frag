// simple toonify fragment shader color set2 (core profile)

#version 400

vec4 toonify(in float intensity) 
{
	vec4 color;

	if (intensity > 0.95)
		color = vec4(0.9, 0.6, 0.6, 1.0);
	else if (intensity > 0.85)
		color = vec4(0.8, 0.5, 0.5, 1.0);	
	else if (intensity > 0.65)
		color = vec4(0.7, 0.4, 0.4, 1.0);	
	else if (intensity > 0.5)
		color = vec4(0.6, 0.3, 0.3, 1.0);	
	else if (intensity > 0.25)
		color = vec4(0.5, 0.2, 0.2, 1.0);	
	else if (intensity > 0.12)
		color = vec4(0.4, 0.1, 0.1, 1.0);
	else
		color = vec4(0.2, 0.0, 0.0, 1.0);		

	return(color);
}