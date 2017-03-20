#include "room.hpp"
namespace dgfx {
    Room::Room (float x, float y, float z, float xrot, float yrot, float zrot) : 
        Cube (x,y,z,xrot,yrot,zrot, 15.0){}


    void Room::wasPicked( uint16_t triangleIdx ){}


    std::vector<std::string> Room::getTexturePaths() {
        std::vector<std::string> textures;
        for (int i = 0; i < 6; i++)
            textures.push_back( "textures/brks.ppm" );

        return textures;
    }
}
