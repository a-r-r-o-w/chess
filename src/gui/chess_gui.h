/**
 * Chess
 * 
 * Written by:
 *    Aryan V S         (PES1UG20CS083)
 *    Aryansh Bhargavan (PES1UG20CS084)
 *    Avanish Bhat      (PES1UG20CS092)
 */

#ifndef chess_gui_h
#define chess_gui_h

#include "gui/renderer.h"
#include "gui/shader.h"
#include "gui/vertex_array.h"
#include "gui/texture.h"

#include "chess/chess.h"
#include "chess/move_generation.h"

const static int BoardWindow_Width  = 640;
const static int BoardWindow_Height = 640;
const static int padding     = 20;
const static int BoardWidth  = BoardWindow_Width  - 2 * padding;
const static int BoardHeight = BoardWindow_Height - 2 * padding;
const static int BoxHeight   = BoardHeight / 8;
const static int BoxWidth    = BoardWidth  / 8;
static int selectedPieceIndex = -1;

const static double trasparencyFactor = 0.6f;

static double mouseX = 0;
static double mouseY = 0;

float OriginalColors[8][8][3];
float BoxColors[8][8][3];

vector_int movesCache;

static GLint u_Color;
static GLint u_RowShift;
static GLint u_ColShift;
static GLint u_isTexture;

static bool isOccupied[64];

const static float highlightColor[] = {0.3f, 0.7f, 0.1f};
const static float attackColor[] = {0.7f, 0.3f, 0.3f};

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
void Window0_Process (Window*, Chess*);

void drawBoard  ();
void drawPieces (Chess*, Texture[]);
void drawSelectColor (Window*, Chess*);
void drawMoves  (Window*, Chess*);
void clearMoves (Window*, Chess*);

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
    Window_SetClearColor(&renderer->m_Windows.data[0], (int[]) { 150, 140, 200, 255 });
    Window_SetPosition(&renderer->m_Windows.data[0], xPos0, yPos0);
}

void GameLoop (Renderer* renderer, Chess* chess) {
    for (int i = 0; i < 16; ++i) {
        if (chess->m_White[i].m_Position != -1)
            isOccupied[chess->m_White[i].m_Position] = true;
        if (chess->m_Black[i].m_Position != -1)
            isOccupied[chess->m_Black[i].m_Position] = true;
    }

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
            OriginalColors[i][j][0] = 128.0f;
            OriginalColors[i][j][1] = 80.0f;
            OriginalColors[i][j][2] = 20.0f;
        }
        else {
            OriginalColors[i][j][0] = 224.0f;
            OriginalColors[i][j][1] = 224.0f;
            OriginalColors[i][j][2] = 224.0f;
        }
        for (int k = 0; k < 3; ++k) {
            OriginalColors[i][j][k] /= max_RGBAValue;
            BoxColors[i][j][k] = OriginalColors[i][j][k];
        }
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
    
    GL_CALL(u_Color     = glGetUniformLocation(program.m_ID, "u_Color"));
    GL_CALL(u_RowShift  = glGetUniformLocation(program.m_ID, "u_RowShift"));
    GL_CALL(u_ColShift  = glGetUniformLocation(program.m_ID, "u_ColShift"));
    GL_CALL(u_isTexture = glGetUniformLocation(program.m_ID, "u_isTexture"));

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
        Window0_Process(window0, chess);

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

    drawBoard();
    drawPieces(chess, textures);
}

