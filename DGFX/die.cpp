#include "die.hpp"
#include "daveutils.hpp"
#include "ghost.hpp"
namespace dgfx {
    Die::Die (float x, float y, float z, float xrot, float yrot, float zrot) :
        Cube (x,y,z,xrot,yrot,zrot, 0.5){
        
        setUpSide(1);
    }


    void Die::wasPicked( uint16_t triangleIdx ) {
        roll();
    }

    std::vector<std::string> Die::getTexturePaths() {
        std::vector<std::string> textures = {
            "textures/one.ppm",
            "textures/two.ppm",
            "textures/three.ppm",
            "textures/four.ppm",
            "textures/five.ppm",
            "textures/six.ppm"

        };

        return textures;
    }

    void Die::roll() {
        m_spinTimer = ROLL_FRAMES;
        m_lastRollValue = static_cast<int>(daveutils::randomFloat(1,7));
    }


    void Die::setUpSide(int side ) {
        switch ( side ) {
            case 1:
            m_xRot = 0;
            m_yRot = 0;
            m_zRot = 0;
            break;
            case 2:
            m_xRot = -180;
            m_yRot = 0;
            m_zRot = 0;

            break;
            case 3:
            m_xRot = 0;
            m_yRot = 0;
            m_zRot = 90;

            break;
            case 4:
            m_xRot = 0;
            m_yRot = 0;
            m_zRot = -90;

            break;
            case 5:
            m_xRot = -90;
            m_yRot = 0;
            m_zRot = 0;
            break;
            case 6:
            m_xRot = -270;
            m_yRot = 0;
            m_zRot = 0;

            break;
        }
    }

    void Die::update(std::map<std::string, GLuint>& shaderMap) { 
        if (m_spinTimer > 0) {
            if (m_spinTimer % 5 == 0) {
                m_xRot = daveutils::randomFloat(0, 360);
                m_yRot = daveutils::randomFloat(0, 360);
                m_zRot = daveutils::randomFloat(0, 360);

            }
            m_spinTimer--;
            return;
        }

        if (m_lastRollValue == 0)
            return;

        setUpSide(m_lastRollValue);
        std::cout << "You rolled a " << m_lastRollValue << "!" << std::endl;

        // TODO generate ghosts from m_lastRollValue
        for (int i = 0; i < m_lastRollValue; i++) {
            float x = daveutils::randomFloat(-7.5, 7.5);
            float y = daveutils::randomFloat(-7.5, 7.5);
            float z = daveutils::randomFloat(-7.5, 7.5);
            m_scene->addEntity(std::unique_ptr<Entity>(new Ghost(x, y, z, 0 , 0 , 0)));
        }
        
        m_lastRollValue = 0;

    }
}
