#include "main.h"

#include <vector>
#include <unordered_map>
#include "framework/loader.h"
#include "framework/GLHeaders.h"


std::vector<Entity*> ballEntities;
std::vector<Entity*> cannonEntities;
std::vector<Entity*> peg3Entities;
std::vector<Entity*> peg4Entities;
std::vector<Entity*> peg5Entities;
std::vector<Entity*> peg6Entities;
EntityRenderer renderer;
unordered_map<GLuint, vector<Entity*>*> entities;
vector<vec3> ballVelocs;

//Main initialization
void init(void) {
	//Creates the loading object
	loader = new Loader();

	//Initialize game resources
    initTextures();
    initShaders();
    initModels();

    //Initialize the entitiy renderer
    renderer = EntityRenderer(&staticShader, projMatrix);

    //Initialize
    cannon.getEntity()->setPosition(vec3(0.0f, 14.0f, GAME_Z));
    cannonEntities.push_back(cannon.getEntity());
    std::pair<GLuint, vector<Entity*>*> cannonPair(cannon.getEntity()->getModel()->getRawModel()->getVAOID(), &cannonEntities);
    entities.insert(cannonPair);

    int vaoID = ballTexModel->getRawModel()->getVAOID();
    std::pair<GLuint, vector<Entity*>*> newMapPair(vaoID, &ballEntities);
    entities.insert(newMapPair);
    
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

void addPeg(vec3 pos) {
    int nSides = Maths::randBetween(3,7);
    // srand(time(NULL));
    int texture = Maths::randBetween(0,3);
    ModelTexture* tex;
    RawModel* raw;

    switch(texture) {
        case 0:
            tex = blueTexture; break;
        case 1:
            tex = greenTexture; break;
        case 2:
            tex = orangeTexture; break;
    }
   
    switch(nSides) {
        case 3:
            raw = pegPoly3; break;
        case 4:
            raw = pegPoly4; break;
        case 5:
            raw = pegPoly5; break;
        case 6:
            raw = pegPoly6; break;
    }
    
    TexturedModel* polyTexModel = new TexturedModel(raw, tex);
    Entity* ent = new Entity(polyTexModel);
    ent->setPosition(pos);

    switch(nSides) {
        case 3:
            peg3Entities.push_back(ent); break;
        case 4:
            peg4Entities.push_back(ent); break;
        case 5:
            peg5Entities.push_back(ent); break;
        case 6:
            peg6Entities.push_back(ent); break;
    }
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

void sphereCollisionReactionStatic(vec2* pos, vec2* vel, vec2 staticPos, float collisionDepth) {
    vec2 collisionNormal = normalize(staticPos - *pos);
    *pos += collisionDepth * collisionNormal;
    *vel = reflect(*vel, collisionNormal);
}

void collisionReactionStatic(vec2* pos, vec2* vel, vec2 staticPos, vec2 collisionNormal) {
    *vel = reflect(*vel, collisionNormal);
}

//Determines if two balls are colliding or not
bool ballsColliding(vec3 pos1, vec3 pos2, float* collisionDepth) {
    *collisionDepth = Maths::distBetween(pos1, pos2) - 2.0f;
    return *collisionDepth < 0.0f;
}

bool circleCollidingVertex(vec2 cPos, float cRadius, vec2 v, vec2* collisionNormal) {
    //Get the vector from the vertex to the circle
    vec2 vecBetween = cPos - v;
    //Get the distance between the circle centre and the vertex
    float dist = length(vecBetween);

    //Determine if they are colliding
    if(dist < cRadius) {
        //Determine the collision normal
        *collisionNormal = normalize(vecBetween);
        //Return that a collision has occured
        return true;
    }

    //Return that no collision has occured
    return false;
}

bool circleCollidingLine(vec2 cPos, float cRadius, vec2 l1, vec2 l2, vec2 normal, vec2* collisionNormal) {
    //Gets the midpoint of the line segment
    vec2 midPoint = l1 + (0.5f * (l2 - l1));
    //Gets the vector from the midpoint of the line to the centre of the circle
    vec2 midToCircle = cPos - midPoint;
    //Gets the length of the line segment
    float lineSegLength = length(l2 - l1);
    //Projects the to circle vector across the surface normal
    vec2 normProjection = proj(midToCircle, normal);
    float distFromLine = length(normProjection);
    //Projects the to circle vector across the line segment
    vec2 vecProjection = proj(midToCircle, l2 - l1);
    float distFromMid = length(vecProjection);

    //If the circle centre is one the same plane as the line segment
    if(distFromMid < lineSegLength / 2.0f) {
        
        //If the distance from the line is less than the radius of the circle
        if(distFromLine < cRadius) { 
            //The collision normal will be the surface's normal
            *collisionNormal = normal;
            //A collision has occured
            return true;
        }

        //If it is not close enough, there is no collsion
        return false;
    }else {
        //The only way the circle can collide with the line outside the plane,
        //is if it collides with one of the vertices
        return circleCollidingVertex(cPos, cRadius, l1, collisionNormal) ||
                circleCollidingVertex(cPos, cRadius, l2, collisionNormal);
    }
}

//Determines if a circle is colliding with a polygon
bool circleCollidingPoly(vec2 cPos, float cRadius, vec2 posOffset, float rotZ, vector<vec2> vertices, vector<vec2> normals, vec2* collisionNormal) {
    //Iterate through each of the polygons vertices
    for(int i = 0; i < vertices.size() - 1; i++) {
        //Translate and Rotate the input vertices
        vec2 v1 = rotate(vertices[i], rotZ) + posOffset;
        vec2 v2 = rotate(vertices[i + 1], rotZ) + posOffset;
        //If the circle is colliding with this edge of the polygon
        if(circleCollidingLine(cPos, cRadius, v1, v2, normals[i], collisionNormal))
            //Then there is a collision occuring
            return true;
    }

    return false;
}

void handlePegCollisions(std::vector<Entity*>* pegs, Entity* ball, int ballIndex, NSidedPolygon& polyModel) {
    for(int j = 0; j < pegs->size(); j++) {
        Entity* peg = pegs[0][j];

        vec2 collisionNormal;
        vec2 pegPos = vec2(peg->getPosX(), peg->getPosY());
        float pegRot = peg->getRotZ();
        vec2 ballPos = vec2(ball->getPosX(), ball->getPosY());
        vec2 ballVel = vec2(ballVelocs[ballIndex].x, ballVelocs[ballIndex].y);

        //Is the ball colliding with the peg
        if(circleCollidingPoly(ballPos, BALL_SIZE, pegPos, pegRot,
            polyModel.vertices2D, polyModel.normals2D, &collisionNormal)) {

            //Perform the collision reaction
            collisionReactionStatic(&ballPos, &ballVel, pegPos, collisionNormal);
            ball->setPosX(ballPos.x);
            ball->setPosY(ballPos.y);
            ballVelocs[ballIndex].x = ballVel.x;
            ballVelocs[ballIndex].y = ballVel.y;

            //Remove the peg as it has now been destroyed
            pegs[0].erase(pegs->begin() + j);
            //Decrement loop otherwise we will skip a peg
            j--;
        }
    }
}

//Update function called before each draw call to update program state
void update(void) {
    handleMouse();
    handleKeyboard();

    //Calculate the window's aspect ration
    float aspectRatio = (windowWidth / windowHeight);

    //Iterate through each ball
    for(int i = 0; i < ballEntities.size(); i++) {
        //Add gravitity to each ball's velocity
        ballVelocs[i] += getFrameTime() * vec3(0.0f, -GRAVITY_CONST, 0.0f);

        //Get the entity
        Entity* ball = ballEntities[i];
        //Rotate the ball by its velocity (not physically accurate, but looks fine)
        ball->increaseRotation(ballVelocs[i]);
        //Move the ball by its velocity over the given timeframe
        ball->increasePosition(ballVelocs[i] * getFrameTime() * 5.0f);

        float zWidthAtBall = 0.5f * aspectRatio * tan(projFOV / 2.0f) * abs(2.0f * ball->getPosZ());
        //Height not entirely accurate, but close enough for its purpose
        float zHeightAtBall = 0.5f * aspectRatio * tan(projFOV / 2.0f) * abs(2.0f * ball->getPosZ());

        //Handle collisions with side walls
        if(ball->getPosX() + BALL_SIZE > zWidthAtBall)
            ballVelocs[i].x = -abs(ballVelocs[i].x);
        else if(ball->getPosX() - BALL_SIZE < -zWidthAtBall)
            ballVelocs[i].x = abs(ballVelocs[i].x);

        //If the ball falls off the bottom
        if(ball->getPosY() < -zHeightAtBall){
            ballVelocs[i].y = abs(ballVelocs[i].y);
            ballVelocs.erase(ballVelocs.begin() + i);
            ballEntities.erase(ballEntities.begin() + i);
        }

        //Check for collisions with other balls
        for(int j = i + 1; j < ballEntities.size(); j++) {
            Entity* ball1 = ballEntities[i];
            Entity* ball2 = ballEntities[j];
            float collisionDepth = 0.0f;

            //If the balls are colliding
            if(ballsColliding(ball1->getPosition(), ball2->getPosition(), &collisionDepth)) {
                vec3 pos1 = ball1->getPosition();
                vec3 pos2 = ball2->getPosition();

                collisionReaction(&pos1, &ballVelocs[i], &pos2, &ballVelocs[j], collisionDepth);         
                
                ball1->setPosition(pos1);
                ball2->setPosition(pos2);
            }
        }

        //Handle collisions with pegs
        handlePegCollisions(&peg3Entities, ball, i, pegPolyModel3);
        handlePegCollisions(&peg4Entities, ball, i, pegPolyModel4);
        handlePegCollisions(&peg5Entities, ball, i, pegPolyModel5);
        handlePegCollisions(&peg6Entities, ball, i, pegPolyModel6);
    }
}


//Called every frame to render objects before the buffers are swapped
void display(void) {
    backShader.start();
    backShader.setShineVariables(woodTexture->getShineDamper(), woodTexture->getReflectivity());
    backShader.setLightColor(LIGHT_COLOR);
    backShader.setLightPosition(LIGHT_POS);
    backShader.setAmbientLight(3.8f * LIGHT_AMBIENT);
    //Render the backdrop
    backdrop.render();

    staticShader.start();
    staticShader.setProjectionMatrix(Maths::createProjectionMatrix(windowWidth, windowHeight, projFOV, projZNear, projZFar));
    staticShader.setViewMatrix(Maths::createViewMatrix(0.0f, 0.0f, vec3(0.0f, -0.0f, 0.0f)));
    staticShader.setLightColor(LIGHT_COLOR);
    staticShader.setLightPosition(LIGHT_POS);
    staticShader.setAmbientLight(0.7f * LIGHT_AMBIENT);
    staticShader.setTime(getTimeSec());
    staticShader.setUseAnimatedTextures(true);
    
    //Render any entities
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
