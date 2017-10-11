#include "main.h"

void handleMouse() {

}

void handleKeyboard() {
    if(glKeys->isPressed('a'))
    	cannon.getEntity()->increaseRotation(0.0f, 0.0f, -CANNON_SENSITIVITY * getFrameTime());
    if(glKeys->isPressed('d'))
    	cannon.getEntity()->increaseRotation(0.0f, 0.0f, CANNON_SENSITIVITY * getFrameTime());
}