void Window0_Process (Window* window, Chess* chess) {
    if (glfwGetKey(window->m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window->m_Window, true);
    
    drawSelectColor(window, chess);
}

void drawBoard () {
    for (int i = 0; i < 8; ++i) for (int j = 0; j < 8; ++j) {
        float* color = BoxColors[i][j];
        
        GL_CALL(glUniform4f(u_Color, color[0], color[1], color[2], 1.0f));
        GL_CALL(glUniform1f(u_RowShift, (float)j * BoxWidth  / (BoardWindow_Width  / 2.0f)));
        GL_CALL(glUniform1f(u_ColShift, (float)i * BoxHeight / (BoardWindow_Height / 2.0f)));

        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
    }
}

void drawPieces (Chess* chess, Texture textures[]) {
    for (int i = 0; i < 16; ++i) {
        Piece* piece = &chess->m_White[i];

        if (piece->m_Position == -1)
            continue;
        
        int rank = piece->m_Position / 8;
        int file = piece->m_Position % 8;
        
        Texture_Bind(&textures[piece->m_Type], 0);

        GL_CALL(glUniform1f(u_RowShift, (float)file * BoxWidth  / (BoardWindow_Width  / 2.0f)));
        GL_CALL(glUniform1f(u_ColShift, (float)rank * BoxHeight / (BoardWindow_Height / 2.0f)));
        GL_CALL(glUniform1i(u_isTexture, true));
        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
        GL_CALL(glUniform1i(u_isTexture, false));
        
        Texture_Unbind(&textures[piece->m_Type]);
    }

    for (int i = 0; i < 16; ++i) {
        Piece* piece = &chess->m_Black[i];

        if (piece->m_Position == -1)
            continue;
        
        int rank = piece->m_Position / 8;
        int file = piece->m_Position % 8;
        
        Texture_Bind(&textures[piece->m_Type], 0);

        GL_CALL(glUniform1f(u_RowShift, (float)file * BoxWidth  / (BoardWindow_Width  / 2.0f)));
        GL_CALL(glUniform1f(u_ColShift, (float)rank * BoxHeight / (BoardWindow_Height / 2.0f)));
        GL_CALL(glUniform1i(u_isTexture, true));
        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));
        GL_CALL(glUniform1i(u_isTexture, false));
        
        Texture_Unbind(&textures[piece->m_Type]);
    }
}

void drawSelectColor (Window* window, Chess* chess) {
    static int oldClickState = GLFW_RELEASE;
    static int old_mX = -1;
    static int old_mY = -1;

    int newClickState = glfwGetMouseButton(window->m_Window, GLFW_MOUSE_BUTTON_LEFT);

    if (newClickState == GLFW_RELEASE && oldClickState == GLFW_PRESS) {
        glfwGetCursorPos(window->m_Window, &mouseX, &mouseY);

        if (mouseX >= padding &&
            mouseY >= padding &&
            mouseX < BoardWindow_Width - padding &&
            mouseY < BoardWindow_Height - padding) {
        
            int mX = (mouseY - padding) / BoxHeight;
            int mY = (mouseX - padding) / BoxWidth;

            int newIndex = 8 * mX + mY;

            if (isOccupied[newIndex]) {
                if (old_mX == -1 && old_mY == -1) {
                    
                    old_mX = mX;
                    old_mY = mY;
                    selectedPieceIndex = indexify(mY + 'a', 8 - mX - 1 + '1');

                    for (int i = 0; i < 3; ++i)
                        BoxColors[mX][mY][i] = trasparencyFactor * BoxColors[mX][mY][i] + (1 - trasparencyFactor) * highlightColor[i];
                    
                    drawMoves(window, chess);
                }
                else if (old_mX == mX && old_mY == mY) {
                    oldClickState = GLFW_RELEASE;
                    return;
                }
                else {
                    for (int i = 0; i < 3; ++i) {
                        BoxColors[old_mX][old_mY][i] = OriginalColors[old_mX][old_mY][i];
                        BoxColors[mX][mY][i] = trasparencyFactor * BoxColors[mX][mY][i] + (1 - trasparencyFactor) * highlightColor[i];
                    }

                    bool changed = false;
                    for (int i = 0; i < movesCache.size; ++i) {
                        if (newIndex == movesCache.data[i]) {
                            for (int j = 0; j < 16; ++j) {
                                if (chess->m_White[j].m_Position == selectedPieceIndex) {
                                    isOccupied[selectedPieceIndex] = false;
                                    chess->m_White[j].m_Position = newIndex;

                                    if (isOccupied[newIndex]) {
                                        bool captured = false;
                                        for (int i = 0; i < 16; ++i) {
                                            if (chess->m_Black[i].m_Position == newIndex) {
                                                chess->m_Black[i].m_Position = -1;
                                                captured = true;
                                            }
                                        }
                                        if (!captured)
                                            EXCEPTION("piece must have been captured");
                                    }

                                    isOccupied[newIndex] = true;
                                    changed = true;
                                    break;
                                }
                                if (chess->m_Black[j].m_Position == selectedPieceIndex) {
                                    isOccupied[selectedPieceIndex] = false;
                                    chess->m_Black[j].m_Position = newIndex;

                                    if (isOccupied[newIndex]) {
                                        bool captured = false;
                                        for (int i = 0; i < 16; ++i) {
                                            if (chess->m_White[i].m_Position == newIndex) {
                                                chess->m_White[i].m_Position = -1;
                                                captured = true;
                                            }
                                        }
                                        if (!captured)
                                            EXCEPTION("piece must have been captured");
                                    }

                                    isOccupied[newIndex] = true;
                                    changed = true;
                                    break;
                                }
                            }
                        }
                    }

                    clearMoves(window, chess);

                    if (changed) {
                        old_mX = -1;
                        old_mY = -1;
                        selectedPieceIndex = -1;
                    }
                    else {
                        old_mX = mX;
                        old_mY = mY;
                        selectedPieceIndex = newIndex;
                        drawMoves(window, chess);
                    }
                }
            }
            else {
                if (old_mX == -1 && old_mY == -1) {
                    oldClickState = GLFW_RELEASE;
                    return;
                }
                else {
                    for (int i = 0; i < 3; ++i)
                        BoxColors[old_mX][old_mY][i] = OriginalColors[old_mX][old_mY][i];
                    
                    for (int i = 0; i < movesCache.size; ++i) {
                        if (newIndex == movesCache.data[i]) {
                            bool changed = false;

                            for (int j = 0; j < 16; ++j) {
                                if (chess->m_White[j].m_Position == selectedPieceIndex) {
                                    isOccupied[selectedPieceIndex] = false;
                                    chess->m_White[j].m_Position = newIndex;

                                    if (isOccupied[newIndex]) {
                                        bool captured = false;
                                        for (int k = 0; k < 16; ++k) {
                                            if (chess->m_Black[k].m_Position == newIndex) {
                                                chess->m_Black[k].m_Position = -1;
                                                captured = true;
                                            }
                                        }
                                        if (!captured)
                                            EXCEPTION("piece must have been captured");
                                    }

                                    isOccupied[newIndex] = true;
                                    changed = true;

                                    break;
                                }
                                if (chess->m_Black[j].m_Position == selectedPieceIndex) {
                                    isOccupied[selectedPieceIndex] = false;
                                    chess->m_Black[j].m_Position = newIndex;

                                    if (isOccupied[newIndex]) {
                                        bool captured = false;
                                        for (int k = 0; k < 16; ++k) {
                                            if (chess->m_White[k].m_Position == newIndex) {
                                                chess->m_White[k].m_Position = -1;
                                                captured = true;
                                            }
                                        }
                                        if (!captured)
                                            EXCEPTION("piece must have been captured");
                                    }

                                    isOccupied[newIndex] = true;
                                    changed = true;
                                    break;
                                }
                            }
                            
                            if (!changed)
                                EXCEPTION("piece position should change")
                            break;
                        }
                    }

                    oldClickState = GLFW_RELEASE;
                    old_mX = -1;
                    old_mY = -1;
                    clearMoves(window, chess);
                    selectedPieceIndex = -1;
                }
            }
        }
    }

    oldClickState = newClickState;
}

