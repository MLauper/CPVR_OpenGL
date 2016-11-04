// simple toonify rendering vertex shader code (core profile)

#version 400

layout (location = 0) in vec4 vecPosition;
layout (location = 1) in vec4 vecNormal;

uniform mat4 matModelView;
uniform mat4 matProjection;

out vec3 lightDir;
out vec3 normal; 

void main()
{
	lightDir = normalize(vec3(10.0, 10.0, 20.0));

	// transform vertex normal into eye coordinates using the normal matrix
	// normal = normalize(gl_NormalMatrix * vecNormal);
	mat3 normal_matrix = transpose( inverse( mat3(matModelView) ));
	normal = normalize( normal_matrix * vec3(vecNormal) );

	// set final vertex position
	gl_Position = matProjection * matModelView * vecPosition;
}