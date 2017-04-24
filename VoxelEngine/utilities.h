#pragma once

#include "includes.h"

void exitWithPause(int code);

GLint checkShaderCompileError(GLint shaderId);

GLint checkShaderLinkError(GLint shaderId);

float generate_float(float low, float high);