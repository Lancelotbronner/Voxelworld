//
//  gl.h
//
//
//  Created by Christophe Bronner on 2023-10-20.
//

#pragma once

#include <OpenGL/gl3.h>

#ifndef DEBUG
#define glDebugClear
#define glDebugCheck
#else

#include <signal.h>
#include <stdio.h>

#define glDebugClear \
	while (glGetError());

#define glDebugCheck {\
	int error = glGetError(); \
	if (error) { \
		printf("[OpenGL] %s:%i failed with 0x%x", __func__, __LINE__, error); \
		raise(SIGINT); \
	}}

#endif
