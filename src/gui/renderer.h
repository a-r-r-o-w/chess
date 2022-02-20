/**
 * Chess
 * 
 * Written by:
 *    Aryan V S         (PES1UG20CS083)
 *    Aryansh Bhargavan (PES1UG20CS084)
 *    Avanish Bhat      (PES1UG20CS092)
 */

#ifndef chess_renderer_h
#define chess_renderer_h

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "exception.h"
#include "vector.h"

#include "vertex_array.h"
#include "shader.h"

#include "chess/chess.h"

typedef struct Renderer Renderer;
typedef struct Window   Window;

/* Function declarations ------------------------------------------------------------------ */

void Window_Constructor   (Window*, int, int, const char*);
void Window_AddHint       (int, int);
void Window_Activate      (Window*);
void Window_SetClearColor (Window*, int*);
void Window_SetPosition   (Window*, int, int);
bool is_WindowOpen        (Window*);
void Window_ClearScreen   (Window*);
void Window_SwapBuffers   (Window*);

void Renderer_Constructor  (Renderer*);
void Renderer_CreateWindow (Renderer*, int, int, const char*);
void Renderer_Destructor   (Renderer*);

void framebuffer_size_callback (GLFWwindow*, int, int);

/* Object types --------------------------------------------------------------------------- */

struct Window {
    GLFWwindow*  m_Window;
    int          m_Width;
    int          m_Height;
    float        m_ClearColor[4];
};

vector_t(Window)

struct Renderer {
    vector(Window) m_Windows;
    int ScreenWidth;
    int ScreenHeight;
};

/* Function definitions ------------------------------------------------------------------- */

/**
 * Construct a window
 *   Window*: Pointer to Window object
 *       int: Width of window
 *       int: Height of window
 *      name: Title/Name of window
 */
void Window_Constructor (Window* window, int width, int height, const char* name) {
    window->m_Width  = width;
    window->m_Height = height;

    window->m_Window = glfwCreateWindow(width, height, name, NULL, NULL);

    GL_ASSERT(window->m_Window != NULL);

    glfwMakeContextCurrent(window->m_Window);

    glfwSetFramebufferSizeCallback(window->m_Window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        EXCEPTION("Failed to initialize GLAD!");
    
    for (int i = 0; i < 3; ++i)
        window->m_ClearColor[i] = 0.0f;
    window->m_ClearColor[3] = 1.0f;
}

/**
 * Add window creation hints and set some basic properties
 */
void Window_AddHint (int hint, int value)
{ glfwWindowHint(hint, value); }

/**
 * Set a window to be the current OpenGL context
 * All rendering happens in the current context
 */
void Window_Activate (Window* window)
{ glfwMakeContextCurrent(window->m_Window); }

/**
 * Set the clear color for a window
 */
void Window_SetClearColor (Window* window, int* vec4) {
    const static float max_RGBAValue = 255.0f;

    for (int i = 0; i < 4; ++i)
        window->m_ClearColor[i] = vec4[i] / max_RGBAValue;
}

/**
 * Set the location where window should be rendered
 */
void Window_SetPosition (Window* window, int x, int y)
{ glfwSetWindowPos(window->m_Window, x, y); }

/**
 * Check if a window is open or closed
 */
bool is_WindowOpen (Window* window)
{ return !glfwWindowShouldClose(window->m_Window); }

/**
 * Flush clear color onto screen
 */
void Window_ClearScreen (Window* window) {
    float r = window->m_ClearColor[0];
    float g = window->m_ClearColor[1];
    float b = window->m_ClearColor[2];
    float a = window->m_ClearColor[3];

    GL_CALL(glClearColor(r, g, b, a));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}

/**
 * Swap display buffers for a window
 */
void Window_SwapBuffers (Window* window)
{ glfwSwapBuffers(window->m_Window); }

/**
 * Create a renderer to create and manipulate Window objects
 */
void Renderer_Constructor (Renderer* renderer) {
    glfwInit();
    vector_Window_constructor(&renderer->m_Windows, 0, (Window) { });
    
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    renderer->ScreenWidth  = mode->width;
    renderer->ScreenHeight = mode->height;
}

/**
 * Create a new window to render
 *   Parameters for Window_Constructor
 */
void Renderer_CreateWindow (Renderer* renderer, int width, int height, const char* name) {
    vector_Window_pushback(&renderer->m_Windows, (Window) { });
    
    Window* window = &renderer->m_Windows.data[renderer->m_Windows.size - 1];
    Window_Constructor(window, width, height, name);
}

/**
 * Destroy the renderer instance
 */
void Renderer_Destructor (Renderer* renderer) {
    vector_Window_destructor(&renderer->m_Windows);
    glfwTerminate();
}

/**
 * Adjust OpenGL viewport when window is resized
 */
void framebuffer_size_callback (GLFWwindow* window, int width, int height)
{ GL_CALL(glViewport(0, 0, width, height)); }

#endif // chess_renderer_h
