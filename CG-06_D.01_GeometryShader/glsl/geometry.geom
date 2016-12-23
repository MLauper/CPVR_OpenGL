#version 400

layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;
//layout( line_strip, max_vertices = 3 ) out;

in vec4 vecSurfaceColor[]; 
out vec4 vecGSurfaceColor; 
noperspective out vec3 vecGEdgeDistance;

uniform mat4 matViewport;  // viewport matrix
uniform bool ubUseOffset;

const float offset_factor = 0.4;

void main()
{
    // transform each vertex into viewport space, since edge drawing is done in
	// fragment shader where we have screen space coordinates
    vec2 p0 = vec2(matViewport * (gl_in[0].gl_Position / gl_in[0].gl_Position.w));
    vec2 p1 = vec2(matViewport * (gl_in[1].gl_Position / gl_in[1].gl_Position.w));
    vec2 p2 = vec2(matViewport * (gl_in[2].gl_Position / gl_in[2].gl_Position.w));

    float a = length(p1 - p2);
    float b = length(p2 - p0);
    float c = length(p1 - p0);
    float alpha = acos( (b*b + c*c - a*a) / (2.0*b*c) );
    float beta = acos( (a*a + c*c - b*b) / (2.0*a*c) );
    float ha = abs( c * sin( beta ) );
    float hb = abs( c * sin( alpha ) );
    float hc = abs( b * sin( alpha ) );

    vec4 offset = vec4(0.0);

    if (ubUseOffset)
    {
        // consider vertex ordering!
        vec3 v1 = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
        vec3 v2 = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
        offset = vec4(normalize(cross(v1, v2)), 1.0);
    }

    vecGEdgeDistance = vec3( ha, 0, 0 );
    gl_Position = gl_in[0].gl_Position + offset_factor * offset;
    vecGSurfaceColor = vecSurfaceColor[0];
    EmitVertex();

    vecGEdgeDistance = vec3( 0, hb, 0 );
    gl_Position = gl_in[1].gl_Position + offset_factor * offset;
    vecGSurfaceColor = vecSurfaceColor[1];
    EmitVertex();

    vecGEdgeDistance = vec3( 0, 0, hc );
    gl_Position = gl_in[2].gl_Position + offset_factor * offset;
    vecGSurfaceColor = vecSurfaceColor[2];
    EmitVertex();

    EndPrimitive();
}
