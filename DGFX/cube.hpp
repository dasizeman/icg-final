#ifndef __CUBE_H__
#define __CUBE_H__
#include "object.hpp"
#include "burst.hpp"

namespace dgfx {
class Cube : public Object {
    public:
        Cube(float x, float y, float z, float xrot, float yrot, float zrot, float scaleFactor);
        GLuint m_activeTextureHandle = 0;
    protected:
        float m_scale;
        std::vector<std::string> m_texturePaths;
        void makeQuad(GLubyte a, GLubyte b, GLubyte c, GLubyte d, vec4 *vertices);
        virtual void generateGeometry();
        virtual void textureInit();
        virtual void textureDraw();
        virtual void setShader( std::map<std::string, GLuint>& shaderMap );
        virtual std::vector<std::string> getTexturePaths() = 0;
        virtual void wasPicked( uint16_t triangleIdx ) = 0;
};
}
#endif
