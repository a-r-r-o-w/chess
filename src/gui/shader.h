#ifndef chess_shader_h
#define chess_shader_h

#include "stdio.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "exception.h"
#include "vector.h"

typedef struct Shader Shader;
typedef struct ShaderProgram ShaderProgram;

/* Function declarations ------------------------------------------------------------------ */

void Shader_Constructor (Shader*, GLenum, const char*);

void ShaderProgram_Constructor (ShaderProgram*);
void ShaderProgram_AddShader   (ShaderProgram*, Shader*);
void ShaderProgram_Link        (ShaderProgram*);
void ShaderProgram_Bind        (ShaderProgram*);
void ShaderProgram_Unbind      (ShaderProgram*);
void ShaderProgram_Destructor  (ShaderProgram*);

/* Object types --------------------------------------------------------------------------- */

struct Shader {
    GLenum m_Type;
    GLint  m_ID;
};

struct ShaderProgram {
    GLint m_ID;
};

/* Function definitions ------------------------------------------------------------------- */

/**
 * Construct an OpenGL shader
 *   GLenum: type of shader
 *   const char*: file path to shader
 */
void Shader_Constructor (Shader* shader, GLenum type, const char* filepath) {
    shader->m_Type = type;

    FILE* file = fopen(filepath, "r");
    if (file == NULL)
        EXCEPTION("Shader file could not be opened");
    
    vector(char) source;
    vector_char_constructor(&source, 0, '\0');
    
    char c;
    while ((c = fgetc(file)) != EOF)
        vector_char_pushback(&source, c);
    vector_char_pushback(&source, '\0');

    GL_CALL(shader->m_ID = glCreateShader(type));
    GL_CALL(glShaderSource(shader->m_ID, 1, (const char* const*)(&source.data), (void*)0));
    GL_CALL(glCompileShader(shader->m_ID));

    vector_char_destructor(&source);
    fclose(file);
    file = NULL;

    const static int size = 1024;
    int is_Success;
    char InfoLog[size];

    glGetShaderiv(shader->m_ID, GL_COMPILE_STATUS, &is_Success);
    if (!is_Success) {
        glGetShaderInfoLog(shader->m_ID, size, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader!\n"
                        "  type: %s\n"
                        "%s",
                        shader->m_Type == GL_VERTEX_SHADER ? "vertex" : "fragment", InfoLog);
        EXCEPTION("Shader Compilation Failed!");
    }
}

/**
 * Construct a shader program to use OpenGL shaders wrapped as a Shader object
 */
void ShaderProgram_Constructor (ShaderProgram* program)
{ GL_CALL(program->m_ID = glCreateProgram()); }

/**
 * Add a Shader to the shader program
 */
void ShaderProgram_AddShader (ShaderProgram* program, Shader* shader)
{ GL_CALL(glAttachShader(program->m_ID, shader->m_ID)); }

/**
 * Link the compiled shaders attached to the shader program
 */
void ShaderProgram_Link (ShaderProgram* program)
{ GL_CALL(glLinkProgram(program->m_ID)); }

/**
 * Bind a shader program to the current context
 */
void ShaderProgram_Bind (ShaderProgram* program)
{ GL_CALL(glUseProgram(program->m_ID)); }

/**
 * Unbind a shader program from the current context
 */
void ShaderProgram_Unbind (ShaderProgram* program)
{ GL_CALL(glUseProgram(0)); }

/**
 * Destroy the shader program
 */
void ShaderProgram_Destructor (ShaderProgram* program)
{ GL_CALL(glDeleteProgram(program->m_ID)); }

#endif // chess_shader_h
