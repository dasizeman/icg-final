#include "ghost.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace dgfx {
    Ghost::Ghost(float x, float y, float z, float xrot, float yrot, float zrot) : Cube(x, y, z, xrot, yrot, zrot, 0.5){}

    void Ghost::rotateTowardCamera(){
      vec4 eye = m_scene->m_activeCamera->m_eye;
      vec3 norm = m_normals[0];
      glm::vec3 glm_eye = glm::normalize(glm::vec3(eye.x - m_x, eye.y - m_y, eye.z - m_z));
      glm::vec3 glm_norm = glm::normalize(glm::vec3(norm.x, norm.y, norm.z));

      m_yRot = (glm::angle(glm::vec2(glm_eye.x, glm_eye.z), glm::vec2(glm_norm.x, glm_norm.z)) * 180) / M_PI;
      if(glm_eye.x < 0)
        m_yRot = 360 - m_yRot;

      glm::vec3 hyp = glm::vec3(eye.x - m_x, eye.y - m_y, eye.z - m_z);
      glm::vec3 adj = glm::vec3(hyp.x, 0, hyp.z);
      float dotProduct = glm::dot(hyp, adj);
      float hyp_dist = glm::length(hyp);
      float adj_dist = glm::length(adj);
      float angle = dotProduct/(adj_dist*hyp_dist);
      angle = acos(angle);
      angle = (angle*180)/M_PI;
      m_xRot = -1*angle;
    }
    
    void Ghost::draw(std::map<std::string, GLuint>& shaderMap) {
        vec4 eye = m_scene->m_activeCamera->m_eye;
        vec4 cam_up = m_scene->m_activeCamera->m_v;
        glm::vec3 cube_eye(m_x, m_y, m_z);
        glm::vec3 glm_eye = glm::normalize(glm::vec3(m_x - eye.x, m_y - eye.y, m_z - eye.z));
        vec3 norm = m_normals[0];
        glm::vec3 glm_norm = glm::normalize(glm::vec3(norm.x, norm.y, norm.z));
        glm::vec3 glm_cross = glm::cross(glm_eye, glm_norm);
        glm::vec3 up = glm::normalize(glm::vec3(cam_up.x, cam_up.y, cam_up.z));

        rotateTowardCamera();
        /* glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(m_x, m_y, m_z)) * glm::lookAt(cube_eye,cube_eye+glm_eye,up); */
        mat4 modelMatrix = Translate( m_x, m_y, m_z ) * RotateY(m_yRot) * RotateX( m_xRot ) * RotateZ( m_zRot);

        glUseProgram( m_activeShader );
        glBindVertexArray( m_vertexArrays[0] );

        // Set model matrix uniform
        GLuint mainModelMatrix = glGetUniformLocation( m_activeShader, "model_matrix" );
        /* glUniformMatrix4fv(mainModelMatrix,1, GL_FALSE, glm::value_ptr(modelMatrix)); */
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
