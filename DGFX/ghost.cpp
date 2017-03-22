#include "ghost.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace dgfx {
    Ghost::Ghost(float x, float y, float z, float xrot, float yrot, float zrot) : Cube(x, y, z, xrot, yrot, zrot, 0.5){
      rotation_speed = 0.2;
      movement_speed = 0.01;
    }

    void Ghost::rotateTowardCamera(){
      vec4 eye = m_scene->m_activeCamera->m_eye;
      vec3 norm = m_normals[0];
      glm::vec3 hyp = glm::normalize(glm::vec3(eye.x-m_x, 0, eye.z-m_z));
      glm::vec3 adj = glm::normalize(glm::vec3(norm.x, 0, norm.z));
        
      //get angle
      float y_angle = (glm::angle(hyp, adj) * 180)/ M_PI;
      if(hyp.x < 0)
        y_angle = 360 - y_angle;
      
      hyp = glm::normalize(glm::vec3(eye.x-m_x, eye.y-m_y, eye.z-m_z));
      adj = glm::normalize(glm::vec3(eye.x-m_x, 0, eye.z-m_z));
      float x_angle = (glm::angle(hyp, adj) * 180)/ M_PI;
      
      if(hyp.y > 0)
        x_angle = 360 - x_angle;
      
      //slow turning
      float ydiff = m_yRot - y_angle;

      if(fabs(ydiff) > rotation_speed){
        if(fabs(ydiff) > 180){
          ydiff = ydiff - (180*(ydiff/fabs(ydiff)));
          m_yRot += rotation_speed*ydiff/fabs(ydiff);
        }
        else{
          m_yRot -= rotation_speed*ydiff/fabs(ydiff);
        }
        if(m_yRot > 360)
          m_yRot -= 360;
        else if(m_yRot < 0)
          m_yRot += 360;
      }
      else{
        m_yRot = y_angle;
      }

      float xdiff = m_xRot - x_angle;

      if(fabs(xdiff) > rotation_speed){
        if(fabs(xdiff) > 180){
          ydiff = xdiff - (180*(xdiff/fabs(xdiff)));
          m_yRot += rotation_speed*ydiff/fabs(xdiff);
        }
        else{
          m_xRot -= rotation_speed*xdiff/fabs(xdiff);
        }
        if(m_xRot > 360)
          m_xRot -= 360;
        else if(m_yRot < 0)
          m_xRot += 360;
      }
      else{
        m_xRot = x_angle;
      }
    }

    void Ghost::moveTowardCamera(){
      glm::vec3 norm = glm::vec3(m_normals[0].x, m_normals[0].y, m_normals[0].z);
      float x_angle = (M_PI * m_xRot) / 180;
      float y_angle = (M_PI * m_yRot) / 180;
      norm = glm::rotateX(norm, x_angle);
      norm = glm::rotateY(norm, y_angle);
      std::cout << "x: " << norm.x << " y: " << norm.y << " z: " << norm.z << std::endl;
      std::cout << "xrot: " << m_xRot << " yrot: " << m_yRot << std::endl << std::endl;
      m_x += movement_speed*norm.x;
      m_y += movement_speed*norm.y;
      m_z += movement_speed*norm.z;
    }

    void Ghost::update(std::map<std::string, GLuint>& shaderMap){
        rotateTowardCamera();
        moveTowardCamera();
    }
    
    void Ghost::draw(std::map<std::string, GLuint>& shaderMap) {
        mat4 modelMatrix = Translate( m_x, m_y, m_z ) * RotateY(m_yRot) * RotateX( m_xRot ) * RotateZ( m_zRot);

        // Making sure vertices are also updated on the cpu
        for (int i = 0; i < m_vertices.size(); i++ )
            m_vertices[i] = modelMatrix * m_originalVertices[i];

        glUseProgram( m_activeShader );
        glBindVertexArray( m_vertexArrays[0] );

        // Set model matrix uniform
        GLuint mainModelMatrix = glGetUniformLocation( m_activeShader, "model_matrix" );
        glUniformMatrix4fv(mainModelMatrix,1, GL_TRUE, modelMatrix);

        // Set material property uniforms
        GLuint shaderLoc = glGetUniformLocation( m_activeShader, "AmbientMaterial" );
        glUniform4fv( shaderLoc, 1, m_ambient );
        shaderLoc = glGetUniformLocation( m_activeShader, "SpecularMaterial" );
        glUniform4fv( shaderLoc, 1, m_specular );
        shaderLoc = glGetUniformLocation( m_activeShader, "DiffuseMaterial" );
        glUniform4fv( shaderLoc, 1, m_diffuse );
        shaderLoc = glGetUniformLocation( m_activeShader, "Shininess" );
        glUniform1f( shaderLoc, m_shininess );

        glUniform1i( glGetUniformLocation( m_activeShader, "EnableLighting" ), 1 );

        // Texture-related draw commands
        textureDraw();

        glDrawArrays( GL_TRIANGLES, 0, m_vertices.size() );

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
