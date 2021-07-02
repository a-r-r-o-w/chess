#ifndef chess_gui_h
#define chess_gui_h

#include "gui/renderer.h"
#include "gui/shader.h"
#include "gui/vertex_array.h"
#include "gui/texture.h"

#include "chess/chess.h"

const int BoardWindow_Width  = 640;
const int BoardWindow_Height = 640;
const int padding      = 20;
const int BoardWidth  = BoardWindow_Width  - 2 * padding;
const int BoardHeight = BoardWindow_Height - 2 * padding;
const int BoxHeight   = BoardHeight / 8;
const int BoxWidth    = BoardWidth  / 8;

static GLfloat BoxColors[8][8][3];

#define AddPNG(x) [x] = "../resources/png/" #x ".png"

static const char* pngFiles[] = {
    AddPNG(white_king),
    AddPNG(white_queen),
    AddPNG(white_rook),
    AddPNG(white_bishop),
    AddPNG(white_knight),
    AddPNG(white_pawn),
    AddPNG(black_king),
    AddPNG(black_queen),
    AddPNG(black_rook),
    AddPNG(black_bishop),
    AddPNG(black_knight),
    AddPNG(black_pawn)
};

/* Function declarations ------------------------------------------------------------------ */

void ChessGUI        (Chess*);
void WindowSetup     (Renderer*);
void GameLoop        (Renderer*, Chess*);
void Window0_Update  (Window*, Chess*, VertexArray*, IndexBuffer*, ShaderProgram*, Texture[]);
void Window0_Process (Window*);

/* Object types --------------------------------------------------------------------------- */

/* Function definitions ------------------------------------------------------------------- */

void ChessGUI (Chess* chess) {
    Renderer renderer;
    Renderer_Constructor(&renderer);

    WindowSetup(&renderer);

    GameLoop(&renderer, chess);

    Renderer_Destructor(&renderer);
}

void WindowSetup (Renderer* renderer) {
    // Set up first window
    Window_AddHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    Window_AddHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    Window_AddHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    Window_AddHint(GLFW_RESIZABLE, GLFW_FALSE);

    const int xPos0 = (renderer->ScreenWidth  - BoardWindow_Width)  / 4;
    const int yPos0 = (renderer->ScreenHeight - BoardWindow_Height) / 2;

    Renderer_CreateWindow(renderer, BoardWindow_Width, BoardWindow_Height, "Board");
    Window_SetClearColor(&renderer->m_Windows.data[0], (int[]) { 128, 128, 128, 255 });
    Window_SetPosition(&renderer->m_Windows.data[0], xPos0, yPos0);
}

void GameLoop (Renderer* renderer, Chess* chess) {

    GLfloat vertices[] = {
      // Box coordinates                                                                           Texture coordinates
        -BoardWindow_Width / 2 + padding           , BoardWindow_Height / 2 - padding            , 0.0f, 1.0f,
        -BoardWindow_Width / 2 + padding + BoxWidth, BoardWindow_Height / 2 - padding            , 1.0f, 1.0f,
        -BoardWindow_Width / 2 + padding + BoxWidth, BoardWindow_Height / 2 - padding - BoxHeight, 1.0f, 0.0f,
        -BoardWindow_Width / 2 + padding           , BoardWindow_Height / 2 - padding - BoxHeight, 0.0f, 0.0f
    };

    for (int i = 0; i < 4 * 4; i += 4) {
        vertices[i]     /= BoardWindow_Width  / 2.0f;
        vertices[i + 1] /= BoardWindow_Height / 2.0f;
    }

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    const static float max_RGBAValue = 255.0f;

    for (int i = 0; i < 8; ++i) for (int j = 0; j < 8; ++j) {
        int parity = (i + j) % 2;
        if (parity) {
            BoxColors[i][j][0] = 128.0f;
            BoxColors[i][j][1] = 80.0f;
            BoxColors[i][j][2] = 20.0f;
        }
        else {
            BoxColors[i][j][0] = 224.0f;
            BoxColors[i][j][1] = 224.0f;
            BoxColors[i][j][2] = 224.0f;
        }
        for (int k = 0; k < 3; ++k)
            BoxColors[i][j][k] /= max_RGBAValue;
    }

    Window_Activate(&renderer->m_Windows.data[0]);

    GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GL_CALL(glEnable(GL_BLEND));

    VertexArray vao;
    VertexArray_Constructor(&vao);

    VertexBuffer vbo;
    VertexBuffer_Constructor(&vbo, vertices, 4 * 4 * GetSize(GL_FLOAT));

    BufferLayout layout;
    BufferLayout_Constructor(&layout);
    BufferLayout_Add_GLfloat(&layout, 2);
    BufferLayout_Add_GLfloat(&layout, 2);

    VertexArray_AddBuffer(&vao, &vbo, &layout);

    IndexBuffer ibo;
    IndexBuffer_Constructor(&ibo, indices, 6);

    Shader vertexShader;
    Shader fragmentShader;
    Shader_Constructor(&vertexShader, GL_VERTEX_SHADER, "../src/gui/shader/vertex.glsl");
    Shader_Constructor(&fragmentShader, GL_FRAGMENT_SHADER, "../src/gui/shader/fragment.glsl");

    ShaderProgram program;
    ShaderProgram_Constructor(&program);
    ShaderProgram_AddShader(&program, &vertexShader);
    ShaderProgram_AddShader(&program, &fragmentShader);
    ShaderProgram_Link(&program);
    ShaderProgram_Bind(&program);

    Texture textures[12];
    for (int i = 0; i < 12; ++i)
        Texture_Constructor(&textures[i], pngFiles[i]);

    // Texture_Bind(&textures[8], 0);
    // GLint u_Texture = glGetUniformLocation(program.m_ID, "u_Texture");
    // glUniform1i(u_Texture, 0);

    while (true) {
        bool isWindowClosed = false;
        for (int i = 0; i < renderer->m_Windows.size; ++i) {
            Window* window = &renderer->m_Windows.data[i];

            if (!is_WindowOpen(window)) {
                isWindowClosed = true;
                break;
            }

            Window_ClearScreen(window);
        }

        if (isWindowClosed)
            break;
        
        // Handle Window 0 - "Board"
        Window* window0 = &renderer->m_Windows.data[0];
        Window_Activate(window0);
        Window0_Update(window0, chess, &vao, &ibo, &program, textures);
        Window0_Process(window0);

        for (int i = 0; i < renderer->m_Windows.size; ++i) {
            Window* window = &renderer->m_Windows.data[i];
            Window_SwapBuffers(window);
        }
        
        glfwPollEvents();
    }

    ShaderProgram_Destructor(&program);
    IndexBuffer_Destructor(&ibo);
    BufferLayout_Destructor(&layout);
    VertexBuffer_Destructor(&vbo);
    VertexArray_Destructor(&vao);
}

