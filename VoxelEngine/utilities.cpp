#include "utilities.h"

void exitWithPause(int code) {
	system("pause");
	exit(code);
}

//Checks the compile status of the passed in shader id
GLint checkShaderCompileError(GLint shaderId) {
	GLint compileStatus;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus) {
		GLint errMsgLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errMsgLength);
		char* msgBuffer = new char[errMsgLength];
		glGetShaderInfoLog(shaderId, errMsgLength, NULL, msgBuffer);
		printf("Failed to compile shader: [%s]\n", msgBuffer);
		delete msgBuffer;
	}
	return compileStatus;
}

//Checks the link status of the passed in shader id
GLint checkShaderLinkError(GLint shaderId) {
	GLint compileStatus;
	glGetProgramiv(shaderId, GL_LINK_STATUS, &compileStatus);
	if (!compileStatus) {
		GLint errMsgLength;
		glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &errMsgLength);
		char* msgBuffer = new char[errMsgLength];
		glGetProgramInfoLog(shaderId, errMsgLength, NULL, msgBuffer);
		printf("Failed to link shader: [%s]\n", msgBuffer);
		delete msgBuffer;
	}
	return compileStatus;
}

float generate_float(float low, float high) {
	return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (high - low)));
}