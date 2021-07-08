/**
 * Chess
 * 
 * Written by:
 *    Aryan V S         (PES1UG20CS083)
 *    Aryansh Bhargavan (PES1UG20CS084)
 *    Avanish Bhat      (PES1UG20CS092)
 */

#ifndef chess_exception_h
#define chess_exception_h

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

#include "glad/glad.h"
#include "glfw/glfw3.h"

#define EXCEPTION(x) exception(x, __FILE__, __LINE__);

#define GL_ASSERT(x) if (!(x)) EXCEPTION("GL_ASSERT failed on '" #x "'")

#define GL_CALL(x) GLClearError(); x; GL_ASSERT(GLLogError(#x, __FILE__, __LINE__));

void exception (const char* message, const char* file, int line) {
    fprintf(stderr, "Exception raised!\n"
                    "   File: %s\n"
                    "   Line: %d\n"
                    "Message: %s\n",
                    file, line, message);
    exit(-1);
}

/**
 * Clears all OpenGL errors
 */
void GLClearError ()
{ do ; while (glGetError() != GL_NO_ERROR); }

/**
 * Log all OpenGL errors on standard error stream
 */
bool GLLogError (const char* function, const char* file, int line) {
    GLenum error = 0;
    while ((error = glGetError())) {
        fprintf(stderr, "[OpenGL Error] (%d)\n"
                        "      File: %s\n"
                        "  Function: %s\n"
                        "      Line: %d\n",
                        error, file, function, line);
        return false;
    }
    return true;
}

#endif // chess_exception_h
