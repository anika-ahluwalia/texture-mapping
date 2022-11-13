#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "settings.h"
#include <GL/glew.h>

#include "shapes/cube.h"
#include "shapes/sphere.h"
#include "shapes/cylinder.h"
#include "shapes/cone.h"

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

    // If you must use this function, do not edit anything above this
}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Students: anything requiring OpenGL calls when the program exits should be done here

    glDeleteBuffers(1, &cube_vbo);
    glDeleteVertexArrays(1, &cube_vao);
    glDeleteBuffers(1, &sphere_vbo);
    glDeleteVertexArrays(1, &sphere_vao);
    glDeleteBuffers(1, &cylinder_vbo);
    glDeleteVertexArrays(1, &cylinder_vao);
    glDeleteBuffers(1, &cone_vbo);
    glDeleteVertexArrays(1, &cone_vao);

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
    generateCubeVBOsVAOs();
    generateSphereVBOsVAOs();
    generateCylinderVBOsVAOs();
    generateConeVBOsVAOs();

    Debug::glErrorCheck();
}

void Realtime::generateCubeVBOsVAOs() {

    // Vertex Buffer Objects //
    glGenBuffers(1, &cube_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
    Cube cube = Cube();
    cube.updateParams(settings.shapeParameter1);
    std::vector<float> cube_vec = cube.generateShape();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cube_vec.size(), &cube_vec[0], GL_STATIC_DRAW);

    Debug::glErrorCheck();

    // Vertex Array Objects //
    glGenVertexArrays(1, &cube_vao);
    glBindVertexArray(cube_vao);

    // Add position and color attributes to VAO
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));

    Debug::glErrorCheck();

    // Returning to Default State //
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Debug::glErrorCheck();
}

void Realtime::generateSphereVBOsVAOs() {

    // Vertex Buffer Objects //
    glGenBuffers(1, &sphere_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sphere_vbo);
    Sphere sphere = Sphere();
    sphere.updateParams(settings.shapeParameter1, settings.shapeParameter2);
    std::vector<float> sphere_vec = sphere.generateShape();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sphere_vec.size(), &sphere_vec[0], GL_STATIC_DRAW);

    Debug::glErrorCheck();

    // Vertex Array Objects //
    glGenVertexArrays(1, &sphere_vao);
    glBindVertexArray(sphere_vao);

    // Task 13: Add position and color attributes to your VAO here
    // position
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));

    Debug::glErrorCheck();

    // Returning to Default State //
    // unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Debug::glErrorCheck();
}

void Realtime::generateCylinderVBOsVAOs() {

    // Vertex Buffer Objects //
    glGenBuffers(1, &cylinder_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cylinder_vbo);
    Cylinder cylinder = Cylinder();
    cylinder.updateParams(settings.shapeParameter1, settings.shapeParameter2);
    std::vector<float> cylinder_vec = cylinder.generateShape();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cylinder_vec.size(), &cylinder_vec[0], GL_STATIC_DRAW);

    Debug::glErrorCheck();

    // Vertex Array Objects //
    glGenVertexArrays(1, &cylinder_vao);
    glBindVertexArray(cylinder_vao);

    // Task 13: Add position and color attributes to your VAO here
    // position
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));

    Debug::glErrorCheck();

    // Returning to Default State //
    // unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Debug::glErrorCheck();
}

void Realtime::generateConeVBOsVAOs() {

    // Vertex Buffer Objects //
    glGenBuffers(1, &cone_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cone_vbo);
    Cone cone = Cone();
    cone.updateParams(settings.shapeParameter1, settings.shapeParameter2);
    std::vector<float> cone_vec = cone.generateShape();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cone_vec.size(), &cone_vec[0], GL_STATIC_DRAW);

    Debug::glErrorCheck();

    // Vertex Array Objects //
    glGenVertexArrays(1, &cone_vao);
    glBindVertexArray(cone_vao);

    // Task 13: Add position and color attributes to your VAO here
    // position
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, reinterpret_cast<void*>(3 * sizeof(GL_FLOAT)));

    Debug::glErrorCheck();

    // Returning to Default State //
    // unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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
//    glUniform1f(glGetUniformLocation(m_shader, "kd"), m_kd);
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

    generateCubeVBOsVAOs();
    generateSphereVBOsVAOs();
    generateCylinderVBOsVAOs();
    generateConeVBOsVAOs();

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
