#include "main.h"

#include <vector>
#include <unordered_map>
#include "framework/loader.h"
#include "framework/GLHeaders.h"

unordered_map<GLuint, vector<Entity*>*> entities;
std::vector<Entity*> ballEntities;
std::vector<Entity*> cannonEntities;
EntityRenderer renderer;
Entity* ent1;
vector<vec3> ballVelocs;

class NSidedPolygon {
public:
    NSidedPolygon(unsigned int nSides, float depth, float radius) {
        float z = depth / 2.0f;
        int startingIndex = 0;

        for(int j = 0; j < 2; j++) {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(z);
            normals.push_back(0.0f);
            normals.push_back(0.0f);
            normals.push_back(startingIndex == 0 ? 1.0f : -1.0f);
            texCoords.push_back(0.0f);
            texCoords.push_back(0.0f);


            for(int i = 0; i < nSides; i++) {
                float theta = radians((float)i / (float)nSides * 360.0f);
                float x = radius * cosf(theta);
                float y = radius * sinf(theta);
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                printf("Vertex <%f, %f, %f>\n", x, y, z);
                normals.push_back(0.0f);
                normals.push_back(0.0f);
                normals.push_back(startingIndex == 0 ? 1.0f : -1.0f);
                texCoords.push_back(0.0f);
                texCoords.push_back(0.0f);
            }

            for(int i = 1; i < nSides; i++) {
                indices.push_back(startingIndex);
                indices.push_back(startingIndex + i);
                indices.push_back(startingIndex + i + 1);
                printf("Index <%d, %d, %d>\n", startingIndex + 0, startingIndex + i, startingIndex + i + 1);
            }

            indices.push_back(startingIndex);
            indices.push_back(startingIndex + nSides);
            indices.push_back(startingIndex + 1);
            printf("Index <%d, %d, %d>\n", startingIndex + 0, startingIndex + nSides, startingIndex + 1);

            startingIndex += nSides + 1;
            z -= depth;
        }

        z = depth / 2.0f;

        for(int j = 0; j < 2; j++) {
            for(int i = 0; i < nSides; i++) {
                float theta = radians((float)i / (float)nSides * 360.0f);
                float x = radius * cosf(theta);
                float y = radius * sinf(theta);
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                printf("Vertex <%f, %f, %f>\n", x, y, z);
                normals.push_back(0.0f);
                normals.push_back(0.0f);
                normals.push_back(startingIndex == 0 ? 1.0f : -1.0f);
                texCoords.push_back(0.0f);
                texCoords.push_back(0.0f);
            }

            z -= depth;
        }

        startingIndex -= 2;
        for(int i = 1; i < nSides; i++) {
            indices.push_back(startingIndex + i);
            indices.push_back(startingIndex + i + nSides + 1);
            indices.push_back(startingIndex + i + 1);
            indices.push_back(startingIndex + i + nSides + 1);
            indices.push_back(startingIndex + i + nSides + 2);
            indices.push_back(startingIndex + i + 1);
            normals.push_back(0.0f);
            normals.push_back(1.0f);
            normals.push_back(0.0f);
            texCoords.push_back(0.0f);
            texCoords.push_back(0.0f);
        }

        indices.push_back(startingIndex + nSides);
        indices.push_back(startingIndex + nSides + nSides + 1);
        indices.push_back(startingIndex + 1);
        indices.push_back(startingIndex + nSides + nSides + 1);
        indices.push_back(startingIndex + nSides + 2);
        indices.push_back(startingIndex + 1);
        normals.push_back(0.0f);
        normals.push_back(1.0f);
        normals.push_back(0.0f);
        texCoords.push_back(0.0f);
        texCoords.push_back(0.0f);
    
        // for(int i = 1; i < nSides; i++) {
        //     indices.push_back(i);
        //     indices.push_back(i + nSides + 1);
        //     indices.push_back(i + 1);
        //     indices.push_back(i + nSides + 1);
        //     indices.push_back(i + nSides + 2);
        //     indices.push_back(i + 1);
        // }

        // indices.push_back(nSides);
        // indices.push_back(nSides + nSides + 1);
        // indices.push_back(1);
        // indices.push_back(nSides + nSides + 1);
        // indices.push_back(nSides + 2);
        // indices.push_back(1);
    }
public:
    vector<GLfloat> vertices;
    vector<GLfloat> texCoords;
    vector<GLfloat> normals;
    vector<GLuint> indices;
};

class NSidedFlatPolygon {
public:
    NSidedFlatPolygon(unsigned int nSides, float depth, float radius) {
        float z = 0;

        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(z);
        normals.push_back(0.0f);
        normals.push_back(0.0f);
        normals.push_back(1.0f);
        texCoords.push_back(0.0f);
        texCoords.push_back(0.0f);


        for(int i = 0; i < nSides; i++) {
            float theta = radians((float)i / (float)nSides * 360.0f);
            float x = radius * cosf(theta);
            float y = radius * sinf(theta);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            collisionPositions.push_back(vec3(x,y,z));

            printf("Vertex <%f, %f, %f>\n", x, y, z);
            normals.push_back(0.0f);
            normals.push_back(0.0f);
            normals.push_back(1.0f);
            texCoords.push_back(0.0f);
            texCoords.push_back(0.0f);
        }

        for(int i = 0; i < collisionPositions.size(); i++) {
            vec3 v1, v2, zVec;
            zVec = vec3(0.0f, 0.0f, 1.0f);
            v1 = collisionPositions[i];
            if(i != collisionPositions.size() -1)
                v2 = collisionPositions[i + 1];
            else
                v2 = collisionPositions[0];

            vec3 normal = normalize(cross(v2 - v1, zVec));
            printf("Normal: <%f, %f, %f>\n", normal.x, normal.y, normal.z);
            collisionNormals.push_back(normal);
        }

        for(int i = 1; i < nSides; i++) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        indices.push_back(0);
        indices.push_back(nSides);
        indices.push_back(1);
    }
public:
    vector<GLfloat> vertices;
    vector<GLfloat> texCoords;
    vector<GLfloat> normals;
    vector<GLuint> indices;

