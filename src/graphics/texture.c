#include "texture.h"

#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint loadTexture(const char *path) {
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);             // OpenGL expects the origin at the bottom
	unsigned char *data = stbi_load(path, &width, &height, &channels, 4); // force RGBA
	if (data == NULL) {
		printf("[texture] could not load '%s' (%s)\n", path, stbi_failure_reason());
		return 0;
	}

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	// mipmaps: essential for big textures (e.g. 4096²) seen from far away.
	// Without them the GPU samples the full texture per pixel and the FPS tanks.
	// GL_GENERATE_MIPMAP lets the driver build the pyramid on the GPU (fast).
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
	printf("[texture] '%s' loaded (%dx%d)\n", path, width, height);
	return tex;
}
