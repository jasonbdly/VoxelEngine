#pragma once

#include "includes.h"

class Chunk {
	struct voxel {
		int x, y, z;
	};

public:
	Chunk(int x, int y, int z);
	~Chunk();

	void update(GLfloat delta);
	void draw();

	void addVoxel(int x, int y, int z);

private:
	//Chunk Data
	vec3 pos;
	vector<voxel> voxels;

	//GL Data
	bool graphicsOutOfDate;
	GLuint vaoId, dataVBOId, indicesVBOId;

	void pushGraphicsData();
};

