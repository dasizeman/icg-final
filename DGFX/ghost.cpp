#include "ghost.hpp"
namespace dgfx {
    Ghost::Ghost(float x, float y, float z, float xrot, float yrot, float zrot) : Cube(x, y, z, xrot, yrot, zrot, 0.5){}

    void Ghost::rotateTowardCamera(){
      vec4 eye = m_scene->m_activeCamera->m_eye;
      float dx = m_x-cam.m_eye.x;
      float dy = m_y-cam.m_eye.y;
      float dz = m_z-cam.m_eye.z;

      vec2 dxy = vec2(dx, dy);
      vec2 dyz = vec2(dy, dz);
      vec2 dzx = vec2(dz, dx);

      vec3 norm = m_normals[0];
      vec2 norm_xy = vec2(norm.x, norm.y);
      vec2 norm_yz = vec2(norm.y, norm.z);
      vec2 norm_zx = vec2(norm.z, norm.x);

      m_xRot = getAngle(dyz, norm_yz, m_xRot);
      m_yRot = getAngle(dzx, norm_zx, m_yRot);
      m_zRot = getAngle(dxy, norm_xy, m_zRot);
    }

    float Ghost::getAngle(vec2 delta, vec2 norm, float angle){
      float x = (norm.x * cos(angle)) - (norm.y * sin(angle));
      float y = (norm.x * sin(angle)) + (norm.y * cos(angle));
      vec2 normal = normalize(vec2(x,y));
      delta = normalize(delta);
      vec2 absNormal = vec2(abs(normal.x), abs(normal.y));
      vec2 absDelta = vec2(abs(delta.x), abs(delta.y));

      return acos(dot(normal, delta)/dot(normal,delta));
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
