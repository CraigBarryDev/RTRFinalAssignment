#include "main.h"

void printControls() {
    printf("******************************\n");
    printf(" PEGGLE - RTR ASSIGNMENT 3\n");
    printf("******************************\n");
    printf("-----------Controls-----------\n");
    printf("A/Left:    Move cannon left\n");
    printf("D/Right:   Move cannon right\n");
    printf("Space:     Fires ball\n");
    printf("S:         Adds a random peg\n");
    printf("R:         Resets the level\n");
    printf("------------------------------\n");
}

void handleMouse() {}

void handleKeyboard() {
    //Move the cannon left
    if(glKeys->isPressed('a') || glKeys->isSpecialKeyPressed(SPECIAL_KEY_LEFT)){
    	//Rotate the cannon to the left
    	cannon.getEntity()->increaseRotation(0.0f, 0.0f, -CANNON_SENSITIVITY * getFrameTime());

    	//Stop cannon rotating too far to the left
    	if(cannon.getEntity()->getRotZ() < MIN_CANNON_ROTATION)
    		cannon.getEntity()->setRotZ(MIN_CANNON_ROTATION);
    }
    //Move the cannon right
    if(glKeys->isPressed('d') || glKeys->isSpecialKeyPressed(SPECIAL_KEY_RIGHT)){
    	//Rotate the cannon to the right
    	cannon.getEntity()->increaseRotation(0.0f, 0.0f, CANNON_SENSITIVITY * getFrameTime());

    	//Stop cannon rotating too far to the right
    	if(cannon.getEntity()->getRotZ() > MAX_CANNON_ROTATION)
    		cannon.getEntity()->setRotZ(MAX_CANNON_ROTATION);
    }
    //Fire the cannon
    if(glKeys->keyUpEvent(' ')) {
    	float xSpeed = -sinf(radians(-cannon.getEntity()->getRotation().z));
    	float ySpeed = -cosf(radians(-cannon.getEntity()->getRotation().z));
    	
    	addCannonBall(cannon.getEntity()->getPosition(), vec3(xSpeed, ySpeed, 0.0f));
    }

    //Add more pegs
    if(glKeys->keyUpEvent('s')) {
        addPeg(vec3(Maths::randBetweenf(-18.0f, 18.0f), Maths::randBetweenf(-15.0f, 5.0f), GAME_Z),
            vec3(0.0f, 0.0f, Maths::randBetweenf(-30.0f, 30.0f)));
    }
    //Reset the level
    if(glKeys->keyUpEvent('r')) {
        cannon.getEntity()->setRotZ(0.0f);
        clearBalls();
        clearPegs();
        setupLevel();
    }
}