#ifndef CYLINDER_H
#define CYLINDER_H

#include "CircularBaseShape.h"

class Cylinder : public CircularBaseShape
{
public:
    Cylinder(int shapeParameter1, int shapeParameter2);
    ~Cylinder();

protected:
    void generateVertices() override;
};

#endif // CYLINDER_H
