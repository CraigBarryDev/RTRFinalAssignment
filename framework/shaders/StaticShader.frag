#version 330 core

in vec2 pass_textureCoords;
in vec3 surfaceNormal;
in vec3 toLightVector;
in float visibility;

in vec3 toCameraVector;

out vec4 outColor;

uniform sampler2D textureSampler;
uniform vec3 lightColor;
uniform float shineDamper;
uniform float reflection;
uniform vec3 skyColor;

float near = 1.0; 
float far  = 100.0; 
float linearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main() {
	//Normalizing Normals
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);
	
	//Calculating diffuse light
	float diffDotProd = dot(unitNormal, unitLightVector);
	diffDotProd = max(diffDotProd, 0.2f);
	vec3 diffuse = diffDotProd * lightColor;

	//Calculating specular light
	vec3 unitVectorToCamera = normalize(toCameraVector);
	vec3 lightDirection = -unitLightVector;
	vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
	float specDotProd = dot(unitVectorToCamera, reflectedLightDirection);
	specDotProd = max(specDotProd, 0.0f);
	float dampedSpecBrightness = pow(specDotProd, shineDamper);
	vec3 specular = dampedSpecBrightness * reflection * lightColor;

	vec4 textureColor = texture(textureSampler, pass_textureCoords);
	if(textureColor.a < 0.5f) {
		discard;
	}

	//Final light
	outColor =  vec4(diffuse,1.0f) * textureColor + vec4(specular, 1.0f);
	// outColor = vec4(reflection);
	// outColor = vec4(specular, 1.0f);

	//Mix color with skycolor to produce fog
	//Mix function takes in two vectors and combines them based on the final parameter
	//last param 0 = entirely 2nd vector, last param 1 = entirely 1st vector
	// outColor = mix(vec4(skyColor, 1.0f), outColor, visibility);

	// outColor = textureColor;
	// outColor = vec4(1.0f,0.0f,0.0f,1.0f);
}