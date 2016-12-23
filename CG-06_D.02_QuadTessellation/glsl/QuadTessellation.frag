#version 400


const float LineWidth = 0.8f;
const vec4 LineColor = vec4(0.05f,0.0f,0.95f,1.0f);
const vec4 QuadColor = vec4(1.0f,1.0f,1.0f,1.0f);

noperspective in vec3 EdgeDistance;

layout ( location = 0 ) out vec4 fragColor; // fragment’s final color

float edgeMix()
{
    // find the smallest distance
    float d = min( min( EdgeDistance.x, EdgeDistance.y ), EdgeDistance.z );

    if( d < LineWidth - 1 ) {
        return 1.0;
    } else if( d > LineWidth + 1 ) {
        return 0.0;
    } else {
        float x = d - (LineWidth - 1);
        return exp2(-2.0 * (x*x));
    }
}

void main()
{
    float mixVal = edgeMix();

    if( gl_FrontFacing )
        fragColor = mix( QuadColor, LineColor, mixVal );
    else
        fragColor = vec4(0.75,0.75,0.75,1.0);
}
