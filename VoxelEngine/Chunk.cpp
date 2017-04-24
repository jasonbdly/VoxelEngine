#include "Chunk.h"



Chunk::Chunk(int x, int y, int z) {
	pos = vec3(x, y, z);
	graphicsOutOfDate = true;
}


Chunk::~Chunk() {

}

void Chunk::update(GLfloat delta) {

}

void Chunk::draw() {
	if (graphicsOutOfDate) {


		graphicsOutOfDate = false;
	}
}

void Chunk::addVoxel(int x, int y, int z) {

}

void Chunk::pushGraphicsData() {

}