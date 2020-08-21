#ifndef MESHSPHERE_H
#define MESHSPHERE_H

#include "scene/object.h"

namespace scene {
    class MeshSphere : public Object
    {
    public:
        MeshSphere(unsigned int parallels, unsigned int meridians);
    };
}


#endif // MESHSPHERE_H
