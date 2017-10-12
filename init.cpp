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
RawModel* ballModel;
TexturedModel* ballTexModel;

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
	//Initialize the cannon
	cannon = Cannon(loader, cannonTexture);
	//Initialize the ball model
	Sphere sphereMesh = Sphere(20, 20, 0.9f);
	ballModel = loader->loadToVAO(sphereMesh.vertices, sphereMesh.normals, sphereMesh.texCoords, sphereMesh.indices);
	ballTexModel = new TexturedModel(ballModel, ballTexture);
}

//Cleans up model resources
void cleanUpModels() {
	backdrop.cleanUp();
	cannon.cleanUp();

	delete ballModel;
	delete ballTexModel;
}