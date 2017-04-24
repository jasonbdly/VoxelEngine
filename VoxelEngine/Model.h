#pragma once

#include <string>
#include <vector>
#include <GLFW\glfw3.h>

using namespace std;

class Model {
public:
	Model(string modelLocation);
	~Model();

	struct ModelData {
		vector<GLfloat> vboData;
		vector<GLuint> vboIndices;
	};

	Model* addVertex(GLfloat x, GLfloat y, GLfloat z, GLfloat nX, GLfloat nY, GLfloat nZ, GLfloat r, GLfloat g, GLfloat b);
	Model* editVertex(unsigned int index, GLfloat x, GLfloat y, GLfloat z);
	Model* editVertex_N(unsigned int index, GLfloat x, GLfloat y, GLfloat z);
	Model* editVertex_C(unsigned int index, GLfloat r, GLfloat g, GLfloat b);

	Model* addFace(GLuint v1, GLuint v2, GLuint v3);

	ModelData* getModelData();

private:
	struct Vertex {
		GLfloat x, y, z;
		GLfloat nX, nY, nZ;
		GLfloat r, g, b;
	};

	struct Face {
		GLuint v1, v2, v3;
	};

	vector<Vertex> vertices;
	vector<Face> faces;

	ModelData* modelData;

	void readModelData(string modelLocation);
};

