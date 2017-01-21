// Fragment shader for drawing the earth with three textures
// (Based on OpenGL Shading Language book example)

#version 400

void doNormalMapping(inout vec3 color);

uniform sampler2D EarthDay;
uniform sampler2D EarthNight;
uniform sampler2D EarthCloudGloss;

uniform bool NoClouds;
uniform bool DoNormalMap;

in float Diffuse;
in vec3  Specular;
in vec2  texCoord;

layout (location = 0) out vec4 fragColor; // fragment’s final color

void main(void)
{
    // Monochrome cloud cover value will be in clouds.r
    // Gloss value will be in clouds.g, clouds.b will be unused
    
    vec2 clouds    = texture(EarthCloudGloss, texCoord).rg;
  
    if (NoClouds) clouds.r = 0.0;  // check if clouds need to be drawn
    
    vec3 daytime   = (texture(EarthDay, texCoord).rgb * Diffuse + Specular * clouds.g)
                     * (1.0 - clouds.r) + clouds.r * Diffuse;
                                    
    vec3 nighttime = texture(EarthNight, texCoord).rgb * (1.0 - clouds.r) * 2.0;

    if (DoNormalMap) doNormalMapping(daytime);   // check if normal map needs to be drawn
    
    vec3 color = daytime;

    if (Diffuse < 0.1)
        color = mix(nighttime, daytime, (Diffuse + 0.1) * 5.0);

    // write out final fragment color
    fragColor = vec4(color, 1.0);
}