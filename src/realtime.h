#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
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

    void generateCubeVBOsVAOs();
    void generateSphereVBOsVAOs();
    void generateCylinderVBOsVAOs();
    void generateConeVBOsVAOs();

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    // Tick Related Variables
    int m_timer;                                        // Stores timer which attempts to run ~60 times per second
    QElapsedTimer m_elapsedTimer;                       // Stores timer which keeps track of actual time between frames

    // Input Related Variables
    bool m_mouseDown = false;                           // Stores state of left mouse button
    glm::vec2 m_prev_mouse_pos;                         // Stores mouse position
    std::unordered_map<Qt::Key, bool> m_keyMap;         // Stores whether keys are pressed or not

    // Device Correction Variables
    int m_devicePixelRatio;

    GLuint m_shader;       //Stores id for shader program

    GLuint cube_vbo;       //Stores id for cube vbo
    GLuint cube_vao;       //Stores id for cube vao
    GLuint sphere_vbo;     //Stores id for sphere vbo
    GLuint sphere_vao;     //Stores id for sphere vao
    GLuint cylinder_vbo;   //Stores id for cylinder vbo
    GLuint cylinder_vao;   //Stores id for cylinder vao
    GLuint cone_vbo;       //Stores id for cone vbo
    GLuint cone_vao;       //Stores id for cone vao
};
