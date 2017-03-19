#include "cube.hpp"
#include "burst.hpp"
#include "dice_roller_scene.hpp"
namespace dgfx {
    Cube::Cube(float x, float y, float z, float xrot, float yrot, float zrot, std::vector<std::string> text) : Object(x,y,z, xrot, yrot, zrot){
      textures = text;
    }

    void Cube::generateGeometry() {
  	vec4 vertices[] = {vec4(-0.5,-0.5,0.5,1.0),vec4(-0.5,0.5,0.5,1.0),vec4(0.5,0.5,0.5,1.0),vec4(0.5,-0.5,0.5,1.0), vec4(-0.5,-0.5,-0.5,1.0),vec4(-0.5,0.5,-0.5,1.0),vec4(0.5,0.5,-0.5,1.0),vec4(0.5,-0.5,-0.5,1.0)};

         makeQuad(1,0,3,2, vertices);  //front
         makeQuad(2,3,7,6, vertices);  //right
         makeQuad(3,0,4,7, vertices);  //bottom
         makeQuad(6,5,1,2, vertices);  //top
         makeQuad(4,5,6,7, vertices);  //back
         makeQuad(5,4,0,1, vertices);  //left

    }


    void Cube::makeQuad(GLubyte a, GLubyte b, GLubyte c, GLubyte d, vec4 *vertices){
      //Triangle #1
      m_vertices.push_back(vertices[a]);
      m_vertices.push_back(vertices[b]);
      m_vertices.push_back(vertices[c]);

      //Triangle #2
      m_vertices.push_back(vertices[a]);
      m_vertices.push_back(vertices[c]);
      m_vertices.push_back(vertices[d]);

    }

    void Cube::textureInit() {
        m_textureHandles.resize(1);
        glGenTextures( 1, &m_textureHandles[0] );
        int width = 512, height = 512;

        glBindTexture( GL_TEXTURE_2D, m_textureHandles[0] );
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        GLubyte *front, *back, *left, *right, *top, *bottom;
        top = ppmRead(textures[0].c_str(), &width, &height);
        glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0 , GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, top);
        bottom = ppmRead(textures[1].c_str(), &width, &height);
        glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0 , GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bottom);
        right = ppmRead(textures[2].c_str(), &width, &height);
        glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0 , GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, right);
        left = ppmRead(textures[3].c_str(), &width, &height);
        glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0 , GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, left);
        front = ppmRead(textures[4].c_str(), &width, &height);
        glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0 , GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, front);
        back = ppmRead(textures[5].c_str(), &width, &height);
        glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0 , GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, back);

        delete[] front;
        delete[] back;
        delete[] left;
        delete[] right;
        delete[] top;
        delete[] bottom;
    }

    void Cube::textureDraw() {
      glEnable(GL_TEXTURE_CUBE_MAP);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureHandles[0]);
      glUniform1i(glGetUniformLocation(m_activeShader, "cubeMap"), 0);
    }


    void Cube::setShader( std::map<std::string, GLuint>& shaderMap ) {
        m_activeShader = shaderMap[ DiceRollerScene::FRAGMENT_TEXTURE_SHADER_NAME ];
    }
    
    void Cube::wasPicked( uint16_t triangleIdx ) {
        std::cout << "A cube was picked, triangle " << triangleIdx << std::endl; 
        
        // Create a burst at our location
        /* m_scene->addEntity(std::unique_ptr<Entity>(new Burst(m_x, m_y, m_z))); */
        // kys
        m_alive = false;
    }

}
