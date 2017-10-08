#include "loader.h"

Loader::Loader() {}

Loader::~Loader() {}

RawModel* Loader::loadToVAO(vector<GLfloat> positions, vector<GLfloat> normals, vector<GLuint> indices) {
	//Creates the VAO object and stores its ID in vaoID
	GLuint vaoID = createVAO();
	//Store the positional data in the first index of the attribute list of the VAO
	storeDataInAttributeList(0, 3, positions);
	storeDataInAttributeList(1, 3, normals);
	//Bind indices buffer to VAO
	bindIndicesBuffer(indices);
	//Unbind the VAO from the context
	unbindVAO();
	//Return the model
	return new RawModel(vaoID, positions.size(), 1.0f);
}

RawModel*  Loader::loadToVAO(vector<GLfloat> positions, vector<GLfloat> normals) {
	//Creates the VAO object and stores its ID in vaoID
	GLuint vaoID = createVAO();
	//Store the positional data in the first index of the attribute list of the VAO
	storeDataInAttributeList(0, 3, positions);
	storeDataInAttributeList(1, 3, normals);
	//Unbind the VAO from the context
	unbindVAO();
	//Return the model
	return new RawModel(vaoID, positions.size(), 1.0f);
}

RawModel*  Loader::loadToVAO(GLuint vao, vector<GLfloat> positions, vector<GLfloat> normals) {
	//Bind the VAO
	glBindVertexArray(vao);
	//Store the vertex data in corresponding attribute lists
	storeDataInAttributeList(0, 3, positions);
	storeDataInAttributeList(1, 3, normals);
	//Unbind the vao
	unbindVAO();
	//Return the model
	return new RawModel(vao, positions.size(), 1.0f);
}

GLuint Loader::createVAO() {
	//Creates the vao and stores its ID in the vaoID variable
	GLuint vaoID;
	//Generates the VAO
	glGenVertexArrays(1, &vaoID);
	//Adds the VAO to the list of vaos
	vaos.push_back(vaoID);
	//Binds the VAO to the context
	glBindVertexArray(vaoID);
	//Returns the vaoID
	return vaoID;
}

void Loader::storeDataInAttributeList(GLuint attributeNumber, GLuint coordinateSize,  std::vector<GLfloat> data) {
	//Creates the VBO and stores its ID in the vboID variable
	GLuint vboID;
	glGenBuffers(1, &vboID);

	//Adds the VBO to the list of VBOs
	vbos.push_back(vboID);

	//Binds the vbo to GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, vboID);

	//Stores the data into the VBO
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

	//Adds the VBO to the VAO
	glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	//Enable the vertex attribute to be used (when passed to a shader)
	glEnableVertexAttribArray(attributeNumber);

	//Unbinds the VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::bindIndicesBuffer(std::vector<GLuint> indices) {
	//Creates the ebo (element array buffer/index buffer)
	GLuint eboID;
	glGenBuffers(1, &eboID);
	//Add it to the list of vbos
	vbos.push_back(eboID);
	//Bind the array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	//Bind the element data to the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void Loader::unbindVAO() {
	//Unbinds the VAO from the context
	glBindVertexArray(0);
}

void Loader::cleanUp() {
	//Iterate through the VAOs and delete them
	for (unsigned int i = 0; i < vaos.size(); i++) {
		glDeleteVertexArrays(1, &vaos[i]);
	}
	//Iterate through the VBOs and delete them
	for (unsigned int i = 0; i < vbos.size(); i++) {
		glDeleteBuffers(1, &vbos[i]);
	}
}