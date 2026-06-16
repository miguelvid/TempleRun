#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glut.h>

// Loads a PNG/JPG as an OpenGL texture (with mipmaps). Returns 0 on failure.
GLuint loadTexture(const char *path);

#endif
