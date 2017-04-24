#include "world.h"

GLchar* vertexShaderSource =
"#version 330\n"
"in vec3 pos;\n"
"in vec3 color;\n"
"in vec3 norm;\n"
"uniform mat4 modelMatrix;\n"
"uniform mat4 viewMatrix;\n"
"uniform mat4 projectionMatrix;\n"
"out vec3 vertexColor;\n"
"out vec3 vertexNormal;\n"
"out vec3 fragmentPosition;\n"
"void main() {\n"
"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(pos, 1.0f);\n"
"    vertexColor = color;\n"
"    vertexNormal = norm;\n"
"    fragmentPosition = vec3(modelMatrix * vec4(pos, 1.0f));\n"
"}\n";

GLchar* fragmentShaderSource =
"#version 330\n"
"uniform vec3 lightPos;\n"
"uniform vec3 lightColor;\n"
"in vec3 vertexColor;\n"
"in vec3 vertexNormal;\n"
"in vec3 fragmentPosition;\n"
"out vec4 color;\n"
"void main() {\n"
"    float ambientStrength = 0.1f;\n"
"    vec3 ambient = ambientStrength * lightColor;\n"
"    vec3 norm = normalize(vertexNormal);\n"
"    vec3 lightDir = normalize(lightPos - fragmentPosition);\n"
"    float diff = max(dot(norm, lightDir), 0.0);\n"
"    vec3 diffuse = diff * lightColor;\n"
"    vec3 result = (ambient + diffuse) * vertexColor;\n"
"    color = vec4(result, 1.0f);\n"
"}\n";

World::World() {
	printf("Creating new world\n");
	graphicsOutOfDate = hasInit = false;
	worldVAO = worldVerticesVBO = worldIndicesVBO = -1;
	shader_attrib_pos = shader_attrib_color = shader_attrib_norm = -1;
	shader_uniform_modelMatrix = shader_uniform_viewMatrix = shader_uniform_projectionMatrix = -1;
	shader_uniform_lightPos = shader_uniform_lightColor = -1;
	deltaTime = lastFrameTime = 0.0f;

	lightPos = vec3(0.0f, 0.0f, 0.0f);
	lightColor = vec3(1.0f, 1.0f, 1.0f);
}

World::~World() {
	printf("Destroying world\n");
	glDisableVertexAttribArray(shader_attrib_pos);
	glDisableVertexAttribArray(shader_attrib_color);
	glDisableVertexAttribArray(shader_attrib_norm);
	glDeleteBuffers(1, &worldVerticesVBO);
	glDeleteBuffers(1, &worldIndicesVBO);
	glDeleteVertexArrays(1, &worldVAO);
}

void World::init() {
	if (!hasInit) {
		vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderId, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShaderId);
		if (checkShaderCompileError(vertexShaderId) == -1) {
			exitWithPause(EXIT_FAILURE);
		}

		fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShaderId);
		if (checkShaderCompileError(fragmentShaderId) == -1) {
			exitWithPause(EXIT_FAILURE);
		}

		shaderProgramId = glCreateProgram();
		glAttachShader(shaderProgramId, vertexShaderId);
		glAttachShader(shaderProgramId, fragmentShaderId);
		glLinkProgram(shaderProgramId);
		if (checkShaderLinkError(shaderProgramId) == -1) {
			exitWithPause(EXIT_FAILURE);
		}

		//Bind shader program
		glUseProgram(shaderProgramId);
		
		//Retrieve input attribute locations
		shader_attrib_pos = glGetAttribLocation(shaderProgramId, "pos");
		shader_attrib_color = glGetAttribLocation(shaderProgramId, "color");
		shader_attrib_norm = glGetAttribLocation(shaderProgramId, "norm");

		//Retrieve uniform locations
		shader_uniform_modelMatrix = glGetUniformLocation(shaderProgramId, "modelMatrix");
		shader_uniform_viewMatrix = glGetUniformLocation(shaderProgramId, "viewMatrix");
		shader_uniform_projectionMatrix = glGetUniformLocation(shaderProgramId, "projectionMatrix");
		shader_uniform_lightPos = glGetUniformLocation(shaderProgramId, "lightPos");
		shader_uniform_lightColor = glGetUniformLocation(shaderProgramId, "lightColor");
		
		//Check for failures
		if (shader_uniform_modelMatrix < 0 || shader_uniform_viewMatrix < 0 || shader_uniform_projectionMatrix < 0) {
			printf("Failed to retrieve all input attribute and uniform locations from shader program\n");
			exitWithPause(EXIT_FAILURE);
		}
		
		projectionMatrix = perspective(radians(50.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);

		glUniformMatrix4fv(shader_uniform_modelMatrix, 1, GL_FALSE, value_ptr(modelMatrix));
		//glUniformMatrix4fv(shader_viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));
		glUniformMatrix4fv(shader_uniform_projectionMatrix, 1, GL_FALSE, value_ptr(projectionMatrix));

		if (shader_uniform_lightPos > -1) {
			glUniform3fv(shader_uniform_lightPos, 1, value_ptr(lightPos));
		}

		if (shader_uniform_lightColor > -1) {
			glUniform3fv(shader_uniform_lightColor, 1, value_ptr(lightColor));
		}

		camera = new Camera(CAMERA_MOVE, CAMERA_MOVE_FAST, CAMERA_SENSITIVITY);

		hasInit = true;
	}
}

