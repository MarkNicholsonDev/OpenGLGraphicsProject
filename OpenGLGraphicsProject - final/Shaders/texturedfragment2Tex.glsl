#version 330 core

uniform sampler2D diffuseTex;
uniform sampler2D diffuseTex2;
uniform float blendColours;

in Vertex{
	vec2 texCoord;
	vec4 colour; // Unused but can be used to mix in RGB
} IN;

out vec4 fragColour;

void main(void)	{
	fragColour = mix(texture(diffuseTex, IN.texCoord), texture(diffuseTex2, IN.texCoord), 1);
}