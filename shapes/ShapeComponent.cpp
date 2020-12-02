 #include "ShapeComponent.h"

ShapeComponent::ShapeComponent(glm::vec3 bottomLeft,  glm::vec3 topLeft, glm::vec3 bottomRight, glm::vec3 topRight, int rows, int cols,
                               std::vector<glm::vec3> normals, glm::mat4 rotation, glm::mat4 translation):
    m_bottom_left(bottomLeft),
    m_top_right(topRight),
    m_bottom_right(bottomRight),
    m_top_left(topLeft),
    m_rows(rows),
    m_cols(cols),
    m_normals(normals),
    rotationMatrix(rotation),
    translationMatrix(translation)
{
    makeVertices();
    makeTriangles();
}

ShapeComponent::ShapeComponent(glm::vec3 bottomLeft,  glm::vec3 topLeft, glm::vec3 bottomRight, glm::vec3 topRight,
                               int rows, int cols, std::vector<glm::vec3> normals):
    m_bottom_left(bottomLeft),
    m_top_right(topRight),
    m_bottom_right(bottomRight),
    m_top_left(topLeft),
    m_rows(rows),
    m_cols(cols),
    m_normals(normals),
    rotationMatrix(glm::rotate((float)0, glm::vec3(1, 0, 0))),
    translationMatrix(glm::translate(glm::vec3(0, 0, 0)))
{
    makeVertices();
    makeTriangles();
}

// Constructor used for cube shape
ShapeComponent::ShapeComponent(int rows, int cols, glm::mat4 rotation, glm::mat4 translation):
    m_bottom_left(glm::vec3(-0.5f, -0.5f, 0.0f)),
    m_top_right(glm::vec3(0.5f, 0.5f, 0.0f)),
    m_bottom_right(glm::vec3(m_top_right.x, m_bottom_left.y, 0)),
    m_top_left(glm::vec3 (m_bottom_left.x, m_top_right.y, 0)),
    m_rows(rows),
    m_cols(cols),
    rotationMatrix(rotation),
    translationMatrix(translation)
{
    glm::vec3 normal = (translationMatrix*rotationMatrix*glm::vec4(0, 0, 1, 1)).xyz();
    m_normals.push_back(normal);
    m_normals.push_back(normal);
    m_normals.push_back(normal);
    m_normals.push_back(normal);

    makeVertices();
    makeTriangles();
}

ShapeComponent::~ShapeComponent()
{
}

std::vector<float> ShapeComponent::getTriangles() {
    return triangles;
}

void insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

glm::vec3 lerp(float x1, float y1, float z1, float x2, float y2, float z2,float percent) {
    return glm::vec3(x1 + percent*(x2-x1), y1 + percent*(y2-y1), z1 + percent*(z2-z1));
}

void ShapeComponent::makeTriangles() {
    for (int i = 0; i < m_rows; i++) {
        for (int k = 0; k < m_cols; k++) {

            int v1 = i + (m_rows + 1) * k;
            int v2 = v1 + m_rows + 1;
            int v3 = v1 + m_rows + 2;
            int v4 = v1 + 1;

            glm::mat4 mat = translationMatrix *rotationMatrix;

            std::vector<glm::vec3> points = {(mat*glm::vec4(vertices[v1*3], vertices[v1*3 + 1], vertices[v1*3 + 2], 1.0f)).xyz(),
                                             (mat*glm::vec4(vertices[v2*3], vertices[v2*3 + 1], vertices[v2*3 + 2], 1.0f)).xyz(),
                                             (mat*glm::vec4(vertices[v3*3], vertices[v3*3 + 1], vertices[v3*3 + 2], 1.0f)).xyz(),
                                             (mat*glm::vec4(vertices[v4*3], vertices[v4*3 + 1], vertices[v4*3 + 2], 1.0f)).xyz()};

            addToTrianglesVector(points, m_normals);
        }
    }
}

void ShapeComponent::addToTrianglesVector(std::vector<glm::vec3> points, std::vector<glm::vec3> normals) {


    insertVec3(triangles, points[0]);
    insertVec3(triangles, normals[0]);

    insertVec3(triangles, points[1]);
    insertVec3(triangles, normals[2]);

    insertVec3(triangles, points[2]);
    insertVec3(triangles, normals[3]);

    insertVec3(triangles, points[0]);
    insertVec3(triangles, normals[0]);

    insertVec3(triangles, points[2]);
    insertVec3(triangles, normals[3]);

    insertVec3(triangles, points[3]);
    insertVec3(triangles, normals[1]);
}

/**
 * @brief Constructs vertices of all triangles in shape.
 */
void ShapeComponent::makeVertices() {
    float divideFactorRow = 1/static_cast<float>(m_rows);
    float divideFactorCol = 1/static_cast<float>(m_cols);
    for (int i = 0; i <= m_cols; i++) {

        glm::vec3 vBottom = lerp(m_bottom_left.x, m_bottom_left.y, m_bottom_left.z, m_bottom_right.x, m_bottom_right.y, m_bottom_right.z, i*divideFactorCol);

        for (int j = 0; j <= m_rows; j++) {
            glm::vec3 vTop = lerp(m_top_left.x, m_top_left.y, m_top_left.z, m_top_right.x, m_top_right.y, m_top_right.z, i*divideFactorCol);
            glm::vec3 v2 = lerp(vBottom.x, vBottom.y, vBottom.z, vTop.x, vTop.y, vTop.z, j*divideFactorRow);

            vertices.push_back(v2.x);
            vertices.push_back(v2.y);
            vertices.push_back(v2.z);
        }
    }
}
