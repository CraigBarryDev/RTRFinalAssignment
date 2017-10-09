#include "main.h"

#include <vector>
#include <unordered_map>
#include "framework/loader.h"
#include "framework/GLHeaders.h"

unordered_map<GLuint, vector<Entity*>> entities;
std::vector<Entity*> entitiesOfSameType;
EntityRenderer renderer;
Entity* ent1;
Entity* ent2;

#define N_BALLS 25
vec3 ballVelocs[N_BALLS];
const float ballSpeed = 1.0f;

//Main initialization
void init(void) {
	//Creates the loading object
	loader = new Loader();

	//Initialize game resources
	initModels();
    initShaders();
    initTextures();
    Cylinder cylinderMesh = Cylinder(20, 20, 1.0f);
    Sphere sphereMesh = Sphere(20, 20, 1.0f);
    
    RawModel* model = loader->loadToVAO(sphereMesh.vertices,
        sphereMesh.normals, sphereMesh.texCoords, sphereMesh.indices);

    woodTexture->setShineDamper(10.0f);
    woodTexture->setReflectivity(0.8f);

    TexturedModel* tm = new TexturedModel(model, woodTexture);
    ent1 = new Entity(tm);
    int vaoID = ent1->getModel()->getRawModel()->getVAOID();

    for(int i = 0; i < N_BALLS; i++) {
        Entity* ent = new Entity(tm);
        ent->setPosition(vec3(0.0f, 0.0f, -25.0f));
        ent->setScale(1.0f);
        entitiesOfSameType.push_back(ent);

        //Set the ball's initial velocity
        ballVelocs[i] = ballSpeed * normalize(vec3(Maths::randBetweenf(-1.0f, 1.0f), 
            Maths::randBetweenf(-1.0f, 1.0f), 
            0.0f));
        ballVelocs[i] = ballSpeed * normalize(vec3(Maths::randBetweenf(-1.0f, 1.0f), 
            Maths::randBetweenf(-1.0f, 1.0f), 
            Maths::randBetweenf(-1.0f, 1.0f)));
        ent->setPosition(vec3(Maths::randBetweenf(-5.0f, 5.0f), Maths::randBetweenf(-5.0f, 5.0f), -15.5f));
    }

    std::pair<GLuint, vector<Entity*>> newMapPair(vaoID, entitiesOfSameType);
    entities.insert(newMapPair);
    renderer = EntityRenderer(&staticShader, projMatrix);
}

void collisionReaction(vec3* pos1, vec3* vel1, vec3* pos2, vec3* vel2) {
    vec3 collisionNormal = normalize(*pos2 - *pos1);
    float initVel1 = dot(*vel1, collisionNormal);
    float initVel2 = dot(*vel2, collisionNormal);

    float mass1 = 1.0f;
    float mass2 = 1.0f;
    float finalVel1 = (mass1 - mass2) / (mass1 + mass2) * initVel1 + 2.0f * mass2 / (mass1 + mass2) * initVel2;
    float finalVel2 = 2.0f * mass1 / (mass1 + mass2) * initVel1 + (mass2 - mass1) / (mass1 + mass2) * initVel2;

    *vel1 = (*vel1 - vec3(initVel1) * collisionNormal) + vec3(finalVel1) * collisionNormal;
    *vel2 = (*vel2 - vec3(initVel2) * collisionNormal) + vec3(finalVel2) * collisionNormal;
}

//Determines if two balls are colliding or not
bool ballsColliding(vec3 pos1, vec3 pos2) {
    return Maths::distBetween(pos1, pos2) <= 2.0f;
}


//Update function called before each draw call to update program state
void update(void) {
    handleMouse();
    handleKeyboard();

    for(int i = 0; i < N_BALLS; i++) {
        for(int j = i + 1; j < N_BALLS; j++) {
            Entity* ent1 = entitiesOfSameType[i];
            Entity* ent2 = entitiesOfSameType[j];
            if(ballsColliding(ent1->getPosition(), ent2->getPosition())) {
                // printf("BALLS COLLIDING\n");
                vec3 pos1 = ent1->getPosition();
                vec3 pos2 = ent2->getPosition();
                // ent1->increasePosition(-ballVelocs[i] * getFrameTime() * 5.0f);
                // ent2->increasePosition(-ballVelocs[j] * getFrameTime() * 5.0f);
                collisionReaction(&pos1, &ballVelocs[i], &pos2, &ballVelocs[j]);
                ent1->increasePosition(ballVelocs[i] * getFrameTime() * 5.0f);
                ent2->increasePosition(ballVelocs[j] * getFrameTime() * 5.0f);
            }
        }
    }
}


//Called every frame to render objects before the buffers are swapped
void display(void) {
    float aspectRatio = (windowWidth / windowHeight) / 2.0f;

    //Iterate through each of the maps key/value pairs
    for (auto mapEntry = entities.begin(); mapEntry != entities.end(); mapEntry++) {
        const unsigned nEntities = mapEntry->second.size();
        //Iterate through the entities in this entity list      
        for (unsigned int i = 0; i < nEntities; i++) {
            //Get the entity
            Entity* entity = mapEntry->second[i];

            entity->increaseRotation(vec3(1.0f, 0.5f, 0.7f));
            entity->increasePosition(ballVelocs[i] * getFrameTime() * 5.0f);

            float zWidthAtBall = aspectRatio * tan(projFOV / 2.0f) * abs(2.0f * entity->getPosZ());
            float zHeightAtBall = aspectRatio * tan(projFOV / 2.0f) * abs(2.0f * entity->getPosZ());
            float minZ = -10.0f;
            float maxZ = -30.0f;

            if(entity->getPosX() > zWidthAtBall / 2.0f)
                ballVelocs[i].x = -abs(ballVelocs[i].x);
            else if(entity->getPosX() < -zWidthAtBall / 2.0f)
                ballVelocs[i].x = abs(ballVelocs[i].x);

            if(entity->getPosY() > zHeightAtBall / 2.0f)
                ballVelocs[i].y = -abs(ballVelocs[i].y);
            else if(entity->getPosY() < -zHeightAtBall / 2.0f)
                ballVelocs[i].y = abs(ballVelocs[i].y);

            if(entity->getPosZ() > minZ)
                ballVelocs[i].z = -abs(ballVelocs[i].z);
            else if(entity->getPosZ() < maxZ)
                ballVelocs[i].z = abs(ballVelocs[i].z);

        }
    }

	
    staticShader.start();
    staticShader.setProjectionMatrix(Maths::createProjectionMatrix(windowWidth, windowHeight, projFOV, projZNear, projZFar));
    staticShader.setViewMatrix(Maths::createViewMatrix(0.0f, 0.0f, vec3(0.0f, -0.0f, 0.0f)));
    staticShader.setLightColor(vec3(1.0f, 1.0f, 1.0f));
    staticShader.setLightPosition(vec3(7.0f, 0.0f, 0.0f));
    
	renderer.render(entities);
}

//Called when the application ends to deallocate memory
void cleanUp(void) {
	//Destroys the loading object
	loader->cleanUp();
	delete loader;

	//Cleans up game resources
	cleanUpModels();
	cleanUpShaders();
	cleanUpTextures();
}