void drawMoves (Window* window, Chess* chess) {
    vector_int_constructor(&movesCache, 0, 0);

    Piece* selectedPiece = NULL;
    int count = 0;

    for (int i = 0; i < 16; ++i)
        if (chess->m_White[i].m_Position == selectedPieceIndex) {
            selectedPiece = &chess->m_White[i];
            ++count;
        }
    
    for (int i = 0; i < 16; ++i)
        if (chess->m_Black[i].m_Position == selectedPieceIndex) {
            selectedPiece = &chess->m_Black[i];
            ++count;
        }
    if (count != 1)
        EXCEPTION("None/Multiple pieces found at selected piece index");

    generateMoves(chess, selectedPiece, &movesCache);

    for (int i = 0; i < movesCache.size; ++i) {
        int file = movesCache.data[i] % 8;
        int rank = movesCache.data[i] / 8;

        for (int j = 0; j < 3; ++j)
            BoxColors[rank][file][j] = attackColor[j];
    }
}

void clearMoves (Window* window, Chess* chess) {
    for (int i = 0; i < movesCache.size; ++i) {
        int file = movesCache.data[i] % 8;
        int rank = movesCache.data[i] / 8;

        for (int j = 0; j < 3; ++j)
            BoxColors[rank][file][j] = OriginalColors[rank][file][j];
    }
    vector_int_destructor(&movesCache);
}

bool checkTurn (Chess* chess, int position) {
    if (chess->m_IsWhiteTurn) {
        for (int i = 0; i < 16; ++i) {
            if (chess->m_White[i].m_Position == position)
                return true;
        }
    }
    else {
        for (int i = 0; i < 16; ++i) {
            if (chess->m_Black[i].m_Position == position)
                return true;
        }
    }
    return false;
}

#endif // chess_gui_h
