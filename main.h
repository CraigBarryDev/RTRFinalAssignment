#ifndef MAIN_H
#define MAIN_H

#include "cylinder.h"
#include "sphere.h"
#include "inputHandler.h"

#include "framework/shaders/StaticShader.h"
#include "framework/EntityRenderer.h"

#include "framework.h"

void cleanUpModels();
void cleanUpShaders();
void cleanUpTextures();

void initModels();
void initShaders();
void initTextures();

static Loader* loader;

extern StaticShader staticShader;

extern ModelTexture* woodTexture;

#endif