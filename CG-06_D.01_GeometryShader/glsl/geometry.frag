// gouraud shading fragment shader
// (for one light only and without emissive component)

#version 400

in vec4 vecGSurfaceColor;
noperspective in vec3 vecGEdgeDistance;

uniform bool ubUseWireLines;

layout (location = 0) out vec4 fragColor; // fragment’s final color

const float LineWidth = 0.5f;
const vec4 LineColor = vec4(0.05f, 0.0f, 0.05f, 1.0f);


void main(void)
{
    fragColor = vecGSurfaceColor;

    if (ubUseWireLines)
    {
		// find the smallest distance
		float d =  min( min( vecGEdgeDistance.x, vecGEdgeDistance.y ), vecGEdgeDistance.z );

		// if (d < LineWidth - 4) --> mixVal = 1.0
		// if (d > LineWidth + 4) --> mixVal = 0.0
		// else --> interpolate
		float mixVal = smoothstep(LineWidth - 4, LineWidth + 4, d);

        fragColor = mix( LineColor, vecGSurfaceColor, mixVal );
    }
}