#include "ghost.hpp"
namespace dgfx {
    Ghost::Ghost(float x, float y, float z, float xrot, float yrot, float zrot, std::vector<std::string> text) : Cube(x, y, z, xrot, yrot, zrot, text){
      textures.push_back("textures/white.ppm");
      textures.push_back("textures/white.ppm");
      textures.push_back("textures/left-arm.ppm");
      textures.push_back("textures/right-arm.ppm");
      textures.push_back("textures/ghost-face.ppm");
      textures.push_back("textures/ghost-butt.ppm");
    }
}
