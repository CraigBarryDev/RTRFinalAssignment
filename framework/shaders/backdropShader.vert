#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoords;
layout(location = 2) in vec3 normal;

out vec2 frag_texCoords;
out vec3 frag_normal;
out vec3 toLightVector;
out vec3 toCameraVector;

uniform vec3 lightPosition;

void main() {
	//Pass through values
	frag_texCoords = textureCoords;
	frag_normal = normal;
	//Position does not need to be translated
	gl_Position = vec4(position, 1.0f);

	//Camera vector will be the negation of position
	toCameraVector = -position;
	//Light vector goes from light to position of vertex
	toLightVector = lightPosition - position;
}