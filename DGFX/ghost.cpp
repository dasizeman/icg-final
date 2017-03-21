#include "ghost.hpp"
namespace dgfx {
    Ghost::Ghost(float x, float y, float z, float xrot, float yrot, float zrot) : Cube(x, y, z, xrot, yrot, zrot, 0.5){}

    void Ghost::rotateTowardCamera(){
      vec4 eye = m_scene->m_activeCamera->m_eye;
      float dx = eye.x - m_x;
      float dy = eye.y - m_y;
      float dz = eye.z - m_z;

      vec2 dxy = vec2(dx, dy);
      vec2 dyz = vec2(dy, dz);
      vec2 dxz = vec2(dx, dz);

      vec3 norm = m_normals[0];
      vec2 norm_xy = vec2(norm.x, norm.y);
      vec2 norm_yz = vec2(norm.y, norm.z);
      vec2 norm_xz = vec2(norm.x, norm.z);

      m_yRot = getAngle(dxz, norm_xz);
      
      // next step:  update normals
      
      /* m_xRot = -1*getAngle(dyz, norm_yz); */

      // next step: solve for angle that makes the normal for top face become (0,1,0)

      //m_zRot = getAngle(dxy, norm_xy);
      std::cout << "z_rot: " << m_zRot << "\n";
      std::cout << "normx: " << norm.x << " normy: " << norm.y << "\n";
      std::cout << "dx: " << dxy.x << " dy: " << dxy.y << "\n\n";
    }

    float Ghost::getAngle(vec2 delta, vec2 norm){
      float magX = sqrt((delta.x*delta.x) + (delta.y*delta.y));
      float magY = sqrt((norm.x*norm.x) + (norm.y*norm.y));
      float magnitude = magX*magY;
      float dotProduct = dot(norm, delta);
      /* std::cout << "magnitude: " << magnitude << " dotProduct: " << dotProduct << "\n"; */

      if(delta.x < 0) 
        return (360-(acos(dotProduct/magnitude) * 180)/M_PI);
      else
        return ((acos(dotProduct/magnitude) * 180)/M_PI);
    }
    
    void Ghost::update(std::map<std::string, GLuint>& shaderMap) { 
        rotateTowardCamera();
        // Generate the model matrix.  We also need to apply it to our in-memory
        // vertex data for the sake of collision detection
        // TODO WARNING!! This is not the right way to do this, as if we ever
        // copy the vertex data to the gpu this transformation will stick!!
        mat4 modelMatrix = Translate(m_x, m_y, m_z ) * 
                           RotateY( 1 ) *
                           Translate( -m_x, -m_y, -m_z );
        for ( int i = 0; i < m_vertices.size(); i++ ) {
            vec4 oldVertex = m_vertices[ i ];
            m_vertices[ i ] = modelMatrix * oldVertex;
        }
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
