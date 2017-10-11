#include "main.h"

//The loader used for loading in models/textures
extern Loader* loader;

//Shaders
StaticShader staticShader;
BackdropShader backShader;
//Textures
ModelTexture* ballTexture;
ModelTexture* woodTexture;
//Models
Backdrop backdrop;

//Compiles shaders
void initShaders() {
	backShader = BackdropShader("framework/shaders/backdropShader.vert", "framework/shaders/backdropShader.frag");
	staticShader = StaticShader("framework/shaders/StaticShader.vert", "framework/shaders/StaticShader.frag");
}

//Cleans up shader resources
void cleanUpShaders() {
	staticShader.cleanUp();
}

//Loads textures
void initTextures() {
	ballTexture = new ModelTexture(loader->loadTexture("textures/metal.jpg"));
	woodTexture = new ModelTexture(loader->loadTexture("textures/wood.png"));
}

//Unloads textures
void cleanUpTextures() {
	delete ballTexture;
	delete woodTexture;
}

//Generates/Loads any required models
void initModels() {
	backdrop = Backdrop(loader, woodTexture, &backShader);
}

//Cleans up model resources
void cleanUpModels() {
	
}