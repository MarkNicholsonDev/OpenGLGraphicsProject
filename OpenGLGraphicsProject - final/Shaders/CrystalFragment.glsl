#version 330 core

uniform sampler2D diffuseTex;
uniform sampler2D gradientTex;
uniform vec3 cameraPos;

struct Light {
	vec4 lightColour;
	vec3 lightPos;
	float lightRadius;
};

layout (std140) uniform Lights {
	Light lights[2];
};

in Vertex {
    vec4 colour;
    vec2 texCoord; //A y-axis sampler used for calculating blends
	vec3 normal;
    vec3 worldPos;
	vec2 texCoords[3]; //Gathers the xyz projections for triplanar mapping of textures
} IN;

out vec4 fragColour;

vec4 triplanarMapping(sampler2D tex){ // Takes in the texture & a individual scaling value for the texture
	vec3 blendVal = abs(IN.normal); 
	blendVal = normalize(max(blendVal, 0.00001));
	float temp = (blendVal.x + blendVal.y + blendVal.z);
	blendVal/vec3(temp, temp, temp);
	
	vec4 xProj = texture2D(tex, IN.texCoords[0]);
	vec4 yProj = texture2D(tex, IN.texCoords[1]);
	vec4 zProj = texture2D(tex, IN.texCoords[2]);
	vec4 finalProj = xProj * blendVal.x + yProj * blendVal.y + zProj * blendVal.z;
	return finalProj;
}

void main(void) {
	vec4 finalColour = vec4(0, 0, 0, 0.5);
	vec4 finalColour2 = vec4(0, 0, 0, 0.5);
	vec4 diffuse = triplanarMapping(diffuseTex);

	//Directional light
	vec3 incident = normalize(lights[0].lightPos - IN.worldPos);
	vec3 viewDir =  normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);

	float lambert = max(dot(incident, IN.normal), 0.0f);
	float distance = length(lights[0].lightPos - IN.worldPos);

	float specFactor = clamp(dot(halfDir, IN.normal), 0.0, 1.0);
	specFactor =  pow(specFactor, 60);

	vec3 surface = (diffuse.rgb * lights[0].lightColour.rgb);
	finalColour.rgb += surface * lambert * lights[0].lightColour.a; // Specular light below V is sampled from an rgb gradient tex to imitate iridescence
	finalColour.rgb += (lights[0].lightColour.rgb * (texture(gradientTex, IN.texCoord).rgb * specFactor)) * lights[0].lightColour.a;

	//Red glow light
	incident = normalize(lights[1].lightPos - IN.worldPos);
	viewDir =  normalize(cameraPos - IN.worldPos);
	halfDir = normalize(incident + viewDir);

	lambert = max(dot(incident, IN.normal), 0.0f);
	distance = length(lights[1].lightPos - IN.worldPos);
	float attenuation = 1.0 - clamp(distance / lights[1].lightRadius, 0.0, 1.0);

	specFactor = clamp(dot(halfDir, IN.normal), 0.0, 1.0);
	specFactor =  pow(specFactor, 60);

	surface = (diffuse.rgb * lights[1].lightColour.rgb);
	finalColour2.rgb += surface * lambert * attenuation * lights[1].lightColour.a;
	finalColour2.rgb += (lights[1].lightColour.rgb * specFactor) * attenuation * lights[1].lightColour.a;

    fragColour = finalColour + finalColour2;
	fragColour.rgb += diffuse.rgb * 0.1f;
	fragColour.r = 1.0f;
	fragColour.a = 0.9f;
}