#version 400 core

uniform sampler2D diffuseTexs[3];
uniform sampler2D bumpTexs[3];
uniform sampler2D blendmap;
uniform vec3 cameraPos;

struct Light {
	vec4 lightColour;
	vec3 lightPos;
	float lightRadius;
};

layout (std140) uniform Lights { //made to support additional lights within the scene
	Light lights[2];
};

in Vertex {
    vec4 colour;
    vec2 texCoord; //A y-axis sampler used for calculating blends
	vec2 texCoords[3]; //Gathers the xyz projections for triplanar mapping of textures
	vec2 blendmapTexCoord;
    vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 position;
} IN;

out vec4 fragColour;

vec4 triplanarMapping(sampler2D tex, float textureScale){ // Takes in the texture & a individual scaling value for the texture
	vec3 blendVal = abs(IN.normal);
	blendVal = normalize(max(blendVal, 0.00001));
	float temp = (blendVal.x + blendVal.y + blendVal.z);
	blendVal/vec3(temp, temp, temp);
	
	vec4 xProj = texture2D(tex, IN.texCoords[0] * textureScale);
	vec4 yProj = texture2D(tex, IN.texCoords[1] * textureScale);
	vec4 zProj = texture2D(tex, IN.texCoords[2] * textureScale);
	vec4 finalProj = (xProj * blendVal.x + yProj * blendVal.y + zProj * blendVal.z);
	return finalProj;
}

void main(void) {
	vec4 finalColour = vec4(0, 0, 0, 1);
    vec2 scaleVector = vec2(29.5,29); // To be made into an input to allow for varied heightmaps to be automatically scaled with textures (scales blendmap)
	vec3 blends = normalize(texture(blendmap, vec2(IN.texCoord.x/ scaleVector.x, IN.texCoord.y/ scaleVector.y)).rgb);
	vec4 diffuse = vec4(blends, 1); // The lower the scaling value below VVV the larger the size of the texture on the mesh
	diffuse = (blends.g * triplanarMapping(diffuseTexs[0], 0.5f)) + (blends.r * triplanarMapping(diffuseTexs[1], 0.5f)) + (blends.b * triplanarMapping(diffuseTexs[2], 0.1f));
	
	//Left here for testing without triplanar mapping (along with the bump normal line)
	//diffuse = (blends.g * texture(diffuseTexs[0], IN.texCoord)) + (blends.r * texture(diffuseTexs[1], IN.texCoord)) + (blends.b * texture(diffuseTexs[2], IN.texCoord));
	
	vec3 bumpNormal = vec3(blends); // Make sure the bumps below have the same texture scale
	bumpNormal = ((blends.g * triplanarMapping(bumpTexs[0], 0.5f)) + (blends.r * triplanarMapping(bumpTexs[1], 0.5f)) + (blends.b * triplanarMapping(bumpTexs[2], 0.1f))).rgb;
	//bumpNormal = ((blends.g * texture(bumpTexs[0], IN.texCoord)) + (blends.r * texture(bumpTexs[1], IN.texCoord)) + (blends.b * texture(bumpTexs[2], IN.texCoord))).rgb;
	
	//Directional light
	vec3 incident = normalize(lights[0].lightPos - IN.position);
	vec3 viewDir =  normalize(cameraPos - IN.position);
	vec3 halfDir = normalize(incident + viewDir);

	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));
	bumpNormal = normalize(TBN * normalize(bumpNormal * 2.0f - 1.0f));

	float lambert = max(dot(incident, bumpNormal), 0.0f);
	float distance = length(lights[0].lightPos - IN.position);
	float specFactor = clamp(dot(halfDir, bumpNormal), 0.0, 1.0);
	specFactor =  pow(specFactor, 60);
	vec3 surface = (diffuse.rgb * lights[0].lightColour.rgb);
	finalColour.rgb += surface * lambert * lights[0].lightColour.a;
	finalColour.rgb += (lights[0].lightColour.rgb * specFactor) * 0.33 * lights[0].lightColour.a;

	//Point light
	incident = normalize(lights[1].lightPos - IN.position);
	viewDir =  normalize(cameraPos - IN.position);
	halfDir = normalize(incident + viewDir);

	lambert = max(dot(incident, IN.normal), 0.0f);
	distance = length(lights[1].lightPos - IN.position);
	float attenuation = 1.0 - clamp(distance / lights[1].lightRadius, 0.0, 1.0);

	specFactor = clamp(dot(halfDir, IN.normal), 0.0, 1.0);
	specFactor =  pow(specFactor, 60);

	surface = (diffuse.rgb * lights[1].lightColour.rgb);
	finalColour.rgb += surface * lambert * attenuation * lights[1].lightColour.a;
	finalColour.rgb += (lights[1].lightColour.rgb * specFactor) * attenuation * lights[1].lightColour.a;

	fragColour = finalColour;
	fragColour.rgb += diffuse.rgb * 0.1;
	fragColour.a = diffuse.a;
}