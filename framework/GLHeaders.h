#ifndef GLHEADERS_H
#define GLHEADERS_H

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string>

#define GL_GLEXT_PROTOTYPES
#define GLM_FORCE_RADIANS

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
// #include <GL/glut.h>
//Using freeglut so that a core profile can be set
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include "SOIL.h"

using namespace std;
using namespace glm;

#endif