// gouraud shading vertex shader
// (for one light only and without emissive component)

#version 400


#if (__VERSION__ == 420)
// define uniform block for light properties
layout (std140, binding = 2) uniform LightProperties
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
} ubLightSource;

// define uniform block for material properties
layout (std140, binding = 4) uniform MaterialProperties
{
	vec4 ambient;		// what part of ambient light is reflected
	vec4 diffuse;		// what part of diffuse light is scattered
	vec4 specular;		// what part of specular light is scattered
	float shininess;	// exponent for sharpening specular reflection
} ubMaterial;
#endif


#if (__VERSION__ == 400)
// define struct for light properties (constants)
struct LightProperties
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};
LightProperties ubLightSource = LightProperties( 
	vec4(0.0, 0.0, 20.0, 1.0),
	vec4(0.8, 0.8,  0.8, 1.0),
	vec4(0.8, 0.8,  0.8, 1.0),
	vec4(1.0, 1.0,  1.0, 1.0));

// define struct for material properties (constants)
struct MaterialProperties
{
	vec4 ambient;		// what part of ambient light is reflected
	vec4 diffuse;		// what part of diffuse light is scattered
	vec4 specular;		// what part of specular light is scattered
	float shininess;	// exponent for sharpening specular reflection
};
MaterialProperties ubMaterial = MaterialProperties(
	vec4(0.1, 0.1, 0.1, 1.0),
	vec4(0.1, 0.1, 0.9, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0), 
	64.0);
#endif

layout (location = 0) in vec3 vecPosition;
layout (location = 1) in vec3 vecNormal;

uniform mat4 matModelView;
uniform mat4 matProjection;

//flat out vec4 vecSurfaceColor; 
out vec4 vecSurfaceColor; 

void main(void)
{
 	// calculate eye coordinates of vertex- and normal vectors, because 
	// specular reflection is calculated in eye coordinates 
	vec3 ec_position = mat3(matModelView) * vecPosition;

	// transform vertex normal into eye coordinates
	mat3 normal_matrix = transpose( inverse( mat3(matModelView) ) );
	vec3 N = normalize( normal_matrix * vecNormal );

	// calculate vector from vertex to point light source
	vec3 L = normalize(ubLightSource.position.xyz - ec_position);

	// calculate vector from vertex to viewing point   
	// (we are in eye coordinates, so eye position is (0,0,0)) 
	vec3 V = normalize(ec_position);

	// calculate reflection vector using built-in function reflect()
	// (reflect() requires incident direction of point light vector, so reverse direction)
	vec3 R = normalize(reflect(-L, N));

	// calculate ambient term using light and material components:  
	vec4 Iamb = ubLightSource.ambient * ubMaterial.ambient;

	// calculate diffuse term using light and material components:  
	vec4 Idiff = ubLightSource.diffuse * ubMaterial.diffuse * max(dot(L, N), 0.0);    

	// calculate specular term using light and material components:
	vec4 Ispec = ubLightSource.specular * ubMaterial.specular * pow( max(dot(R, V), 0.0), ubMaterial.shininess );

	// summation of total light intensity (ignoring emissive component)
	vec4 Itotal = Iamb + Idiff + Ispec;

	// write total fragment color:  
	vecSurfaceColor = Itotal;   

	// set final vertex position
	gl_Position = matProjection * matModelView * vec4(vecPosition, 1.0);
}