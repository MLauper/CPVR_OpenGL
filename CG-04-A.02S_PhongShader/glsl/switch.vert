// switch vertex shader

#version 400 core

void doPhongShading(void);
void doGouraudShading(void);

uniform bool uSwitchPhongShader;

uniform mat4 matModelView;
uniform mat4 matProjection;

layout(location = 0) in vec3 vecPosition;

void main(void)
{
	if (uSwitchPhongShader)
	{
	   doPhongShading();   
	}
	else
	{
	   doGouraudShading();   
	}

	// set final vertex position
	gl_Position = matProjection * matModelView * vec4(vecPosition, 1.0);
}
