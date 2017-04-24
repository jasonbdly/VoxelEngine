#pragma once

#include "includes.h"

#include "utilities.h"
#include "keyhandler.h"
#include "MouseHandler.h"
#include "Camera.h"

using namespace std;
using namespace glm;

const int SCREEN_WIDTH = 1600, SCREEN_HEIGHT = 900;

class World {
	struct voxel {
		int x, y, z;
	};

public:
	World();
	~World();
	void init();
	void update();
	void render();

	void addVoxel(int x, int y, int z);

private:
	GLfloat voxelSize;
	vector<voxel> voxels;

	mat4 modelMatrix, viewMatrix, projectionMatrix;

	GLint shaderProgramId, vertexShaderId, fragmentShaderId;
	GLint shader_attrib_pos, shader_attrib_color, shader_attrib_norm;
	GLint shader_uniform_modelMatrix, shader_uniform_viewMatrix, shader_uniform_projectionMatrix,
		shader_uniform_lightPos, shader_uniform_lightColor;
	
	GLuint worldVAO, worldVerticesVBO, worldIndicesVBO;

	bool hasInit, graphicsOutOfDate;
	vector<GLfloat> vertices;
	vector<GLfloat> normals;
	vector<GLfloat> colors;
	vector<GLuint> indices;

	GLfloat deltaTime, lastFrameTime;

	Camera *camera;

	vec3 lightPos, lightColor;

	void updateGraphicsData();
};

