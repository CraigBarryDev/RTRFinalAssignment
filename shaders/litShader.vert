#version 130

in vec3 position;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec3 fragPos;

void main() {
	gl_Position = projMatrix * viewMatrix * vec4(position.xyz, 1.0f);

	fragPos = vec3(gl_Position.xyz);
}
