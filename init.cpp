#include "main.h"

//The loader used for loading in models/textures
extern Loader* loader;
EntityRenderer renderer;

//Shaders
StaticShader staticShader;
BackdropShader backShader;
ParticleShader particleShader;

//Textures
ModelTexture* ballTexture;
ModelTexture* blueTexture;
ModelTexture* greenTexture;
ModelTexture* orangeTexture;
ModelTexture* woodTexture;
ModelTexture* cannonTexture;
ModelTexture* particleTexture;

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

//Entity Lists
vector<Entity*> ballEntities;
vector<Entity*> cannonEntities;
vector<Entity*> peg3Entities;
vector<Entity*> peg4Entities;
vector<Entity*> peg5Entities;
vector<Entity*> peg6Entities;
unordered_map<GLuint, vector<Entity*>*> entities;

//Compiles shaders
void initShaders() {
	backShader = BackdropShader("framework/shaders/backdropShader.vert", "framework/shaders/backdropShader.frag");
	staticShader = StaticShader("framework/shaders/StaticShader.vert", "framework/shaders/StaticShader.frag");
	particleShader = ParticleShader("framework/shaders/ParticleShader.vert", "framework/shaders/ParticleShader.frag");
}

//Cleans up shader resources
void cleanUpShaders() {
	backShader.cleanUp();
	staticShader.cleanUp();
	particleShader.cleanUp();
}

//Loads textures
void initTextures() {
	blueTexture = new ModelTexture(loader->loadTexture("textures/blue.jpg"));
	blueTexture->setIsAnimated(true);

	ballTexture = new ModelTexture(loader->loadTexture("textures/metal.jpg"));
	ballTexture->setShineDamper(10.0f);
    ballTexture->setReflectivity(0.8f);

	cannonTexture = new ModelTexture(loader->loadTexture("textures/cannon.jpg"));

	greenTexture = new ModelTexture(loader->loadTexture("textures/green.jpg"));
	greenTexture->setIsAnimated(true);

	orangeTexture = new ModelTexture(loader->loadTexture("textures/orange.jpg"));
	orangeTexture->setIsAnimated(true);

	particleTexture = new ModelTexture(loader->loadTexture("textures/red_particle.png", true));
	particleTexture->setHasTransparency(true);

	woodTexture = new ModelTexture(loader->loadTexture("textures/wood.png"));
	woodTexture->setShineDamper(25.0f);
    woodTexture->setReflectivity(0.2f);
}

//Unloads textures
void cleanUpTextures() {
	delete ballTexture;
	delete blueTexture;
	delete cannonTexture;
	delete greenTexture;
	delete orangeTexture;
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

void initRenderer() {
	//Initialize the entitiy renderer
    renderer = EntityRenderer(&staticShader, projMatrix);

    //Initialize the cannon
    cannon.getEntity()->setPosition(vec3(0.0f, 14.0f, GAME_Z));
    cannonEntities.push_back(cannon.getEntity());
    std::pair<GLuint, vector<Entity*>*> cannonPair(cannon.getEntity()->getModel()->getRawModel()->getVAOID(), &cannonEntities);
    entities.insert(cannonPair);

    //Initialize balls
    int vaoID = ballTexModel->getRawModel()->getVAOID();
    std::pair<GLuint, vector<Entity*>*> ballPair(vaoID, &ballEntities);
    entities.insert(ballPair);
    
    //Initialize various peg types
    vaoID = pegPoly3->getVAOID();
    std::pair<GLuint, vector<Entity*>*> pegPolyPair3(vaoID, &peg3Entities);
    vaoID = pegPoly4->getVAOID();
    std::pair<GLuint, vector<Entity*>*> pegPolyPair4(vaoID, &peg4Entities);
    vaoID = pegPoly5->getVAOID();
    std::pair<GLuint, vector<Entity*>*> pegPolyPair5(vaoID, &peg5Entities);
    vaoID = pegPoly6->getVAOID();
    std::pair<GLuint, vector<Entity*>*> pegPolyPair6(vaoID, &peg6Entities);
    entities.insert(pegPolyPair3);
    entities.insert(pegPolyPair4);
    entities.insert(pegPolyPair5);
    entities.insert(pegPolyPair6);
}