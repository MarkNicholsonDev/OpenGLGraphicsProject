#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in vec3 position;
in vec4 colour;
in vec3 normal;
in vec2 texCoord;

out Vertex {
	vec4 colour;
    vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
	vec2 texCoords[3];
} OUT;

void main(void) {
	OUT.texCoords[0] = vec4(position, 1.0f).yz * 0.1;
	OUT.texCoords[1] = vec4(position, 1.0f).xz * 0.1;
	OUT.texCoords[2] = vec4(position, 1.0f).xy * 0.1;

	OUT.colour = colour;
	OUT.texCoord = texCoord;

	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	OUT.normal = normalize(normalMatrix*normalize(normal));

    vec4 worldPos = (modelMatrix * vec4(position, 1));

	OUT.worldPos = worldPos.xyz;

	gl_Position = (projMatrix * viewMatrix) * worldPos;
}