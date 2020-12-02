#include "Scene.h"

#include "lib/ResourceLoader.h"

using namespace CS123::GL;

Scene::Scene()
{
    // loading and compiling shader
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/test.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/test.frag");
    m_shader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

Scene::~Scene()
{
}

void Scene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shader->bind();
    // hardcoded triangle
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    static const GLfloat g_vertex_buffer_data[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
    };
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
       0,
       3,
       GL_FLOAT,
       GL_FALSE,
       0,
       (void*)0
    );
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    m_shader->unbind();
}