    vector<vec3> collisionNormals;
    vector<vec3> collisionPositions;
};

//Main initialization
void init(void) {
	//Creates the loading object
	loader = new Loader();

    NSidedFlatPolygon n = NSidedFlatPolygon(3, 1.0f, 1.0f);

	//Initialize game resources
    initTextures();
    initShaders();
    initModels();
    
    ballTexture->setShineDamper(10.0f);
    ballTexture->setReflectivity(0.8f);
    woodTexture->setShineDamper(25.0f);
    woodTexture->setReflectivity(0.2f);

    // ballTexModel = new TexturedModel(model, ballTexture);
    int vaoID = ballTexModel->getRawModel()->getVAOID();

    cannon.getEntity()->setPosition(vec3(0.0f, 14.0f, GAME_Z));
    cannonEntities.push_back(cannon.getEntity());
    std::pair<GLuint, vector<Entity*>*> cannonPair(cannon.getEntity()->getModel()->getRawModel()->getVAOID(), &cannonEntities);
    entities.insert(cannonPair);

    std::pair<GLuint, vector<Entity*>*> newMapPair(vaoID, &ballEntities);
    entities.insert(newMapPair);
    renderer = EntityRenderer(&staticShader, projMatrix);
}

void addCannonBall(vec3 pos, vec3 vel) {
    Entity* ent = new Entity(ballTexModel);
    ent->setPosition(pos);
    ballVelocs.push_back(normalize(vel) * BALL_SPEED);
    ballEntities.push_back(ent);
}

void collisionReaction(vec3* pos1, vec3* vel1, vec3* pos2, vec3* vel2, float collisionDepth) {
    vec3 collisionNormal = normalize(*pos2 - *pos1);
    float initVel1 = dot(*vel1, collisionNormal);
    float initVel2 = dot(*vel2, collisionNormal);

    float mass1 = 1.0f;
    float mass2 = 1.0f;
    float finalVel1 = (mass1 - mass2) / (mass1 + mass2) * initVel1 + 2.0f * mass2 / (mass1 + mass2) * initVel2;
    float finalVel2 = 2.0f * mass1 / (mass1 + mass2) * initVel1 + (mass2 - mass1) / (mass1 + mass2) * initVel2;

    *pos1 += (collisionDepth / 2.0f) * collisionNormal;
    *pos2 -= (collisionDepth / 2.0f) * collisionNormal;

    *vel1 = (*vel1 - vec3(initVel1) * collisionNormal) + vec3(finalVel1) * collisionNormal;
    *vel2 = (*vel2 - vec3(initVel2) * collisionNormal) + vec3(finalVel2) * collisionNormal;
}

void collisionReactionStatic(vec3* pos, vec3* vel, vec3 staticPos, float collisionDepth) {
    vec3 collisionNormal = normalize(staticPos - *pos);
    *pos += collisionDepth * collisionNormal;
    *vel = reflect(*vel, collisionNormal);
}

//Determines if two balls are colliding or not
bool ballsColliding(vec3 pos1, vec3 pos2, float* collisionDepth) {
    *collisionDepth = Maths::distBetween(pos1, pos2) - 2.0f;
    return *collisionDepth < 0.0f;
}

//Update function called before each draw call to update program state
void update(void) {
    handleMouse();
    handleKeyboard();

    float aspectRatio = (windowWidth / windowHeight) / 2.0f;

    for(int i = 0; i < ballEntities.size(); i++) {
        //Add gravitity to each ball's velocity
        ballVelocs[i] += getFrameTime() * vec3(0.0f, -GRAVITY_CONST, 0.0f);

        //Get the entity
        Entity* entity = ballEntities[i];

        // entity->increaseRotation(vec3(1.0f, 0.5f, 0.7f));
        entity->increaseRotation(ballVelocs[i]);
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

        //Check for collisions
        for(int j = i + 1; j < ballEntities.size(); j++) {
            Entity* ent1 = ballEntities[i];
            Entity* ent2 = ballEntities[j];
            float collisionDepth = 0.0f;

            //If the balls are colliding
            if(ballsColliding(ent1->getPosition(), ent2->getPosition(), &collisionDepth)) {
                vec3 pos1 = ent1->getPosition();
                vec3 pos2 = ent2->getPosition();

                collisionReaction(&pos1, &ballVelocs[i], &pos2, &ballVelocs[j], collisionDepth);         
                
                ent1->setPosition(pos1);
                ent2->setPosition(pos2);
            }
        }
    }
}


//Called every frame to render objects before the buffers are swapped
void display(void) {
    backShader.start();
    backShader.setShineVariables(woodTexture->getShineDamper(), woodTexture->getReflectivity());
    backShader.setLightColor(LIGHT_COLOR);
    backShader.setLightPosition(LIGHT_POS);
    backShader.setAmbientLight(3.8f * LIGHT_AMBIENT);
    backdrop.render();

    staticShader.start();
    staticShader.setProjectionMatrix(Maths::createProjectionMatrix(windowWidth, windowHeight, projFOV, projZNear, projZFar));
    staticShader.setViewMatrix(Maths::createViewMatrix(0.0f, 0.0f, vec3(0.0f, -0.0f, 0.0f)));
    staticShader.setLightColor(LIGHT_COLOR);
    staticShader.setLightPosition(LIGHT_POS);
    staticShader.setAmbientLight(0.7f * LIGHT_AMBIENT);
    
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
