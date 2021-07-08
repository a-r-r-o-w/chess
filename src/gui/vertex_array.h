/**
 * Chess
 * 
 * Written by:
 *    Aryan V S         (PES1UG20CS083)
 *    Aryansh Bhargavan (PES1UG20CS084)
 *    Avanish Bhat      (PES1UG20CS092)
 */

#ifndef chess_vertex_array_h
#define chess_vertex_array_h

#include "glad/glad.h"
#include "glfw/glfw3.h"

#include "exception.h"
#include "vector.h"

typedef struct VertexBuffer  VertexBuffer;
typedef struct IndexBuffer   IndexBuffer;
typedef struct BufferElement BufferElement;
typedef struct BufferLayout  BufferLayout;
typedef struct VertexArray   VertexArray;

/* Function declarations ------------------------------------------------------------------ */

GLuint GetSize (GLuint);

void VertexBuffer_Constructor (VertexBuffer*, const void*, GLuint);
void VertexBuffer_Bind        (VertexBuffer*);
void VertexBuffer_Unbind      (VertexBuffer*);
void VertexBuffer_Destructor  (VertexBuffer*);

void IndexBuffer_Constructor (IndexBuffer*, const void*, GLuint);
void IndexBuffer_Bind        (IndexBuffer*);
void IndexBuffer_Unbind      (IndexBuffer*);
void IndexBuffer_Destructor  (IndexBuffer*);

void BufferLayout_Constructor (BufferLayout*);
void BufferLayout_Add_GLfloat (BufferLayout*, GLuint);
void BufferLayout_Destructor  (BufferLayout*);

void VertexArray_Constructor (VertexArray*);
void VertexArray_AddBuffer   (VertexArray*, VertexBuffer*, BufferLayout*);
void VertexArray_Bind        (VertexArray*);
void VertexArray_Unbind      (VertexArray*);
void VertexArray_Destructor  (VertexArray*);

/* Object types --------------------------------------------------------------------------- */

struct VertexBuffer {
    GLuint m_ID;
};

struct IndexBuffer {
    GLuint m_ID;
    GLuint m_Count;
};

struct BufferElement {
    GLenum m_Type;
    GLuint m_Count;
};

vector_t(BufferElement)

struct BufferLayout {
    vector(BufferElement) m_Elements;
    GLuint m_Stride;
};

struct VertexArray {
    GLuint m_ID;
};

/* Function definitions ------------------------------------------------------------------- */

/**
 * Helper function to return size of an OpenGL type
 */
GLuint GetSize (GLuint type) {
    switch (type) {
        case GL_FLOAT:         return sizeof(GLfloat);
        case GL_UNSIGNED_INT:  return sizeof(GLuint);
        case GL_UNSIGNED_BYTE: return sizeof(GLbyte);
    }
    EXCEPTION("invalid type");
    return 0;
}

/**
 * Construct a vertex buffer to store vertex data
 */
void VertexBuffer_Constructor (VertexBuffer* vb, const void* data, GLuint size) {
    GL_CALL(glGenBuffers(1, &vb->m_ID));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vb->m_ID));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

/**
 * Bind a vertex buffer to current context
 */
void VertexBuffer_Bind (VertexBuffer* vb)
{ GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vb->m_ID)); }

/**
 * Unbind a vertex buffer from current context
 */
void VertexBuffer_Unbind (VertexBuffer* vb)
{ GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

/**
 * Destroy a vertex buffer
 */
void VertexBuffer_Destructor (VertexBuffer* vb)
{ GL_CALL(glDeleteBuffers(1, &vb->m_ID)); }

/**
 * Construct a index buffer to store vertex data
 */
void IndexBuffer_Constructor (IndexBuffer* ib, const void* data, GLuint count) {
    ib->m_Count = count;
    GL_CALL(glGenBuffers(1, &ib->m_ID));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->m_ID));
    GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

/**
 * Bind an index buffer to current context
 */
void IndexBuffer_Bind (IndexBuffer* ib)
{ GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->m_ID)); }

/**
 * Unbind an index buffer from current context
 */
void IndexBuffer_Unbind (IndexBuffer* ib)
{ GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

/**
 * Destroy an index buffer
 */
void IndexBuffer_Destructor (IndexBuffer* ib)
{ GL_CALL(glDeleteBuffers(1, &ib->m_ID)); }

/**
 * Construct a buffer layout to represent data
 */
void BufferLayout_Constructor (BufferLayout* layout) {
    vector_BufferElement_constructor(&layout->m_Elements, 0, (BufferElement) { });
    layout->m_Stride = 0;
}

/**
 * Add GLfloat type data layout
 */
void BufferLayout_Add_GLfloat (BufferLayout* layout, GLuint count) {
    vector_BufferElement_pushback(&layout->m_Elements, (BufferElement) { .m_Type = GL_FLOAT, .m_Count = count });
    layout->m_Stride += count * GetSize(GL_FLOAT);
}

/**
 * Destroy a buffer layout
 */
void BufferLayout_Destructor (BufferLayout* layout)
{ vector_BufferElement_destructor(&layout->m_Elements); }

/**
 * Construct a vertex array
 */
void VertexArray_Constructor (VertexArray* va)
{ GL_CALL(glGenVertexArrays(1, &va->m_ID)); }

/**
 * Attach a vertex buffer to a vertex array with an appropriate layout
 */
void VertexArray_AddBuffer (VertexArray* va, VertexBuffer* vb, BufferLayout* layout) {
    VertexArray_Bind(va);
    VertexBuffer_Bind(vb);

    GLuint offset = 0;
    for (int i = 0; i < layout->m_Elements.size; ++i) {
        BufferElement* element = &layout->m_Elements.data[i];

        GL_CALL(glEnableVertexAttribArray(i));
        GL_CALL(glVertexAttribPointer(i, element->m_Count, element->m_Type, GL_FALSE, layout->m_Stride, (const void*)(uintptr_t)offset));

        offset += element->m_Count * GetSize(element->m_Type);
    }
}

/**
 * Bind a vertex array to current context
 */
void VertexArray_Bind (VertexArray* va)
{ GL_CALL(glBindVertexArray(va->m_ID)); }

/**
 * Unbind a vertex array from current context
 */
void VertexArray_Unbind (VertexArray* va)
{ GL_CALL(glBindVertexArray(0)); }

/**
 * Destroy a vertex array
 */
void VertexArray_Destructor (VertexArray* va)
{ GL_CALL(glDeleteVertexArrays(1, &va->m_ID)); }

#endif // chess_vertex_array_h
