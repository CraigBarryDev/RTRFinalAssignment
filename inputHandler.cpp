#include "main.h"

void handleMouse() {}

void handleKeyboard() {
    if(glKeys->isPressed('a')){
    	//Rotate the cannon to the left
    	cannon.getEntity()->increaseRotation(0.0f, 0.0f, -CANNON_SENSITIVITY * getFrameTime());

    	//Stop cannon rotating too far to the left
    	if(cannon.getEntity()->getRotZ() < MIN_CANNON_ROTATION)
    		cannon.getEntity()->setRotZ(MIN_CANNON_ROTATION);
    }
    if(glKeys->isPressed('d')){
    	//Rotate the cannon to the right
    	cannon.getEntity()->increaseRotation(0.0f, 0.0f, CANNON_SENSITIVITY * getFrameTime());

    	//Stop cannon rotating too far to the right
    	if(cannon.getEntity()->getRotZ() > MAX_CANNON_ROTATION)
    		cannon.getEntity()->setRotZ(MAX_CANNON_ROTATION);
    }
    if(glKeys->keyUpEvent(' ')) {
    	float xSpeed = -sinf(radians(-cannon.getEntity()->getRotation().z));
    	float ySpeed = -cosf(radians(-cannon.getEntity()->getRotation().z));
    	
    	addCannonBall(cannon.getEntity()->getPosition(), vec3(xSpeed, ySpeed, 0.0f));
    }
    if(glKeys->keyUpEvent('s')) {
        addPeg(vec3(Maths::randBetweenf(-18.0f, 18.0f), Maths::randBetweenf(-15.0f, 5.0f), GAME_Z),
            vec3(0.0f, 0.0f, Maths::randBetweenf(-30.0f, 30.0f)));
    }
}