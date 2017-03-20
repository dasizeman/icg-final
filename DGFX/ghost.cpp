#include "ghost.hpp"
#include "burst.hpp"
namespace dgfx {
    Ghost::Ghost(float x, float y, float z, float xrot, float yrot, float zrot) : Cube(x, y, z, xrot, yrot, zrot, 1.0){
    }

    std::vector<std::string> Ghost::getTexturePaths() {
        std::vector<std::string> textures;

        textures.push_back("textures/white.ppm");
        textures.push_back("textures/white.ppm");
        textures.push_back("textures/left-arm.ppm");
        textures.push_back("textures/right-arm.ppm");
        textures.push_back("textures/ghost-face.ppm");
        textures.push_back("textures/ghost-butt.ppm");

        return textures;
    }

    void Ghost::wasPicked( uint16_t triangleIdx ) {
        std::cout << "A cube was picked, triangle " << triangleIdx << std::endl; 
        
        // Create a burst at our location
        m_scene->addEntity(std::unique_ptr<Entity>(new Burst(m_x, m_y, m_z)));
        // kys
        m_alive = false;
    }
}
