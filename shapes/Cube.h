#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape
{
public:
    Cube(int shapeParameter1);
    ~Cube();

    void updateParameters(int shapeParameter1, int shapeParameter2, float shapeParameter3) override;
protected:
    void generateVertices() override;

private:
    int m_shapeParameter1;
};

#endif // CUBE_H
