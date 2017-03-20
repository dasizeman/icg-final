#ifndef __GHOST_H__
#define __GHOST_H__
#include "cube.hpp"

namespace dgfx {
  class Ghost : public Cube {
    public:
      Ghost(float x, float y, float z, float xrot, float yrot, float zrot);

    protected:
        std::vector<std::string> getTexturePaths();
        void wasPicked( uint16_t triangleIdx );
  };
}
#endif
