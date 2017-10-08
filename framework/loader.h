#ifndef LOADER_H
#define LOADER_H

#include "GLHeaders.h"
#include <vector>

class Loader {
public:
	Loader();
	~Loader();
public:
	void cleanUp();
	GLuint loadToVAO(vector<GLfloat> positions, vector<GLfloat> normals, vector<GLuint> indices);
	GLuint loadToVAO(vector<GLfloat> positions, vector<GLfloat> normals);
	GLuint loadToVAO(GLuint vao, vector<GLfloat> positions, vector<GLfloat> normals);
private:
	GLuint createVAO();
	void storeDataInAttributeList(GLuint attributeNumber, GLuint coordinateSize, std::vector<GLfloat> data);
	void unbindVAO();
	void bindIndicesBuffer(std::vector<GLuint> indices);
private:
	std::vector<GLuint> vaos = std::vector<GLuint>();
	std::vector<GLuint> vbos = std::vector<GLuint>();
};

#endif