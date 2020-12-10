#ifndef CONE_H
#define CONE_H

#include "CircularBaseShape.h"

class Cone : public CircularBaseShape
{
public:
    Cone(int shapeParameter1, int shapeParameter2);
    ~Cone();

protected:
    void generateVertices() override;
};

#endif // CONE_H
