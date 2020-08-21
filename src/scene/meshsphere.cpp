#include "meshsphere.h"

#include <glm.hpp>
#include <math.h>

#define _USE_MATH_DEFINES

namespace scene {
    //TODO remove redundant verticies at poles (z axis)
    MeshSphere::MeshSphere(unsigned int parallels, unsigned int meridians)
    {
        meridians = meridians -1;

        //radius 1
        for(unsigned int o = 0; o <= meridians; o++) {
            float meridian =  M_PI * (float)o / (float)meridians;
            for(unsigned int i = 0; i < parallels; i++) {
                float parallel = 2.0 * M_PI * i / parallels;

                float xc = glm::sin(meridian) * glm::cos(parallel);
                float yc = glm::sin(meridian) * glm::sin(parallel);
                float zc = glm::cos(meridian);

                mesh.AddVertex({xc, yc, zc, xc, yc, zc});
            }
        }

        for(unsigned int o = 1; o <= meridians; o++) {
            for(unsigned int i = 0; i < parallels; i++) {
                unsigned int higher = i + (o-1) * parallels;
                unsigned int current = i + o * parallels;
                unsigned int current1 = ((i+1)%parallels) + o * parallels;

                unsigned int higher1 = ((i+1)%parallels) + (o-1) * parallels;

                mesh.AddIndices({higher, current, current1,
                                 higher, higher1, current1});
            }
        }

        //std::cout << mesh.LenIndices() << " " << mesh.LenVertices() << std::endl;
    }
}
