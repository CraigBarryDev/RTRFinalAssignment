#include "main.h"

//Compiles shaders
void initShaders() {
	litShader = LitShader("shaders/litShader.vert", "shaders/litShader.frag");
}

//Cleans up shader resources
void cleanUpShaders() {
	litShader.cleanUp();
}

//Generates/Loads any required models
void initModels() {

}