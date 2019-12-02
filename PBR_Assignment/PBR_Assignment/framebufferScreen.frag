#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float time;
const float offset= 1.0/300.0;

uniform float gamma;

uniform float exposure;

uniform int blurIntesity;
void main()
{

	   vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
//Edges 

//1,1,1,
1,-8,1,
1,1,1

//Default
0,0,0,
0,1,0,
0,0,0

//Blur
//1.0 / 16, 1 / 8, 1.0 / 16,
//1.0 / 8, 1.0 / blurIntesity, 1.0 / 8,
//1.0 / 16, 1.0 / 8, 1.0 / 16  

    );
     
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
	// Exposure tone mapping

    vec3 mapped =  vec3(1.0) - exp(-col * exposure);
	  // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
	 
}	