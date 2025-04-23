#pragma once

// This is a cross-platform compatibility header
// It redirects to the appropriate GLUT header based on platform

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
// For Windows/Linux
#include_next <GL/glut.h>
#endif