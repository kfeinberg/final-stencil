#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape
{
public:
    Sphere(int shapeParameter1, int shapeParameter2);
    ~Sphere();

    void updateParameters(int shapeParameter1, int shapeParameter2, float shapeParameter3) override;

protected:
    void generateVertices() override;

private:
    glm::vec3 calculateCoordinates(float phi, float theta, float r = 0.5f);
    void calculateAndStoreNormal(std::vector<glm::vec4> &v);

    int m_shapeParameter1;
    int m_shapeParameter2;
};

#endif // SPHERE_H
