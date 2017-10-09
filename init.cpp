#include "main.h"

//The loader used for loading in models/textures
extern Loader* loader;

//Shaders
StaticShader staticShader;
//Textures
ModelTexture* woodTexture;

//Compiles shaders
void initShaders() {
	staticShader = StaticShader("framework/shaders/StaticShader.vert", "framework/shaders/StaticShader.frag");
}

//Cleans up shader resources
void cleanUpShaders() {
	staticShader.cleanUp();
}

//Loads textures
void initTextures() {
	woodTexture = new ModelTexture(loader->loadTexture("textures/metal.jpg"));
	// woodTexture = new ModelTexture(1);
}

//Unloads textures
void cleanUpTextures() {
	delete woodTexture;
}

//Generates/Loads any required models
void initModels() {

}

//Cleans up model resources
void cleanUpModels() {
	
}