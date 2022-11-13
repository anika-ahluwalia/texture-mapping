#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "settings.h"
#include <GL/glew.h>

#include "utils/scenedata.h"
#include "utils/sceneparser.h"
#include "shaderloader.h"
#include "debug.h"

// ================== Project 5: Lights, Camera

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;

    gl = GLHelper(settings.shapeParameter1, settings.shapeParameter2);

    // If you must use this function, do not edit anything above this
}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Students: anything requiring OpenGL calls when the program exits should be done here
    gl.cleanMemory();

    glDeleteProgram(m_shader);

    this->doneCurrent();
}

void Realtime::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();

    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    // Initializing GL.
    // GLEW (GL Extension Wrangler) provides access to OpenGL functions.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error while initializing GL: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Initialized GL: Version " << glewGetString(GLEW_VERSION) << std::endl;

    // Allows OpenGL to draw objects appropriately on top of one another
    glEnable(GL_DEPTH_TEST);
    // Tells OpenGL to only draw the front face
    glEnable(GL_CULL_FACE);
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here

    // clearing screen and loading shader
    glClearColor(0, 0, 0, 255);
    m_shader = ShaderLoader::createShaderProgram(":/Resources/Shaders/default.vert", ":/Resources/Shaders/default.frag");
    Debug::glErrorCheck();

    // load and bind VBOs and VAOs for each shape
    gl.generateAllShapes();

    Debug::glErrorCheck();
}

void Realtime::paintGL() {
    // Students: anything requiring OpenGL calls every frame should be done here

//    // Clear screen color and depth before painting
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    // Bind Sphere Vertex Data
//    glBindVertexArray(m_sphere_vao);

//    // Task 2: activate the shader program by calling glUseProgram with `m_shader`
//    glUseProgram(m_shader);

//    // Task 6: pass in m_model as a uniform into the shader program
//    glUniformMatrix4fv(glGetUniformLocation(m_shader, "modelMatrix"), 1, GL_FALSE, &m_model[0][0]);

//    // Task 7: pass in m_view and m_proj
//    glUniformMatrix4fv(glGetUniformLocation(m_shader, "viewMatrix"), 1, GL_FALSE, &m_view[0][0]);
//    glUniformMatrix4fv(glGetUniformLocation(m_shader, "projectionMatrix"), 1, GL_FALSE, &m_proj[0][0]);

//    // Task 12: pass m_ka into the fragment shader as a uniform
//    glUniform1f(glGetUniformLocation(m_shader, "ka"), m_ka);

//    // Task 13: pass light position and m_kd into the fragment shader as a uniform
//    glUniform1f(glGetUniformLocaon(m_shader, "kd"), m_kd);
//    glUniform4fv(glGetUniformLocation(m_shader, "lightPos"), 1, &m_lightPos[0]);

//    // Task 14: pass shininess, m_ks, and world-space camera position
//    glUniform1f(glGetUniformLocation(m_shader, "ks"), m_ks);
//    glUniform1f(glGetUniformLocation(m_shader, "shininess"), m_shininess);

//    glm::vec4 origin = {0.f, 0.f, 0.f, 1.f};
//    glm::vec4 camera_pos = glm::inverse(m_view) * origin;
//    glUniform4fv(glGetUniformLocation(m_shader, "worldSpaceCameraPos"), 1, &camera_pos[0]);

//    // Draw Command
//    glDrawArrays(GL_TRIANGLES, 0, m_sphereData.size() / 3);
//    // Unbind Vertex Array
//    glBindVertexArray(0);

//    // Task 3: deactivate the shader program by passing 0 into glUseProgram
//    glUseProgram(0);
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here
}

void Realtime::generateMatrices(SceneCameraData& cameraData) {
    Camera camera = Camera(cameraData, size().width(), size().height(), settings.nearPlane, settings.farPlane);

    view = camera.getViewMatrix();
    inverse_view = camera.getInverseViewMatrix();
    m_perspective = camera.getProjectionMatrix();
}

void Realtime::sceneChanged() {

    // loading in the scene data
    RenderData metaData;
    bool success = SceneParser::parse(settings.sceneFilePath, metaData);
    std::vector<RenderShapeData> shapes = metaData.shapes;

    generateMatrices(metaData.cameraData);

    for (int i = 0; i < shapes.size(); i++) {

        switch (shapes[i].primitive.type) {
            case PrimitiveType::PRIMITIVE_CUBE: {
                std::cout << "cube" << std::endl;
                break;
            }
            case PrimitiveType::PRIMITIVE_CONE: {
                std::cout << "cube" << std::endl;
                break;
            }
            case PrimitiveType::PRIMITIVE_SPHERE: {
                std::cout << "cube" << std::endl;
                break;
            }
            case PrimitiveType::PRIMITIVE_CYLINDER: {
                std::cout << "cube" << std::endl;
                break;
            }
        }
    }

    update(); // asks for a PaintGL() call to occur
}

void Realtime::settingsChanged() {

    gl.cleanMemory();
    gl = GLHelper(settings.shapeParameter1, settings.shapeParameter2);
    gl.generateAllShapes();

    Debug::glErrorCheck();

    update(); // asks for a PaintGL() call to occur
}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
    }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = false;
    }
}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseDown) {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);

        // Use deltaX and deltaY here to rotate

        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();

    // Use deltaTime and m_keyMap here to move around

    update(); // asks for a PaintGL() call to occur
}
