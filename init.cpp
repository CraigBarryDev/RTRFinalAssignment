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
NSidedPolygon pegPolyModel3;
NSidedPolygon pegPolyModel4;
NSidedPolygon pegPolyModel5;
NSidedPolygon pegPolyModel6;
RawModel* pegPoly3;
RawModel* pegPoly4;
RawModel* pegPoly5;
RawModel* pegPoly6;

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
	ballTexture->setShineDamper(10.0f);
    ballTexture->setReflectivity(0.8f);

	cannonTexture = new ModelTexture(loader->loadTexture("textures/cannon.jpg"));

	woodTexture = new ModelTexture(loader->loadTexture("textures/wood.png"));
	woodTexture->setShineDamper(25.0f);
    woodTexture->setReflectivity(0.2f);
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
	Sphere sphereMesh = Sphere(20, 20, BALL_SIZE);
	ballModel = loader->loadToVAO(sphereMesh.vertices, sphereMesh.normals, sphereMesh.texCoords, sphereMesh.indices);
	ballTexModel = new TexturedModel(ballModel, ballTexture);
	//Initialize peg models
	pegPolyModel3 = NSidedPolygon(3, 1.0f, 1.0f);
	pegPoly3 = loader->loadToVAO(pegPolyModel3.vertices, pegPolyModel3.normals, pegPolyModel3.texCoords);
	pegPolyModel4 = NSidedPolygon(4, 1.0f, 1.0f);
	pegPoly4 = loader->loadToVAO(pegPolyModel4.vertices, pegPolyModel4.normals, pegPolyModel4.texCoords);
	pegPolyModel5 = NSidedPolygon(5, 1.0f, 1.0f);
	pegPoly5 = loader->loadToVAO(pegPolyModel5.vertices, pegPolyModel5.normals, pegPolyModel5.texCoords);
	pegPolyModel6 = NSidedPolygon(6, 1.0f, 1.0f);
	pegPoly6 = loader->loadToVAO(pegPolyModel6.vertices, pegPolyModel6.normals, pegPolyModel6.texCoords);
}

//Cleans up model resources
void cleanUpModels() {
	backdrop.cleanUp();
	cannon.cleanUp();

	delete ballModel;
	delete ballTexModel;
	delete pegPoly3;
	delete pegPoly4;
	delete pegPoly5;
	delete pegPoly6;
}