#version 400 core
uniform sampler2D diffuseTexs[3];

uniform float minHeight;
uniform float maxHeight;

struct TexturingData{
	float startHeights[3];
	float blendStrengths[3];
	float colourStrengths[3];
	vec3 colours[3];
};

layout (std140)uniform TextureData {
	TexturingData textureData;
};

float inverseLerp(float a, float b, float value) {
	return clamp((value - a) / (b - a), 0.0, 1.0);
}

in Vertex {
    vec4 colour;
    vec2 texCoord;
    vec3 position;
} IN;

out vec4 fragColour;

void main(void) {
	float heightPercent = inverseLerp(minHeight, maxHeight, IN.position.y);
	const float epsilon = 1E-4;
	TexturingData texturingData;
	fragColour = texture(diffuseTexs[0],IN.texCoord);

	for(int i = 0; i < 1; ++i) {
		float drawStrength = inverseLerp(-texturingData.blendStrengths[i]/2 - epsilon, texturingData.blendStrengths[i]/2, heightPercent - texturingData.startHeights[i]);
		vec3 baseColour = texturingData.colours[i] * texturingData.colourStrengths[i];
		vec4 textureColour = texture(diffuseTexs[i],IN.texCoord);
		fragColour = mix(vec4(baseColour + textureColour.rgb, textureColour.w), texture(diffuseTexs[i+1],IN.texCoord), drawStrength);
	}


}