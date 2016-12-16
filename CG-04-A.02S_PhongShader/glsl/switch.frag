// switch fragment shader 
// (for one light only and without emissive component)

#version 400 core

vec4 doPhongShading(void); 
vec4 doGouraudShading(void);
uniform bool uSwitchPhongShader;

out vec4 fragColor; // fragment’s final color

void main(void)  
{  
	if (uSwitchPhongShader)
	{
	   fragColor = doPhongShading();   
	}
	else
	{
	   fragColor = doGouraudShading();   
	}
}