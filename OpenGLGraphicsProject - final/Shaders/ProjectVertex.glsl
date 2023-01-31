#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in vec3 position;
in vec4 colour;
in vec2 texCoord;
in vec2 blendmapTexCoord;
in vec3 normal;
in vec4 tangent;

out Vertex {
    vec4 colour;
    vec2 texCoord;
	vec2 texCoords[3];
	vec2 blendmapTexCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
    vec3 position;
} OUT;

void main(void) {
	OUT.texCoord = (vec4(texCoord, 0.0, 1.0)).xy;
	OUT.texCoords[0] = vec4(position, 1.0f).yz * 0.1;
	OUT.texCoords[1] = vec4(position, 1.0f).xz * 0.1;
	OUT.texCoords[2] = vec4(position, 1.0f).xy * 0.1;
	OUT.blendmapTexCoord =(vec4(blendmapTexCoord, 0.0, 1.0)).xy;
    OUT.colour = colour;

    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	vec3 wNormal = normalize(normalMatrix * normalize(normal));
	vec3 wTangent = normalize(normalMatrix * normalize(tangent.xyz));
	OUT.normal = wNormal;
	OUT.tangent = wTangent;
	OUT.binormal = cross(wTangent, wNormal) * tangent.w;

    vec4 position = (modelMatrix * vec4(position, 1));
	OUT.position = position.xyz;
	gl_Position = (projMatrix * viewMatrix) * position;
}