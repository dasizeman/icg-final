#include "camera.hpp"
#include "glm/gtx/rotate_vector.hpp"

namespace dgfx {
    Camera::Camera( uint16_t id,
                    vec4 eye,
                    vec4 at,
                    vec4 up)  :
                    m_id( id ),
                    m_eye( eye ),
                    m_n( -at ),
                    m_v( up ),
                    m_width(500),
                    m_height(500){
        m_v.w = 0;
        m_v = normalize(m_v);
        m_n.w = 0;
        m_n = normalize(m_n);
        m_u = normalize(cross(m_v,m_n));
        m_u.w = 0;

        m_moveForward = glm::vec3(0,0,-1);
        m_moveStrafe = glm::vec3(1,0,0);
        updateViewMatrix();
    }

    void Camera::toggleProjectionMode() {
        // TODO maybe don't hardcode this
        //m_usePerspectiveProjection = !m_usePerspectiveProjection;
        //iif ( m_usePerspectiveProjection )
            usePerspectiveProjection( 65, m_aspect, 1, 100 );
        //else
           // useOrthographicProjection( -1, 1, -1, 1, 1, 3 );
    
    }
    void Camera::usePerspectiveProjection( float fovy, float aspect, float znear, float zfar ) {
        m_projectionMatrix = Perspective( fovy, aspect, znear, zfar );
        m_fov = fovy;
        m_aspect = aspect;
        m_near = znear;
        m_far = zfar;

    }
    void Camera::useOrthographicProjection( float left, float right, float bottom, float top, float near, float far ) {
        m_projectionMatrix = Ortho( left, right, bottom, top, near, far );
    }


    void Camera::updateViewMatrix() {
        m_viewMatrix = LookAt( m_eye, m_eye- m_n, m_v );
    }

    void Camera::changeProjectionAspectRatio(double width, double height) {
        m_width = width;
        m_height = height;
        m_aspect = static_cast<double>(m_width) / static_cast<double>(m_height);
        toggleProjectionMode(); // Hack, to update projection matrix
    }


    void Camera::moveAlongAt( float amount ) {
        // This is moving along -n
        m_eye -= amount * normalize(m_n);
        updateViewMatrix();
    }
    void Camera::moveAlongU( float amount ) {
        m_eye += amount * normalize(m_u);
        updateViewMatrix();

    }

    void Camera::moveForward( float amount ) {
        vec4 direction ( m_moveForward.x,
                        m_moveForward.y,
                        m_moveForward.z,
                        0 );
        m_eye += amount * direction;
        updateViewMatrix();
    }

    void Camera::moveStrafe( float amount ) {
        vec4 direction ( m_moveStrafe.x,
                        m_moveStrafe.y,
                        m_moveStrafe.z,
                        0 );
        m_eye += amount * direction;
        updateViewMatrix();

    }
    void Camera::pitch( float amount ) {
        vec4 old_v = m_v;
        m_v = normalize(cos(amount)*old_v - sin(amount)*m_n);
        m_n = normalize(sin(amount)*old_v + cos(amount)*m_n);
        m_u = normalize(cross(m_v,m_n)) ;
        m_u.w = 0;
        updateViewMatrix();
    }
    void Camera::roll( float amount ) {
        vec4 old_v = m_v;
        m_v = normalize(cos(amount)*old_v - sin(amount)*m_u);
        m_u = normalize(sin(amount)*old_v + cos(amount)*m_u);
        m_n = normalize(cross(m_u, m_v));
        m_n.w = 0;
        updateViewMatrix();

    }
    void Camera::yaw( float amount ) {
        vec4 old_u = m_u;
        m_u = normalize(cos(amount)*old_u - sin(amount)*m_n);
        m_n = normalize(sin(amount)*old_u + cos(amount)*m_n);
        m_v = normalize(cross(m_n, m_u));
        m_v.w = 0;
        updateViewMatrix();
    }

    void Camera::rotateInX( float amount ) {
        glm::vec3 y(0, 1, 0);
        glm::vec3 n(m_n.x, m_n.y, m_n.z);
        glm::vec3 u(m_u.x, m_u.y, m_u.z);
        glm::vec3 v(m_v.x, m_v.y, m_v.z);

        // Reset to eye-line before turning left or right
        m_u = normalize(vec4(m_moveStrafe.x, m_moveStrafe.y, m_moveStrafe.z,0));
        m_n = -normalize(vec4(m_moveForward.x, m_moveForward.y, m_moveForward.z,0));
        m_v = vec4(0,1,0,0);

        // We want to rotate n, u, around v
        n = glm::rotate(n, amount, y);
        u = glm::rotate(u, amount, y);

        // And move, strafe around y
        m_moveForward = glm::normalize(
                glm::rotate(m_moveForward, amount, y));
        m_moveStrafe = glm::normalize(
                glm::rotate(m_moveStrafe, amount, y));



        m_u = normalize(vec4(u.x, u.y, u.z, 0));
        m_n = normalize(vec4(n.x, n.y, n.z, 0));
        updateViewMatrix();
    }

    void Camera::rotateInY( float amount ){
        glm::vec3 v(m_v.x, m_v.y, m_v.z);
        glm::vec3 n(m_n.x, m_n.y, m_n.z);

        // We want to rotate n, u, around strafe
        n = glm::rotate(n, amount, m_moveStrafe);
        v = glm::rotate(v, amount, m_moveStrafe);


        m_v = normalize(vec4(v.x, v.y, v.z, 0));
        m_n = normalize(vec4(n.x, n.y, n.z, 0));
        updateViewMatrix();
    
    }


    mat4 Camera::viewInverse() {
        return mat4 (
            vec4( m_u.x, m_v.x, m_n.x, m_eye.x ),
            vec4( m_u.y, m_v.y, m_n.y, m_eye.y ),
            vec4( m_u.z, m_v.z, m_n.z, m_eye.z ),
            vec4( 0, 0, 0, 1 ) );
    }
}
