#ifndef __ROOM_H__
#define __ROOM_H__
#include "cube.hpp"
namespace dgfx {
class Room : public Cube {
public:
    Room (float x, float y, float z, float xrot, float yrot, float zrot);

protected:
    void wasPicked( uint16_t triangleIdx );
    std::vector<std::string> getTexturePaths();
};
}
#endif
