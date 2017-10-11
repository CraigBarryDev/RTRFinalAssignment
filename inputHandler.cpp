#include "main.h"

void handleMouse() {

}

void handleKeyboard() {
    if(glKeys->isPressed('a'))
    	cannon.getEntity()->increaseRotation(0.0f, 0.0f, -CANNON_SENSITIVITY * getFrameTime());
    if(glKeys->isPressed('d'))
    	cannon.getEntity()->increaseRotation(0.0f, 0.0f, CANNON_SENSITIVITY * getFrameTime());
    if(glKeys->keyUpEvent(' ')) {
    	float xSpeed = sinf(radians(cannon.getEntity()->getRotation().z));
    	float ySpeed = cosf(radians(cannon.getEntity()->getRotation().z));
    	
    	addCannonBall(cannon.getEntity()->getPosition(), vec3(xSpeed, ySpeed, 0.0f));
    	printf("Space pressed\n");
    }
}