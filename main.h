#ifndef MAIN_H
#define MAIN_H

#include "cylinder.h"
#include "inputHandler.h"

#include "shaders/litShader.h"

void cleanUpShaders();

void initModels();
void initShaders();

static LitShader litShader;

#endif