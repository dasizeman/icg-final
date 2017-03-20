#ifndef __BURST_H__
#define __BURST_H__

#include "object.hpp"
namespace dgfx {
class Burst : public Object {
    public:
        Burst(float x, float y, float z);
    protected:
        std::vector<vec4> m_directionVectors;
        std::vector<vec4> m_particlePositions;
        vec4 m_gravityPull;

        int m_aliveTicks;

        void makeQuad(GLubyte a, GLubyte b, GLubyte c, GLubyte d, vec4 *vertices);
        void draw(std::map<std::string, GLuint>& shaderMap);
        void update(std::map<std::string, GLuint>& shaderMap);
        virtual void generateGeometry();
        virtual void textureInit();
        virtual void textureDraw();
        virtual void setShader( std::map<std::string, GLuint>& shaderMap );

        void initializeParticles();
};
}
#endif
