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

#define N_BALLS 45
vec3 ballVelocs[N_BALLS];
bool statics[N_BALLS];
const float ballSpeed = 1.0f;

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
        if(Maths::randBetween(0,2) == 1) {
            ballVelocs[i] = ballSpeed * normalize(vec3(Maths::randBetweenf(-1.0f, 1.0f), 
            Maths::randBetweenf(-1.0f, 1.0f), 
            Maths::randBetweenf(-1.0f, 1.0f)));
        }else {
            ballVelocs[i] = vec3(0.0f);
            // statics[i] = true;
        }
        
        ent->setPosition(vec3(Maths::randBetweenf(-5.0f, 5.0f), 
            Maths::randBetweenf(-5.0f, 5.0f), 
            Maths::randBetweenf(-5.0f, 5.0f) -17.0f));
    }

    std::pair<GLuint, vector<Entity*>> newMapPair(vaoID, entitiesOfSameType);
    entities.insert(newMapPair);
    renderer = EntityRenderer(&staticShader, projMatrix);
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

vector<vec3> spherePolyColliding(vec3 spherePos, float radius, vec3 polyPos, vector<vec3> polySurfaces, vector<vec3> polySurfaceNormals) {
    for(int i = 0; i < polySurfaces.size() - 1; i++) {
        vec3 surfacePos = polyPos + polySurfaces[i];
    }
}


//Update function called before each draw call to update program state
void update(void) {
    handleMouse();
    handleKeyboard();

    for(int i = 0; i < N_BALLS; i++) {
        for(int j = i + 1; j < N_BALLS; j++) {
            Entity* ent1 = entitiesOfSameType[i];
            Entity* ent2 = entitiesOfSameType[j];
            float collisionDepth = 0.0f;

            //If the balls are colliding
            if(ballsColliding(ent1->getPosition(), ent2->getPosition(), &collisionDepth)) {
                vec3 pos1 = ent1->getPosition();
                vec3 pos2 = ent2->getPosition();

                if(statics[i]) {
                    collisionReactionStatic(&pos2, &ballVelocs[j], pos1, collisionDepth);
                }else if(statics[j]) {
                    collisionReactionStatic(&pos1, &ballVelocs[i], pos2, collisionDepth);
                }else {
                    collisionReaction(&pos1, &ballVelocs[i], &pos2, &ballVelocs[j], collisionDepth);
                }
                
                
                ent1->setPosition(pos1);
                ent2->setPosition(pos2);
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
