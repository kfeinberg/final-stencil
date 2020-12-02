#include "CylinderComponent.h"
#include "ShapeComponent.h"

/**
 * @brief CylinderComponent::CylinderComponent
 * Creates a circular base. Used for cylinder, sphere, cone, torus.
 * @param param1 complexity of base
 * @param param2 number of sides
 * @param rotation rotation of the base
 * @param translation translation of the base
 * @param radius radius of the base
 */
CylinderComponent::CylinderComponent(int param1, int param2,
                                     glm::mat4 rotation, glm::mat4 translation, float radius, ShapeEnum shape):
    rotationMatrix(rotation),
    translationMatrix(translation),
    complexity(param1),
    sides(std::max(param2, 3)),
    m_radius(radius),
    m_shape(shape)
{
    makeVertices();
    makeTriangles();
}

CylinderComponent::CylinderComponent(int param1, int param2,
                                   glm::mat4 rotation, glm::mat4 translation, float radius, ShapeEnum shape, float phi):
    rotationMatrix(rotation),
    translationMatrix(translation),
    complexity(param1),
    sides(std::max(param2, 3)),
    m_phi(phi),
    m_radius(radius),
    m_shape(shape)
{
    makeVertices();
    makeTriangles();
}

CylinderComponent::~CylinderComponent()
{
}

void CylinderComponent::makeTriangles() {

    glm::vec3 normal = (rotationMatrix*glm::vec4(0, 0, 1, 1)).xyz();

    for (int i = 0; i < sides; i++) {

            int v1 = i;
            int v2 = i+1;

            glm::vec3 bottomLeft = glm::vec3(vertices[v1*3], vertices[v1*3 + 1], vertices[v1*3 + 2]);
            glm::vec3 bottomRight = glm::vec3(vertices[v2*3], vertices[v2*3 + 1], vertices[v2*3 + 2]);
            glm::vec3 center = glm::vec3(0, 0, 0);

            ShapeComponent sc = ShapeComponent(bottomLeft, center, bottomRight, center, complexity, 1, std::vector<glm::vec3>{normal, normal, normal, normal}, rotationMatrix, translationMatrix);
            triangles.insert(triangles.end(), sc.triangles.begin(), sc.triangles.end());
    }
}

/**
 * @brief Constructs outer vertices.
 */
void CylinderComponent::makeVertices() {
    float divideFactor = 1/static_cast<float>(sides);
    for (int i = 0; i <= sides; i++) {
        vertices.push_back(m_radius*cos(i * divideFactor*2*M_PI));
        vertices.push_back(m_radius*sin(i * divideFactor*2*M_PI));
        vertices.push_back(m_radius*0);

        if (m_shape == CYLINDER) {
            sideNormals.push_back(glm::vec3(cos(i*divideFactor*2*M_PI), 0, sin(i*divideFactor*2*M_PI)));
        }
        else if (m_shape == CONE) {
            sideNormals.push_back(glm::vec3(cos(i*divideFactor*2*M_PI), .477, sin(i*divideFactor*2*M_PI)));
        }
        else if (m_shape == SPHERE) {
            float x = sin(m_phi)*cos(i*divideFactor*2*M_PI);
            float z = cos(m_phi);
            float y = sin(m_phi)*sin(i*divideFactor*2*M_PI);
            sideNormals.push_back(glm::vec3(x, y, z));
        }
        else {
             sideNormals.push_back(glm::vec3(cos(i*divideFactor*2*M_PI), 0, sin(i*divideFactor*2*M_PI)));
        }
    }
}