void Window0_Update (Window* window, Chess* chess, VertexArray* vao, IndexBuffer* ibo, ShaderProgram* program, Texture textures[]) {
    ShaderProgram_Bind(program);
    VertexArray_Bind(vao);
    IndexBuffer_Bind(ibo);

    GL_CALL(GLint u_Color    = glGetUniformLocation(program->m_ID, "u_Color"));
    GL_CALL(GLint u_RowShift = glGetUniformLocation(program->m_ID, "u_RowShift"));
    GL_CALL(GLint u_ColShift = glGetUniformLocation(program->m_ID, "u_ColShift"));
    GL_CALL(GLint is_tex = glGetUniformLocation(program->m_ID, "u_isTexture"));

    for (int i = 0; i < 8; ++i) for (int j = 0; j < 8; ++j) {
        float* color = BoxColors[i][j];
        
        GL_CALL(glUniform4f(u_Color, color[0], color[1], color[2], 1.0f));
        GL_CALL(glUniform1f(u_RowShift, (float)j * BoxWidth  / (BoardWindow_Width  / 2.0f)));
        GL_CALL(glUniform1f(u_ColShift, (float)i * BoxHeight / (BoardWindow_Height / 2.0f)));

        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
    }

    for (int i = 0; i < 16; ++i) {
        if (chess->m_White[i].m_Position != -1) {
            Texture_Bind(&textures[chess->m_White[i].m_Type], 0);
            int rank = chess->m_White[i].m_Position / 8;
            int file = chess->m_White[i].m_Position % 8;
            GL_CALL(glUniform1f(u_RowShift, (float)file * BoxWidth  / (BoardWindow_Width  / 2.0f)));
            GL_CALL(glUniform1f(u_ColShift, (float)rank * BoxHeight / (BoardWindow_Height / 2.0f)));
            glUniform1i(is_tex, true);
            GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
            glUniform1i(is_tex, false);
            Texture_Unbind(&textures[chess->m_White[i].m_Type]);
        }
    }

    for (int i = 0; i < 16; ++i) {
        if (chess->m_Black[i].m_Position != -1) {
            Texture_Bind(&textures[chess->m_Black[i].m_Type], 0);
            int rank = chess->m_Black[i].m_Position / 8;
            int file = chess->m_Black[i].m_Position % 8;
            GL_CALL(glUniform1f(u_RowShift, (float)file * BoxWidth  / (BoardWindow_Width  / 2.0f)));
            GL_CALL(glUniform1f(u_ColShift, (float)rank * BoxHeight / (BoardWindow_Height / 2.0f)));
            glUniform1i(is_tex, true);
            GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
            glUniform1i(is_tex, false);
            Texture_Unbind(&textures[chess->m_Black[i].m_Type]);
        }
    }
}

void Window0_Process (Window* window) {
    if (glfwGetKey(window->m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window->m_Window, true);
}

#endif // chess_gui_h
