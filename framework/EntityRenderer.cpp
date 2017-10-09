#include "EntityRenderer.h"

EntityRenderer::EntityRenderer(StaticShader* shader, mat4 projectionMatrix)
{
	this->staticShader = shader;
	shader->start();
	shader->setProjectionMatrix(projectionMatrix);
	shader->stop();
}

void EntityRenderer::render(unordered_map<GLuint, vector<Entity*>> entitiesMap) {
	//Iterate through each of the maps key/value pairs
	for (auto mapEntry = entitiesMap.begin(); mapEntry != entitiesMap.end(); mapEntry++) {
		//Get the textured model from the entity
		TexturedModel* texModel = mapEntry->second[0]->getModel();
		prepareTexturedModel(texModel);

		const unsigned nEntities = mapEntry->second.size();
		//Iterate through the entities in this entity list		
		for (unsigned int i = 0; i < nEntities; i++) {
			//Get the entity
			Entity* entity = mapEntry->second[i];

			//Prepare each entity for drawing (binding transfromations etc)
			prepareInstance(entity);

			//Draw the model
			glDrawElements(GL_TRIANGLES, texModel->getRawModel()->getVertexCount(), GL_UNSIGNED_INT, 0);
		}

		//Unbind the textured model
		unbindTexturedModel();
	}
}

void EntityRenderer::prepareTexturedModel(TexturedModel* model) {
	ModelTexture* modelTex = model->getTexture();
	RawModel* rModel = model->getRawModel();
	
	//Bind the VAO to the context
	glBindVertexArray(rModel->getVAOID());

	//Enable the attribute list index 0 on the VAO (our positional data)
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//Set the shine variables
	staticShader->setShineVariables(modelTex->getShineDamper(), modelTex->getReflectivity());
	staticShader->setUseFakeLighting(modelTex->getUseFakeLighting());

	//Set the texture atlas size
	staticShader->setTextureAtlasSize(vec2(modelTex->getTextureAtlasSizeX(), modelTex->getTextureAtlasSizeY()));

	//Bind the texture to the texture unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, modelTex->getTextureID());

	if (model->getTexture()->getHasTransparency())
		disableCulling();
	else
		enableCulling();

}

void EntityRenderer::prepareInstance(Entity* entity) {
	//Creates the transformation matrix from entities position
	mat4 transMatrix = entity->getTransformationMatrix();
	//Sets the transformation matrix in the shader
	staticShader->setTransformationMatrix(transMatrix);

	staticShader->setTextureAtlasOffsets(entity->getTextureOffset());
}

void EntityRenderer::unbindTexturedModel() {
	//Disable the attribute lists from the VAO
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	
	//Unbind the VAO from the context
	glBindVertexArray(0);
}

void EntityRenderer::enableCulling() {
	//Stops back faces from being rendered
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void EntityRenderer::disableCulling() {
	//Disabled back faces from not being rendered
	glDisable(GL_CULL_FACE);
}
