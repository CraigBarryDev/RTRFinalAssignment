#include "main.h"

//The loader used for loading in models/textures
extern Loader* loader;

//Shaders
StaticShader staticShader;
BackdropShader backShader;
//Textures
ModelTexture* ballTexture;
ModelTexture* woodTexture;
ModelTexture* cannonTexture;
//Models
Backdrop backdrop;
Cannon cannon;

//Compiles shaders
void initShaders() {
	backShader = BackdropShader("framework/shaders/backdropShader.vert", "framework/shaders/backdropShader.frag");
	staticShader = StaticShader("framework/shaders/StaticShader.vert", "framework/shaders/StaticShader.frag");
}

//Cleans up shader resources
void cleanUpShaders() {
	backShader.cleanUp();
	staticShader.cleanUp();
}

//Loads textures
void initTextures() {
	ballTexture = new ModelTexture(loader->loadTexture("textures/metal.jpg"));
	cannonTexture = new ModelTexture(loader->loadTexture("textures/cannon.jpg"));
	woodTexture = new ModelTexture(loader->loadTexture("textures/wood.png"));
}

//Unloads textures
void cleanUpTextures() {
	delete ballTexture;
	delete cannonTexture;
	delete woodTexture;
}

//Generates/Loads any required models
void initModels() {
	//Initializes the backdrop
	backdrop = Backdrop(loader, woodTexture, &backShader);
	cannon = Cannon(loader, cannonTexture);
}

//Cleans up model resources
void cleanUpModels() {
	backdrop.cleanUp();
	cannon.cleanUp();
}