// phong shading fragment shader 
// (for one light only and without emissive component)

#version 420 core

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
// define struct for light properties
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

// define struct for material properties
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

in vec3 Vertex;    
in vec3 Normal;

vec4 doPhongShading(void) 
{  
   // setup vectors needed for phong illumination calculation
   
   vec3 N = normalize(Normal);

   // calculate vector from vertex to point light source
   vec3 L = normalize(ubLightSource.position.xyz - Vertex); 

   
   // calculate vector from vertex to viewing point   
   // (we are in eye coordinates, so eye position is (0,0,0)) 
   vec3 V = normalize(Vertex); 
   
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
   
   // return total fragment color:  
   return Itotal;   
}