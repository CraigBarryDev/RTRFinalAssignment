#include "litShader.h"

LitShader::LitShader()
	:
Shader()
{}

LitShader::LitShader(string vertexFile, string fragFile)
	:
Shader(vertexFile, fragFile)
{}

LitShader::~LitShader() {}

void LitShader::bindAttributes() {

}