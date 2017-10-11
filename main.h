#ifndef MAIN_H
#define MAIN_H

#include "backdrop.h"
#include "cannon.h"
#include "cylinder.h"
#include "sphere.h"

#include "framework/shaders/StaticShader.h"
#include "framework/EntityRenderer.h"

#include "framework.h"

#define CANNON_SENSITIVITY 50.0f

#define LIGHT_COLOR vec3(1.0f, 1.0f, 1.0f)
#define LIGHT_POS vec3(7.0f, 0.0f, 0.0f)
#define LIGHT_AMBIENT vec3(0.4f)

void addCannonBall(vec3 pos, vec3 vel);

void handleMouse();
void handleKeyboard();

void cleanUpModels();
void cleanUpShaders();
void cleanUpTextures();

void initModels();
void initShaders();
void initTextures();

static Loader* loader;

//Shaders
extern StaticShader staticShader;
extern BackdropShader backShader;

//Textures
extern ModelTexture* ballTexture;
extern ModelTexture* woodTexture;
extern ModelTexture* cannonTexture;

//Models
extern Backdrop backdrop;
extern Cannon cannon;


#endif