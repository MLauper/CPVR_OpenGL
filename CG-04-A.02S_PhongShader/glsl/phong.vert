// phong shading vertex shader

#version 400 core

uniform mat4 matModelView;
uniform mat4 matProjection;

layout(location = 0) in vec3 vecPosition;
layout(location = 1) in vec3 vecNormal;

out vec3 Vertex;
out vec3 Normal;

void doPhongShading(void)
{
 	// calculate eye coordinates of vertex- and normal vectors, because 
	// specular reflection is calculated in eye coordinates 
	Vertex = mat3(matModelView) * vecPosition;      
    
	// transform normals with calculated normal_matrix (only needed for non-uniform scaling)
	mat3 normal_matrix = transpose( inverse( mat3(matModelView) ) );
	Normal = normalize( normal_matrix * vecNormal );
}
