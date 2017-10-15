#include "main.h"

#include <vector>
#include <unordered_map>
#include "framework/loader.h"
#include "framework/GLHeaders.h"

vector<vec3> ballVelocs;
vector<Peg> pegs;
vector<Particles> particleSystems;

#define N_PEGS 100

void setupLevel() {
    //Print the controls for the user
    printControls();
    //Iterate through the number of pegs in a level
    for(int i = 0; i < N_PEGS; i++) {
        //Add random pegs to the level
        addPeg(vec3(Maths::randBetweenf(-18.0f, 18.0f), Maths::randBetweenf(-15.0f, 5.0f), GAME_Z),
            vec3(0.0f, 0.0f, Maths::randBetweenf(-30.0f, 30.0f)));
    }
}

void clearPegs() {
    pegs.clear();
    peg3Entities.clear();
    peg4Entities.clear();
    peg5Entities.clear();
    peg6Entities.clear();
}

void clearBalls() {
    ballVelocs.clear();
    ballEntities.clear();
}

//Main initialization
void init(void) {
	//Creates the loading object
	loader = new Loader();

	//Initialize game resources
    initTextures();
    initShaders();
    initModels();
    initRenderer();

    //Setup the level
    setupLevel();
}

void addPeg(vec3 pos, vec3 angularVeloc) {
    int nSides = Maths::randBetween(3,7);
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

    Peg peg = Peg(ent);
    peg.setAngularVeloc(angularVeloc);
    pegs.push_back(peg);
}

void addCannonBall(vec3 pos, vec3 vel) {
    Entity* ent = new Entity(ballTexModel);
    ent->setPosition(pos);
    ballVelocs.push_back(normalize(vel) * BALL_SPEED);
    ballEntities.push_back(ent);
}

void startExplosion(vec3 pos) {
    Particles p = Particles(loader, 100, particleTexture, particleShader, pos);
    particleSystems.push_back(p);
}

void handlePegCollisions(std::vector<Entity*>* pegs, Entity* ball, int ballIndex, NSidedPolygon& polyModel) {
    for(int j = 0; j < pegs->size(); j++) {
        Entity* peg = pegs[0][j];

        //If the peg is not destroyed
        if(!peg->getDestroyed()) {
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

                peg->setDestroyed();
            }
        }
    }
}

void disposeDestroyedPegs(std::vector<Entity*>* pegs) {
    for(int j = 0; j < pegs->size(); j++) {
        Entity* peg = pegs[0][j];

        //If the peg is read to be disposed, dispose it
        if(peg->shouldBeDisposed()) {
            startExplosion(peg->getPosition());
            //Remove the peg as it has now been destroyed
            pegs->erase(pegs->begin() + j);
            //Decrement loop otherwise we will skip a peg
            j--;
        }
    }
}

void disposeParticleSystems() {
    for(int i = 0; i < particleSystems.size(); i++) {
        //If the particle system is finished
        if(particleSystems[i].finished()) {
            //Cleanup particle system resources
            particleSystems[i].cleanUp();
            //Destroy the particle system
            particleSystems.erase(particleSystems.begin() + i);
            //Decrement loop to not skip a particle system
            i--;
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
            if(spheresColliding(ball1->getPosition(), ball2->getPosition(), &collisionDepth)) {
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

    //Dispose any items that need to be disposed
    disposeDestroyedPegs(&peg3Entities);
    disposeDestroyedPegs(&peg4Entities);
    disposeDestroyedPegs(&peg5Entities);
    disposeDestroyedPegs(&peg6Entities);
    disposeParticleSystems();

    //Iterate through each peg and update its position
    for(int i = 0; i < pegs.size(); i++) {
        pegs[i].update();
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

    //Calculate the view/projection matrices
    mat4 projMatrix = Maths::createProjectionMatrix(windowWidth, windowHeight, projFOV, projZNear, projZFar);
    mat4 viewMatrix = Maths::createViewMatrix(0.0f, 0.0f, vec3(0.0f, -0.0f, 0.0f));

    particleShader.start();
    particleShader.setProjectionMatrix(projMatrix);
    particleShader.setViewMatrix(viewMatrix);
    //Iterate through each of the particle systems
    for(int i = 0; i < particleSystems.size(); i++) {
        //Render the particle system
        particleSystems[i].render();
    }

    staticShader.start();
    staticShader.setProjectionMatrix(projMatrix);
    staticShader.setViewMatrix(viewMatrix);
    staticShader.setLightColor(LIGHT_COLOR);
    staticShader.setLightPosition(LIGHT_POS);
    staticShader.setAmbientLight(0.7f * LIGHT_AMBIENT);
    staticShader.setTime(getTimeSec());
    //Render all entities
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
