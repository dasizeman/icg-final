#include <sstream>
#include <cassert>

#include "scene.hpp"
#include "daveutils.hpp"

namespace dgfx {

    const std::string Scene::DEFAULT_WINDOW_NAME = "CS432";
    const std::string Scene::SHADER_PATH = "shaders/";
    const int Scene::DEFAULT_WINDOW_WIDTH = 500, Scene::DEFAULT_WINDOW_HEIGHT = 500;

    std::shared_ptr<Scene> Scene::m_instance = nullptr;
    

    Scene::Scene() : 
        Scene::Scene( DEFAULT_WINDOW_NAME, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT){}
    
    Scene::Scene(std::string windowName, int width, int height) : 
        m_screenWidth( width ), m_screenHeight( height ){
        init( windowName, width, height );
    }

    void Scene::init(std::string windowName, int width, int height) {
#ifdef __APPLE__
        glutInitDisplayMode( GLUT_3_2_CORE_PROFILE|GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
        glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
        glutInitWindowSize( width, height );

        glutCreateWindow( windowName.c_str() );
        glewInitAndVersion();


        // Assign event callbacks
        glutDisplayFunc( &Scene::display_callback_wrapper );
        glutMouseFunc( &Scene::click_callback_wrapper );
        glutKeyboardFunc( &Scene::keyboard_callback_wrapper );
        glutSpecialFunc( &Scene::special_key_wrapper );
        glutReshapeFunc( &Scene::reshape_callback_wrapper );
        glutWMCloseFunc( &Scene::close_handler );

        // 100 fps!
        glutTimerFunc( 1000, timer_callback_wrapper, 0);


        glEnable (GL_DEPTH_TEST);
        glClearColor( 0.0, 0.0, 0.0, 1.0 );

    }


    void Scene::addEntity(std::unique_ptr<Entity> entity) {
        entity->m_scene = Scene::m_instance;
        entity->setShader( m_shaderMap );
        entity->init( m_shaderMap );

        m_entities.push_back( std::move(entity) );

    }

    void Scene::addShader ( std::string shaderName ) {
        if ( m_shaderMap.find( shaderName ) != m_shaderMap.end() )
            return;

        std::stringstream ss;
        ss << SHADER_PATH << shaderName << ".vert";
        auto vertexShaderPath = ss.str();

        ss.str("");
        ss << SHADER_PATH << shaderName << ".frag";
        auto fragmentShaderPath = ss.str();

        GLuint shader = InitShader( vertexShaderPath.c_str(), fragmentShaderPath.c_str() );
        std::cout << "Initialized shader ID " << shader << std::endl;
        m_shaderMap[ shaderName ] = shader;
    }
    
    void Scene::start() {
        assert( m_instance != nullptr );
        glutMainLoop();
    }

    void Scene::globalAnimationToggle() {
         m_doAnimation = !m_doAnimation;
        for ( std::unique_ptr<Entity> &entity : m_entities )
            entity->m_doAnimation = m_doAnimation;
    }

    void Scene:: removeDead() {
        m_entities.erase( std::remove_if(
                    m_entities.begin(),
                    m_entities.end(),
                    [] (const std::unique_ptr<Entity> &entity) {
                        return !entity->m_alive; 
                    }),
                m_entities.end());
    }

    void Scene::keyboardHandler(unsigned char key, int x, int y) {
        switch( key ) {
        case 033:  // Escape key
        case 'q': case 'Q':
            exit( EXIT_SUCCESS );
            break;
        }

        for ( std::unique_ptr<Entity>& entity : m_entities )
            entity->keyboardHandler( key, x, y);

    }


    void Scene::specialKeyHandler(int key, int x, int y) {
        for (std::unique_ptr<Entity> &entity : m_entities )
            entity->specialKeyHandler( key, x, y);
    }

    void Scene::clickHandler(GLint button, GLint state, GLint x, GLint y) {
        for ( std::unique_ptr<Entity>& entity : m_entities )
            entity->clickHandler( button, state, x, y);
    }

    void Scene::displayCallback() {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        for ( std::unique_ptr<Entity>& entity : m_entities )
            entity->draw( m_shaderMap );

        glutSwapBuffers();
        //glFlush();
    }

    void Scene::timerCallback( int value ) {
        // Remove any dead things
        removeDead();
        for ( std::unique_ptr<Entity>& entity : m_entities )
            entity->update( m_shaderMap );
    }

    void Scene::reshapeCallback( int width, int height ){}


    void Scene::glewInitAndVersion(void)
    {
       fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
       fprintf(stdout, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

#ifndef __APPLE__
       GLenum err = glewInit();
       if (GLEW_OK != err)
       {
       fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
       }
       fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
    }


    // Static 
    void Scene::display_callback_wrapper() {
        Scene::m_instance->displayCallback();
    }

     void Scene::timer_callback_wrapper( int value ) {
        Scene::m_instance->timerCallback( value );
        glutTimerFunc( 1000/FPS, timer_callback_wrapper, 0 );
        glutPostRedisplay();
     }

     void Scene::reshape_callback_wrapper( int width, int height ) {
        Scene::m_instance->reshapeCallback( width, height );
     }

     void Scene::keyboard_callback_wrapper(unsigned char key, int x, int y) {
        Scene::m_instance->keyboardHandler( key, x, y );
     }

     void Scene::special_key_wrapper( int key, int x, int y ) {
        Scene::m_instance->specialKeyHandler( key, x, y );
     }

     void Scene::click_callback_wrapper(GLint button, GLint state, GLint x, GLint y) {
         Scene::m_instance->clickHandler( button, state, x, y );
     }

        void Scene::close_handler() {
    }
}
