#ifndef __DIE_H__
#define __DIE_H__
#include "cube.hpp"
namespace dgfx {
class Die : public Cube {
public:
    Die (float x, float y, float z, float xrot, float yrot, float zrot);

protected:
    const int ROLL_FRAMES = 100;

    int m_spinTimer = 0;
    int m_lastRollValue = 0;
    void wasPicked( uint16_t triangleIdx );
    std::vector<std::string> getTexturePaths();

    void setUpSide(int side);
    void roll();
    void update(std::map<std::string, GLuint>& shaderMap);
};
}
#endif
