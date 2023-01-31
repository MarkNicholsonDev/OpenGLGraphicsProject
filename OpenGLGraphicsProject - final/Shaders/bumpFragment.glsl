#version 330 core

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;
uniform vec3 cameraPos;

struct Light {
	vec4 lightColour;
	vec3 lightPos;
	float lightRadius;
};

layout (std140) uniform Lights {
	Light lights[4];
};

in Vertex {
    vec4 colour;
    vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
    vec3 worldPos;
} IN;

out vec4 fragColour;

void main(void) {
	vec4 finalColour = vec4(0, 0, 0, 1);
	vec4 diffuse = texture(diffuseTex, IN.texCoord);
	vec3 bumpNormal = texture(bumpTex,IN.texCoord).rgb;

	for(int i = 0; i < 4; ++i){
		vec3 incident = normalize(lights[i].lightPos - IN.worldPos);
		vec3 viewDir =  normalize(cameraPos - IN.worldPos);
		vec3 halfDir = normalize(incident + viewDir);

		mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));
		bumpNormal = normalize(TBN * normalize(bumpNormal * 2.0f - 1.0f));

		float lambert = max(dot(incident, IN.normal), 0.0f);
		float distance = length(lights[i].lightPos - IN.worldPos);
		float attenuation = 1.0 - clamp(distance / lights[i].lightRadius, 0.0, 1.0);

		float specFactor = clamp(dot(halfDir, bumpNormal), 0.0, 1.0);
		specFactor =  pow(specFactor, 60);

		vec3 surface = (diffuse.rgb * lights[i].lightColour.rgb);
		finalColour.rgb += surface * lambert * attenuation * lights[i].lightColour.a;
		finalColour.rgb += (lights[i].lightColour.rgb * specFactor) * attenuation * lights[i].lightColour.a;
	}

    fragColour = finalColour;
	fragColour.rgb += diffuse.rgb * 0.1;
	fragColour.a = diffuse.a;
}