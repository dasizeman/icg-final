#include "burst.hpp"
#include "dice_roller_scene.hpp"
#include "daveutils.hpp"
namespace dgfx {
    Burst::Burst(float x, float y, float z) : Object(x,y,z, 0, 0, 0){
        initializeParticles();
        m_gravityPull.w = 1.0;
        m_aliveTicks = 100;
    }

    void Burst::generateGeometry() {
        for (int i = 0; i < 300; i++ )
            m_vertices.push_back( vec4( 0, 0, 0, 1.0 ) );

        // We will set every texture coord to zero since we will be using single
        // color "textures"
        for (int i = 0; i < m_vertices.size(); i++ )
            m_textureCoords.push_back( vec2(0,0) );

    }


    void Burst::makeQuad(GLubyte a, GLubyte b, GLubyte c, GLubyte d, vec4 *vertices){
      //Triangle #1
      m_vertices.push_back(vertices[a]);
      m_vertices.push_back(vertices[b]);
      m_vertices.push_back(vertices[c]);

      //Triangle #2
      m_vertices.push_back(vertices[a]);
      m_vertices.push_back(vertices[c]);
      m_vertices.push_back(vertices[d]);

    }

    void Burst::initializeParticles() {
        int numSteps = 25;
        float stepSize = 2*M_PI / numSteps;

        for (float phi = 0; phi < M_PI; phi += stepSize)
            for ( float theta = 0; theta < 2 * M_PI; theta += stepSize ) {
                float x = sin(phi) * cos(theta);
                float y = sin(phi) * sin(theta);
                float z = cos(phi);

                vec3 direction3 = normalize(vec3(x,y,z));

                // Using vec4 as convenience when modifying world position
                vec4 direction4 ( direction3.x,
                                  direction3.y,
                                  direction3.z,
                                  1.0 );
                m_directionVectors.push_back( direction4 );

            }

        // Start all particles at the center point. This x,y,z is in model
        // coordinates
        for (int i = 0; i < m_directionVectors.size(); i++ )
            m_particlePositions.push_back( vec4( 0, 0, 0, 1.0) );
    }

    void Burst::draw(std::map<std::string, GLuint>& shaderMap) {
       glPointSize(10.0); 

        glUseProgram( m_activeShader );
        glBindVertexArray( m_vertexArrays[0] );

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

        // Set the model matrix and draw every point
        for (int i = 0; i < m_vertices.size(); i++) {
            vec4 pos = m_particlePositions[i];

            glUniformMatrix4fv(glGetUniformLocation( m_activeShader, "model_matrix" ),1, GL_TRUE, 
                     Translate(m_x, m_y, m_z) * Translate(pos.x, pos.y, pos.z));
            
            glDrawArrays( GL_POINTS, i, i+1 );

        }

    }


    void Burst::update(std::map<std::string, GLuint>& shaderMap) {
        float velocity = 3.0 / 200;
        vec4 g(0, -0.0001, 0, 1);
        m_gravityPull += g;

        // Update the positions of all particles
        for (int i = 0; i < m_particlePositions.size(); i++ )
            m_particlePositions[i] = m_particlePositions[i] + (velocity * m_directionVectors[i] ) + m_gravityPull;

        m_aliveTicks--;
        if ( m_aliveTicks <= 0)
            m_alive = false;

    }

    void Burst::textureInit() {

        GLubyte color[3];
        for (int i = 0; i < 3; i++)
            color[i] = static_cast<GLubyte>(daveutils::randomFloat(0,256));
        GLubyte *image = color;

        m_textureHandles.resize(1);
        glGenTextures( 1, &m_textureHandles[0] );
        int width = 1, height = 1;

        glBindTexture( GL_TEXTURE_CUBE_MAP, m_textureHandles[0] );
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);

        glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0 , GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0 , GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0 , GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0 , GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0 , GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0 , GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);


    }

    void Burst::textureDraw() {

      glEnable(GL_TEXTURE_CUBE_MAP);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureHandles[0]);
      glUniform1i(glGetUniformLocation(m_activeShader, "cubeMap"), 0);

    }


    void Burst::setShader( std::map<std::string, GLuint>& shaderMap ) {
        m_activeShader = shaderMap[ DiceRollerScene::FRAGMENT_TEXTURE_SHADER_NAME ];
    }

}
