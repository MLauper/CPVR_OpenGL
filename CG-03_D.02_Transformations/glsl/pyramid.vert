// simple modeling vertex shader code (core profile)

#version 400

layout (location = 0) in vec4 vecPosition;
layout (location = 1) in vec4 vecColor;

uniform mat4 matModelView;
uniform mat4 matProjection;

out vec4 color;

void main()
{
	color = vecColor;
	gl_Position = matProjection * matModelView * vecPosition;
}
