#ifndef CIRCULARBASESHAPE_H
#define CIRCULARBASESHAPE_H

#include "Shape.h"

class CircularBaseShape : public Shape
{
public:
    CircularBaseShape(int shapeParameter1, int shapeParameter2);
    virtual ~CircularBaseShape();

    void updateParameters(int shapeParameter1, int shapeParameter2, float shapeParameter3) override;

protected:
    void generateCircularBaseSlice(std::vector<glm::vec4> &v);

    int m_shapeParameter1;
    int m_shapeParameter2;
};

#endif // CIRCULARBASESHAPE_H
