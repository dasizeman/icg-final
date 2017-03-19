#include <vector>
#include <memory>
#include <iostream>

#include "object.hpp"
#include "scene.hpp"
#include "dice_roller_scene.hpp"
#include "cube.hpp"
#include "burst.hpp"
#include "plane.hpp"
#include "Angel.h"
using namespace dgfx;

int main(int argc, char **argv) {
	glutInit( &argc, argv );

    //A scene encapsulates eveything the user sees and interacts with    
    std::shared_ptr<Scene> scene(new DiceRollerScene("Cool Sizer: Gotta Oops", 1000, 1000));

    // This is kind of hacky, but we do it so we can hide *most* of the glut
    // init and callback hooking inside the scene class
    Scene::m_instance = scene;

    //scene->addEntity(std::unique_ptr<Entity>(new Plane(0, 0, 0, 0,0, 0, 100)));
    /* for (float y = 0.5; y < 5.5; y++) */
    /*    for (float x = -2; x <= 2; x++) */
    
    std::vector<std::string> textures;
    /* for(int i = 0; i < 6; i++){ */
    /*   textures.push_back("textures/crate_texture.ppm"); */
    /* } */
    textures.push_back("textures/sky-top.ppm");
    textures.push_back("textures/sky-bottom.ppm");
    textures.push_back("textures/sky-left.ppm");
    textures.push_back("textures/sky-left.ppm");
    textures.push_back("textures/sky-front.ppm");
    textures.push_back("textures/sky-back.ppm");
    scene->addEntity(std::unique_ptr<Entity>(new Cube(-2, 0.5, -4, 0, 0, 0, textures)));


    scene->start();

    return 0;
}
