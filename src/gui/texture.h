/**
 * Chess
 * 
 * Written by:
 *    Aryan V S         (PES1UG20CS083)
 *    Aryansh Bhargavan (PES1UG20CS084)
 *    Avanish Bhat      (PES1UG20CS092)
 */

#ifndef chess_texture_h
#define chess_texture_h

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "stb/stb_image.h"

#include "exception.h"

typedef struct Texture Texture;

/* Function declarations ------------------------------------------------------------------ */

void Texture_Constructor (Texture*, const char*);
void Texture_Bind        (Texture*, GLuint);
void Texture_Unbind      (Texture*);
void Texture_Destructor  (Texture*);

/* Object types --------------------------------------------------------------------------- */

struct Texture {
    GLuint m_ID;
    int    m_Width;
    int    m_Height;
    int    m_BitsPerPixel;
};

/* Function definitions ------------------------------------------------------------------- */

void Texture_Constructor (Texture* texture, const char* filepath) {
    texture->m_ID           = 0;
    texture->m_Width        = 0;
    texture->m_Height       = 0;
    texture->m_BitsPerPixel = 0;

    GL_CALL(glGenTextures(1, &texture->m_ID));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture->m_ID));

    stbi_set_flip_vertically_on_load(true);

    unsigned char* buffer = stbi_load(filepath, &texture->m_Width, &texture->m_Height, &texture->m_BitsPerPixel, 0);

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    if (buffer) {
        GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->m_Width, texture->m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
        GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
        stbi_image_free(buffer);
    }

    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture_Bind (Texture* texture, GLuint slot) {
    GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture->m_ID));
}

void Texture_Unbind (Texture* texture)
{ GL_CALL(glBindTexture(GL_TEXTURE_2D, 0)); }

void Texture_Destructor (Texture* texture)
{ GL_CALL(glDeleteTextures(1, &texture->m_ID)); }

#endif // chess_texture_h
