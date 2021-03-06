#include <vector>
#include <memory>
#include <iostream>

#include "object.hpp"
#include "scene.hpp"
#include "dice_roller_scene.hpp"
#include "ghost.hpp"
#include "burst.hpp"
#include "room.hpp"
#include "die.hpp"

#include "Angel.h"
using namespace dgfx;

int main(int argc, char **argv) {
	glutInit( &argc, argv );

    //A scene encapsulates eveything the user sees and interacts with    
    std::shared_ptr<Scene> scene(new DiceRollerScene("Cool Sizer: Gotta Oops", 1000, 1000));

    // This is kind of hacky, but we do it so we can hide *most* of the glut
    // init and callback hooking inside the scene class
    Scene::m_instance = scene;

    
    for (int x = -4; x < 5; x++)
        scene->addEntity(std::unique_ptr<Entity>(new Ghost(x, 0.5, -4, 0, 0, 0)));
    scene->addEntity(std::unique_ptr<Entity>(new Die(0, 0.35, 2, 0, 0, 0)));
    scene->addEntity(std::unique_ptr<Entity>(new Room(0, 7.5, 0, 0, 0, 0)));


    scene->start();

    return 0;
}
