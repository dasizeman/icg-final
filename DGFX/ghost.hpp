#ifndef __GHOST_H__
#define __GHOST_H__
#include "cube.hpp"

namespace dgfx {
  class Ghost : public Cube {
    public:
      Ghost(float x, float y, float z, float xrot, float yrot, float zrot);
      void moveTowardCamera();
      void rotateTowardCamera();
    protected:
      std::vector<std::string> getTexturePaths();
      void wasPicked( uint16_t triangleIdx );
      void update(std::map<std::string, GLuint>& shaderMap);
      void draw(std::map<std::string, GLuint>& shaderMap);
    private:
      float rotation_speed;
      float movement_speed;
  };
}
#endif