void World::update() {
	GLfloat currentFrameTime = glfwGetTime();
	deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;


	//Camera look controls
	camera->lookX(MouseHandler::instance()->getXOffset() * deltaTime);
	camera->lookY(MouseHandler::instance()->getYOffset() * deltaTime);

	//Camera movement controls
	if (KeyHandler::instance()->isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
		camera->enableFastMode();
	} else {
		camera->disableFastMode();
	}

	if (KeyHandler::instance()->isKeyDown(GLFW_KEY_W)) {
		camera->moveZ(deltaTime);
	}
	if (KeyHandler::instance()->isKeyDown(GLFW_KEY_S)) {
		camera->moveZ(-deltaTime);
	}
	if (KeyHandler::instance()->isKeyDown(GLFW_KEY_A)) {
		camera->moveX(-deltaTime);
	}
	if (KeyHandler::instance()->isKeyDown(GLFW_KEY_D)) {
		camera->moveX(deltaTime);
	}
	if (KeyHandler::instance()->isKeyDown(GLFW_KEY_E)) {
		camera->moveY(deltaTime);
	}
	if (KeyHandler::instance()->isKeyDown(GLFW_KEY_Q)) {
		camera->moveY(-deltaTime);
	}

	//Light Position Controls
	if (KeyHandler::instance()->isKeyDown(GLFW_KEY_KP_8)) {
		lightPos.z -= CAMERA_MOVE * deltaTime;
	}
	if (KeyHandler::instance()->isKeyDown(GLFW_KEY_KP_5)) {
		lightPos.z += CAMERA_MOVE * deltaTime;
	}
	if (KeyHandler::instance()->isKeyDown(GLFW_KEY_KP_4)) {
		lightPos.x -= CAMERA_MOVE * deltaTime;
	}
	if (KeyHandler::instance()->isKeyDown(GLFW_KEY_KP_6)) {
		lightPos.x += CAMERA_MOVE * deltaTime;
	}
	if (KeyHandler::instance()->isKeyDown(GLFW_KEY_KP_7)) {
		lightPos.y -= CAMERA_MOVE * deltaTime;
	}
	if (KeyHandler::instance()->isKeyDown(GLFW_KEY_KP_9)) {
		lightPos.y += CAMERA_MOVE * deltaTime;
	}


	//lightPos = camera->getPos();
	if (shader_uniform_lightPos > -1) {
		glUniform3fv(shader_uniform_lightPos, 1, value_ptr(lightPos));
	}

	if (graphicsOutOfDate) {
		if (worldVAO == -1) {
			glGenVertexArrays(1, &worldVAO);
		}
		glBindVertexArray(worldVAO);

		if (worldVerticesVBO == -1) {
			glGenBuffers(1, &worldVerticesVBO);
			glBindBuffer(GL_ARRAY_BUFFER, worldVerticesVBO);
			glEnableVertexAttribArray(shader_attrib_pos);
			glEnableVertexAttribArray(shader_attrib_color);
			glEnableVertexAttribArray(shader_attrib_norm);
		}

		if (worldIndicesVBO == -1) {
			glGenBuffers(1, &worldIndicesVBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, worldIndicesVBO);
		}

		//Regenerate voxel data
		updateGraphicsData();

		if (vertices.size() > 0 && indices.size() > 0) {
			//Configure byte alignment for vertex position data
			glVertexAttribPointer(shader_attrib_pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

			if (shader_attrib_norm > -1) {
				//Configure byte alignment for vertex normal data
				glVertexAttribPointer(shader_attrib_norm, 3, GL_FLOAT, GL_TRUE, 0, (GLvoid*)(vertices.size() * sizeof(GLfloat)));
			}

			if (shader_attrib_color > -1) {
				//Configure byte alignment for vertex color data
				glVertexAttribPointer(shader_attrib_color, 3, GL_FLOAT, GL_TRUE, 0, (GLvoid*)((vertices.size() + normals.size()) * sizeof(GLfloat)));
			}

			//Send new voxel data over to GPU
			glBufferData(GL_ARRAY_BUFFER, (vertices.size() + normals.size() + colors.size()) * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), &vertices[0]);
				
			if (shader_attrib_norm > -1) {
				glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), normals.size() * sizeof(GLfloat), &normals[0]);
			}

			if (shader_attrib_color > -1) {
				glBufferSubData(GL_ARRAY_BUFFER, (vertices.size() + normals.size()) * sizeof(GLfloat), colors.size() * sizeof(GLfloat), &colors[0]);
			}

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

			graphicsOutOfDate = false;
		}
		glBindVertexArray(0);
	}
}

