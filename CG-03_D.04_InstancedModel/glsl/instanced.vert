// instanced drawing vertex shader code (core profile)

#version 400

layout (location = 0) in vec4 vecPosition;
layout (location = 1) in vec4 vecColor;

uniform mat4 matModelView;
uniform mat4 matProjection;
uniform float floatOffset;

out vec4 color;

void main()
{
	color = vecColor;
	vec4 position = vecPosition + vec4(vec3(0.0f, 0.0f, gl_InstanceID*floatOffset), 1.0f);
	gl_Position = matProjection * matModelView * position;
}
