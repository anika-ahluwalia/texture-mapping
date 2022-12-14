#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#include "glhelper.h"
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>

#include "camera/camera.h"

class Realtime : public QOpenGLWidget
{
public:
    Realtime(QWidget *parent = nullptr);
    void finish();                                      // Called on program exit
    void sceneChanged();
    void settingsChanged();

public slots:
    void tick(QTimerEvent* event);                      // Called once per tick of m_timer

protected:
    void initializeGL() override;                       // Called once at the start of the program
    void paintGL() override;                            // Called whenever the OpenGL context changes or by an update() request
    void resizeGL(int width, int height) override;      // Called when window size changes

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    void generateMatrices(SceneCameraData& cameraData);
    void makeFBO();
    void paintShapes();
    void paintTexture(GLuint texture);

    // Tick Related Variables
    int m_timer;                                        // Stores timer which attempts to run ~60 times per second
    QElapsedTimer m_elapsedTimer;                       // Stores timer which keeps track of actual time between frames

    // Input Related Variables
    bool m_mouseDown = false;                           // Stores state of left mouse button
    glm::vec2 m_prev_mouse_pos;                         // Stores mouse position
    std::unordered_map<Qt::Key, bool> m_keyMap;         // Stores whether keys are pressed or not

    // Device Correction Variables
    int m_devicePixelRatio;



    // helper class to store VAOs and VBOs
    GLHelper gl;

    bool is_intialized = false;

    RenderData metadata;
    GLuint m_shader;
    GLuint m_texture_shader;

    Camera camera;

    int param1 = 1;
    int param2 = 1;
    float nearPlane = 1;
    float farPlane = 1;

    // storing for extra credit
    bool adaptiveShapes = false;
    bool adaptiveDistance = false;
    glm::vec4 camera_pos;

    // action additions
    GLuint m_defaultFBO;
    GLuint m_fbo;
    GLuint m_fbo_texture;
    GLuint m_fbo_renderbuffer;

    GLuint m_fullscreen_vbo;
    GLuint m_fullscreen_vao;
    int m_fbo_width;
    int m_fbo_height;

    QImage m_image;
    GLuint m_brick_texture;
};