void World::render() {
	glUseProgram(shaderProgramId);
	glBindVertexArray(worldVAO);

	//Send camera position over to shader
	glUniformMatrix4fv(shader_uniform_viewMatrix, 1, GL_FALSE, value_ptr(camera->getLookAt()));
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
}

void World::addVoxel(int x, int y, int z) {
	voxels.push_back({ x, y, z });
	graphicsOutOfDate = true;
}

void World::updateGraphicsData() {
	//Clear stored vertex and index data
	vertices.clear();
	normals.clear();
	colors.clear();
	indices.clear();

	//Loop through voxel data to generate vertex data
	for (unsigned long i = 0; i < voxels.size(); i++) {
		World::voxel *voxel = &voxels[i];

		//Cube vertices
		vec3 p0 = vec3(voxel->x, voxel->y, voxel->z);
		vec3 p1 = vec3(voxel->x, voxel->y, voxel->z + VOXEL_SIZE_Z);
		vec3 p2 = vec3(voxel->x + VOXEL_SIZE_X, voxel->y, voxel->z + VOXEL_SIZE_Z);
		vec3 p3 = vec3(voxel->x + VOXEL_SIZE_X, voxel->y, voxel->z);
		vec3 p4 = vec3(voxel->x, voxel->y + VOXEL_SIZE_Y, voxel->z + VOXEL_SIZE_Z);
		vec3 p5 = vec3(voxel->x + VOXEL_SIZE_X, voxel->y + VOXEL_SIZE_Y, voxel->z + VOXEL_SIZE_Z);
		vec3 p6 = vec3(voxel->x + VOXEL_SIZE_X, voxel->y + VOXEL_SIZE_Y, voxel->z);
		vec3 p7 = vec3(voxel->x, voxel->y + VOXEL_SIZE_Y, voxel->z);

		//Vertex Colors
		vec3 c0 = vec3(0.0f, 0.0f, 0.0f);
		vec3 c1 = vec3(1.0f, 0.0f, 0.0f);
		vec3 c2 = vec3(0.0f, 1.0f, 0.0f);
		vec3 c3 = vec3(0.0f, 0.0f, 1.0f);
		vec3 c4 = vec3(1.0f, 1.0f, 0.0f);
		vec3 c5 = vec3(1.0f, 0.0f, 1.0f);
		vec3 c6 = vec3(0.0f, 1.0f, 1.0f);
		vec3 c7 = vec3(1.0f, 1.0f, 1.0f);

		//Face Normals
		vec3 n_front = vec3(0.0f, 0.0f, 1.0f);
		vec3 n_left = vec3(-1.0f, 0.0f, 0.0f);
		vec3 n_right = vec3(1.0f, 0.0f, 0.0f);
		vec3 n_back = vec3(0.0f, 0.0f, -1.0f);
		vec3 n_bottom = vec3(0.0f, -1.0f, 0.0f);
		vec3 n_top = vec3(0.0f, 1.0f, 0.0f);

		//Front-Top Face
		vertices.push_back(p1.x);
		vertices.push_back(p1.y);
		vertices.push_back(p1.z);
		colors.push_back(c1.r);
		colors.push_back(c1.g);
		colors.push_back(c1.b);
		normals.push_back(n_front.x);
		normals.push_back(n_front.y);
		normals.push_back(n_front.z);
		indices.push_back(i * 36);
		vertices.push_back(p5.x);
		vertices.push_back(p5.y);
		vertices.push_back(p5.z);
		colors.push_back(c5.r);
		colors.push_back(c5.g);
		colors.push_back(c5.b);
		normals.push_back(n_front.x);
		normals.push_back(n_front.y);
		normals.push_back(n_front.z);
		indices.push_back(i * 36 + 1);
		vertices.push_back(p4.x);
		vertices.push_back(p4.y);
		vertices.push_back(p4.z);
		colors.push_back(c4.r);
		colors.push_back(c4.g);
		colors.push_back(c4.b);
		normals.push_back(n_front.x);
		normals.push_back(n_front.y);
		normals.push_back(n_front.z);
		indices.push_back(i * 36 + 2);

		//Front-Bottom Face
		vertices.push_back(p1.x);
		vertices.push_back(p1.y);
		vertices.push_back(p1.z);
		colors.push_back(c1.r);
		colors.push_back(c1.g);
		colors.push_back(c1.b);
		normals.push_back(n_front.x);
		normals.push_back(n_front.y);
		normals.push_back(n_front.z);
		indices.push_back(i * 36 + 3);
		vertices.push_back(p2.x);
		vertices.push_back(p2.y);
		vertices.push_back(p2.z);
		colors.push_back(c2.r);
		colors.push_back(c2.g);
		colors.push_back(c2.b);
		normals.push_back(n_front.x);
		normals.push_back(n_front.y);
		normals.push_back(n_front.z);
		indices.push_back(i * 36 + 4);
		vertices.push_back(p5.x);
		vertices.push_back(p5.y);
		vertices.push_back(p5.z);
		colors.push_back(c5.r);
		colors.push_back(c5.g);
		colors.push_back(c5.b);
		normals.push_back(n_front.x);
		normals.push_back(n_front.y);
		normals.push_back(n_front.z);
		indices.push_back(i * 36 + 5);

		//Left-Top Face
		vertices.push_back(p0.x);
		vertices.push_back(p0.y);
		vertices.push_back(p0.z);
		colors.push_back(c0.r);
		colors.push_back(c0.g);
		colors.push_back(c0.b);
		normals.push_back(n_left.x);
		normals.push_back(n_left.y);
		normals.push_back(n_left.z);
		indices.push_back(i * 36 + 6);
		vertices.push_back(p4.x);
		vertices.push_back(p4.y);
		vertices.push_back(p4.z);
		colors.push_back(c4.r);
		colors.push_back(c4.g);
		colors.push_back(c4.b);
		normals.push_back(n_left.x);
		normals.push_back(n_left.y);
		normals.push_back(n_left.z);
		indices.push_back(i * 36 + 7);
		vertices.push_back(p7.x);
		vertices.push_back(p7.y);
		vertices.push_back(p7.z);
		colors.push_back(c7.r);
		colors.push_back(c7.g);
		colors.push_back(c7.b);
		normals.push_back(n_left.x);
		normals.push_back(n_left.y);
		normals.push_back(n_left.z);
		indices.push_back(i * 36 + 8);

		//Left-Bottom Face
		vertices.push_back(p0.x);
		vertices.push_back(p0.y);
		vertices.push_back(p0.z);
		colors.push_back(c0.r);
		colors.push_back(c0.g);
		colors.push_back(c0.b);
		normals.push_back(n_left.x);
		normals.push_back(n_left.y);
		normals.push_back(n_left.z);
		indices.push_back(i * 36 + 9);
		vertices.push_back(p1.x);
		vertices.push_back(p1.y);
		vertices.push_back(p1.z);
		colors.push_back(c1.r);
		colors.push_back(c1.g);
		colors.push_back(c1.b);
		normals.push_back(n_left.x);
		normals.push_back(n_left.y);
		normals.push_back(n_left.z);
		indices.push_back(i * 36 + 10);
		vertices.push_back(p4.x);
		vertices.push_back(p4.y);
		vertices.push_back(p4.z);
		colors.push_back(c4.r);
		colors.push_back(c4.g);
		colors.push_back(c4.b);
		normals.push_back(n_left.x);
		normals.push_back(n_left.y);
		normals.push_back(n_left.z);
		indices.push_back(i * 36 + 11);

		//Right-Top Face
		vertices.push_back(p2.x);
		vertices.push_back(p2.y);
		vertices.push_back(p2.z);
		colors.push_back(c2.r);
		colors.push_back(c2.g);
		colors.push_back(c2.b);
		normals.push_back(n_right.x);
		normals.push_back(n_right.y);
		normals.push_back(n_right.z);
		indices.push_back(i * 36 + 12);
		vertices.push_back(p6.x);
		vertices.push_back(p6.y);
		vertices.push_back(p6.z);
		colors.push_back(c6.r);
		colors.push_back(c6.g);
		colors.push_back(c6.b);
		normals.push_back(n_right.x);
		normals.push_back(n_right.y);
		normals.push_back(n_right.z);
		indices.push_back(i * 36 + 13);
		vertices.push_back(p5.x);
		vertices.push_back(p5.y);
		vertices.push_back(p5.z);
		colors.push_back(c5.r);
		colors.push_back(c5.g);
		colors.push_back(c5.b);
		normals.push_back(n_right.x);
		normals.push_back(n_right.y);
		normals.push_back(n_right.z);
		indices.push_back(i * 36 + 14);

		//Right-Bottom Face
		vertices.push_back(p2.x);
		vertices.push_back(p2.y);
		vertices.push_back(p2.z);
		colors.push_back(c2.r);
		colors.push_back(c2.g);
		colors.push_back(c2.b);
		normals.push_back(n_right.x);
		normals.push_back(n_right.y);
		normals.push_back(n_right.z);
		indices.push_back(i * 36 + 15);
		vertices.push_back(p3.x);
		vertices.push_back(p3.y);
		vertices.push_back(p3.z);
		colors.push_back(c3.r);
		colors.push_back(c3.g);
		colors.push_back(c3.b);
		normals.push_back(n_right.x);
		normals.push_back(n_right.y);
		normals.push_back(n_right.z);
		indices.push_back(i * 36 + 16);
		vertices.push_back(p6.x);
		vertices.push_back(p6.y);
		vertices.push_back(p6.z);
		colors.push_back(c6.r);
		colors.push_back(c6.g);
		colors.push_back(c6.b);
		normals.push_back(n_right.x);
		normals.push_back(n_right.y);
		normals.push_back(n_right.z);
		indices.push_back(i * 36 + 17);

		//Top-Back Face
		vertices.push_back(p5.x);
		vertices.push_back(p5.y);
		vertices.push_back(p5.z);
		colors.push_back(c5.r);
		colors.push_back(c5.g);
		colors.push_back(c5.b);
		normals.push_back(n_top.x);
		normals.push_back(n_top.y);
		normals.push_back(n_top.z);
		indices.push_back(i * 36 + 18);
		vertices.push_back(p6.x);
		vertices.push_back(p6.y);
		vertices.push_back(p6.z);
		colors.push_back(c6.r);
		colors.push_back(c6.g);
		colors.push_back(c6.b);
		normals.push_back(n_top.x);
		normals.push_back(n_top.y);
		normals.push_back(n_top.z);
		indices.push_back(i * 36 + 19);
		vertices.push_back(p7.x);
		vertices.push_back(p7.y);
		vertices.push_back(p7.z);
		colors.push_back(c7.r);
		colors.push_back(c7.g);
		colors.push_back(c7.b);
		normals.push_back(n_top.x);
		normals.push_back(n_top.y);
		normals.push_back(n_top.z);
		indices.push_back(i * 36 + 20);

		//Top-Front Face
		vertices.push_back(p4.x);
		vertices.push_back(p4.y);
		vertices.push_back(p4.z);
		colors.push_back(c4.r);
		colors.push_back(c4.g);
		colors.push_back(c4.b);
		normals.push_back(n_top.x);
		normals.push_back(n_top.y);
		normals.push_back(n_top.z);
		indices.push_back(i * 36 + 21);
		vertices.push_back(p5.x);
		vertices.push_back(p5.y);
		vertices.push_back(p5.z);
		colors.push_back(c5.r);
		colors.push_back(c5.g);
		colors.push_back(c5.b);
		normals.push_back(n_top.x);
		normals.push_back(n_top.y);
		normals.push_back(n_top.z);
		indices.push_back(i * 36 + 22);
		vertices.push_back(p7.x);
		vertices.push_back(p7.y);
		vertices.push_back(p7.z);
		colors.push_back(c7.r);
		colors.push_back(c7.g);
		colors.push_back(c7.b);
		normals.push_back(n_top.x);
		normals.push_back(n_top.y);
		normals.push_back(n_top.z);
		indices.push_back(i * 36 + 23);

		//Bottom-Back Face
		vertices.push_back(p0.x);
		vertices.push_back(p0.y);
		vertices.push_back(p0.z);
		colors.push_back(c0.r);
		colors.push_back(c0.g);
		colors.push_back(c0.b);
		normals.push_back(n_bottom.x);
		normals.push_back(n_bottom.y);
		normals.push_back(n_bottom.z);
		indices.push_back(i * 36 + 24);
		vertices.push_back(p3.x);
		vertices.push_back(p3.y);
		vertices.push_back(p3.z);
		colors.push_back(c3.r);
		colors.push_back(c3.g);
		colors.push_back(c3.b);
		normals.push_back(n_bottom.x);
		normals.push_back(n_bottom.y);
		normals.push_back(n_bottom.z);
		indices.push_back(i * 36 + 25);
		vertices.push_back(p2.x);
		vertices.push_back(p2.y);
		vertices.push_back(p2.z);
		colors.push_back(c2.r);
		colors.push_back(c2.g);
		colors.push_back(c2.b);
		normals.push_back(n_bottom.x);
		normals.push_back(n_bottom.y);
		normals.push_back(n_bottom.z);
		indices.push_back(i * 36 + 26);

		//Bottom-Front Face
		vertices.push_back(p0.x);
		vertices.push_back(p0.y);
		vertices.push_back(p0.z);
		colors.push_back(c0.r);
		colors.push_back(c0.g);
		colors.push_back(c0.b);
		normals.push_back(n_bottom.x);
		normals.push_back(n_bottom.y);
		normals.push_back(n_bottom.z);
		indices.push_back(i * 36 + 27);
		vertices.push_back(p2.x);
		vertices.push_back(p2.y);
		vertices.push_back(p2.z);
		colors.push_back(c2.r);
		colors.push_back(c2.g);
		colors.push_back(c2.b);
		normals.push_back(n_bottom.x);
		normals.push_back(n_bottom.y);
		normals.push_back(n_bottom.z);
		indices.push_back(i * 36 + 28);
		vertices.push_back(p1.x);
		vertices.push_back(p1.y);
		vertices.push_back(p1.z);
		colors.push_back(c1.r);
		colors.push_back(c1.g);
		colors.push_back(c1.b);
		normals.push_back(n_bottom.x);
		normals.push_back(n_bottom.y);
		normals.push_back(n_bottom.z);
		indices.push_back(i * 36 + 29);

		//Back-Top Face
		vertices.push_back(p3.x);
		vertices.push_back(p3.y);
		vertices.push_back(p3.z);
		colors.push_back(c3.r);
		colors.push_back(c3.g);
		colors.push_back(c3.b);
		normals.push_back(n_back.x);
		normals.push_back(n_back.y);
		normals.push_back(n_back.z);
		indices.push_back(i * 36 + 30);
		vertices.push_back(p7.x);
		vertices.push_back(p7.y);
		vertices.push_back(p7.z);
		colors.push_back(c7.r);
		colors.push_back(c7.g);
		colors.push_back(c7.b);
		normals.push_back(n_back.x);
		normals.push_back(n_back.y);
		normals.push_back(n_back.z);
		indices.push_back(i * 36 + 31);
		vertices.push_back(p6.x);
		vertices.push_back(p6.y);
		vertices.push_back(p6.z);
		colors.push_back(c6.r);
		colors.push_back(c6.g);
		colors.push_back(c6.b);
		normals.push_back(n_back.x);
		normals.push_back(n_back.y);
		normals.push_back(n_back.z);
		indices.push_back(i * 36 + 32);

		//Back-Bottom Face
		vertices.push_back(p0.x);
		vertices.push_back(p0.y);
		vertices.push_back(p0.z);
		colors.push_back(c0.r);
		colors.push_back(c0.g);
		colors.push_back(c0.b);
		normals.push_back(n_back.x);
		normals.push_back(n_back.y);
		normals.push_back(n_back.z);
		indices.push_back(i * 36 + 33);
		vertices.push_back(p7.x);
		vertices.push_back(p7.y);
		vertices.push_back(p7.z);
		colors.push_back(c7.r);
		colors.push_back(c7.g);
		colors.push_back(c7.b);
		normals.push_back(n_back.x);
		normals.push_back(n_back.y);
		normals.push_back(n_back.z);
		indices.push_back(i * 36 + 34);
		vertices.push_back(p3.x);
		vertices.push_back(p3.y);
		vertices.push_back(p3.z);
		colors.push_back(c3.r);
		colors.push_back(c3.g);
		colors.push_back(c3.b);
		normals.push_back(n_back.x);
		normals.push_back(n_back.y);
		normals.push_back(n_back.z);
		indices.push_back(i * 36 + 35);
	}
}