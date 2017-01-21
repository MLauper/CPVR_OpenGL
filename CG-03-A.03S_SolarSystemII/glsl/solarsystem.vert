// simple modeling vertex shader code (core profile)

#version 400

layout (location = 0) in vec4 vecPosition;

uniform mat4 matModelView;
uniform mat4 matProjection;

void main()
{
	gl_Position = matProjection * matModelView * vecPosition;
}
