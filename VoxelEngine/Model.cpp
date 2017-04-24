#include "Model.h"

Model::Model(string modelLocation) {
	modelData = new ModelData;
}

Model::~Model() {
	modelData->vboData.clear();
	modelData->vboIndices.clear();
	delete modelData;
}

Model* Model::addVertex(GLfloat x, GLfloat y, GLfloat z, GLfloat nX, GLfloat nY, GLfloat nZ, GLfloat r, GLfloat g, GLfloat b) {
	vertices.push_back({ x, y, z, nX, nY, nZ, r, g, b });
	return this;
}

Model* Model::editVertex(unsigned int index, GLfloat x, GLfloat y, GLfloat z) {
	vertices[index].x = x;
	vertices[index].y = y;
	vertices[index].z = z;
	return this;
}

Model* Model::editVertex_N(unsigned int index, GLfloat x, GLfloat y, GLfloat z) {
	vertices[index].nX = x;
	vertices[index].nY = y;
	vertices[index].nZ = z;
	return this;
}

Model* Model::editVertex_C(unsigned int index, GLfloat r, GLfloat g, GLfloat b) {
	vertices[index].r = r;
	vertices[index].g = g;
	vertices[index].b = b;
	return this;
}

Model* Model::addFace(GLuint v1, GLuint v2, GLuint v3) {
	faces.push_back({ v1, v2, v3 });
	return this;
}

Model::ModelData* Model::getModelData() {
	modelData->vboData.clear();
	for (size_t i = 0; i < vertices.size(); i++) {
		//Vertex Data
		modelData->vboData.push_back(vertices[i].x);
		modelData->vboData.push_back(vertices[i].y);
		modelData->vboData.push_back(vertices[i].z);

		//Vertex Normal Data
		modelData->vboData.push_back(vertices[i].nX);
		modelData->vboData.push_back(vertices[i].nY);
		modelData->vboData.push_back(vertices[i].nZ);

		//Vertex Color Data
		modelData->vboData.push_back(vertices[i].r);
		modelData->vboData.push_back(vertices[i].g);
		modelData->vboData.push_back(vertices[i].b);
	}

	modelData->vboIndices.clear();
	for (size_t i = 0; i < faces.size(); i++) {
		modelData->vboIndices.push_back(faces[i].v1);
		modelData->vboIndices.push_back(faces[i].v2);
		modelData->vboIndices.push_back(faces[i].v3);
	}

	return modelData;
}

void Model::readModelData(string modelLocation) {

}