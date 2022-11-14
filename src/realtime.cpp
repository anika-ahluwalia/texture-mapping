#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "settings.h"
#include <GL/glew.h>

#include "utils/scenedata.h"
#include "utils/sceneparser.h"
#include "utils/shaderloader.h"
#include "debug.h"

// ================== Project 5: Lights, Camera

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent),
      gl({ settings.shapeParameter1, settings.shapeParameter2 })
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
    m_shader = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert", ":/resources/shaders/default.frag");
    Debug::glErrorCheck();

    // load and bind VBOs and VAOs for each shape
    gl.generateAllShapes();

    Debug::glErrorCheck();

    is_intialized = true;
}

void Realtime::paintGL() {
    // Students: anything requiring OpenGL calls every frame should be done here

    std::vector<RenderShapeData> shapes = metadata.shapes;

    GLuint vao;
    std::vector<float> shape_data;

    for (int i = 0; i < shapes.size(); i++) {
        switch (shapes[i].primitive.type) {
            case PrimitiveType::PRIMITIVE_CUBE: {
                vao = gl.cube_vao;
                shape_data = gl.cube_data;
                break;
            }
            case PrimitiveType::PRIMITIVE_CONE: {
                vao = gl.cone_vao;
                shape_data = gl.cone_data;
                break;
            }
            case PrimitiveType::PRIMITIVE_SPHERE: {
                vao = gl.sphere_vao;
                shape_data = gl.sphere_data;
                break;
            }
            case PrimitiveType::PRIMITIVE_CYLINDER: {
                vao = gl.cylinder_vao;
                shape_data = gl.cylinder_data;
                break;
            }
        }

        // Clear screen color and depth before painting
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);

        // Activate the shader program by calling glUseProgram with `m_shader`
        glUseProgram(m_shader);

        glUniformMatrix4fv(glGetUniformLocation(m_shader, "viewMatrix"), 1, GL_FALSE, &m_view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "projectionMatrix"), 1, GL_FALSE, &m_projection[0][0]);
        // Pass in m_model as a uniform into the shader program
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "modelMatrix"), 1, GL_FALSE, &shapes[i].ctm[0][0]);

        glUniform1f(glGetUniformLocation(m_shader, "ka"), metadata.globalData.ka);
        glUniform1f(glGetUniformLocation(m_shader, "kd"), metadata.globalData.kd);
        glUniform1f(glGetUniformLocation(m_shader, "ks"), metadata.globalData.ks);

        glUniform4fv(glGetUniformLocation(m_shader, "cAmbient"), 1, &shapes[i].primitive.material.cAmbient[0]);
        glUniform4fv(glGetUniformLocation(m_shader, "cDiffuse"), 1, &shapes[i].primitive.material.cDiffuse[0]);
        glUniform4fv(glGetUniformLocation(m_shader, "cSpecular"), 1, &shapes[i].primitive.material.cSpecular[0]);

        glm::vec4 origin = {0.f, 0.f, 0.f, 1.f};
        glm::vec4 camera_pos = glm::inverse(m_view) * origin;
        glUniform4fv(glGetUniformLocation(m_shader, "worldSpaceCameraPos"), 1, &camera_pos[0]);

        glUniform1f(glGetUniformLocation(m_shader, "shininess"), shapes[i].primitive.material.shininess);

        for (int i = 0; i < metadata.lights.size(); i++) {
            SceneLightData light = metadata.lights[i];

            std::string dir_pos = "lightDirections[" + std::to_string(i) + "]";
            GLint dir_loc = glGetUniformLocation(m_shader, dir_pos.c_str());
            glUniform3f(dir_loc, light.dir[0], light.dir[1], light.dir[2]);

             std::string color_pos = "lightColors[" + std::to_string(i) + "]";
            GLint color_loc = glGetUniformLocation(m_shader, color_pos.c_str());
            glUniform4f(color_loc, light.color[0], light.color[1], light.color[2], light.color[3]);
        }


        // Draw Command
        glDrawArrays(GL_TRIANGLES, 0, shape_data.size() / 3);
        // Unbind Vertex Array
        glBindVertexArray(0);

        // Deactivate the shader program
        glUseProgram(0);
    }
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here
    generateMatrices(metadata.cameraData);
}

void Realtime::generateMatrices(SceneCameraData& cameraData) {
    Camera camera = Camera(cameraData, size().width(), size().height(), settings.nearPlane, settings.farPlane);

    m_view = camera.getViewMatrix();
    m_inverse_view = camera.getInverseViewMatrix();
    m_projection = camera.getProjectionMatrix();
}

void Realtime::sceneChanged() {

    // loading in the scene data
    bool success = SceneParser::parse(settings.sceneFilePath, metadata);
    generateMatrices(metadata.cameraData);

    update(); // asks for a PaintGL() call to occur
}

void Realtime::settingsChanged() {

    if (is_intialized) {
        gl = GLHelper(settings.shapeParameter1, settings.shapeParameter2);
        gl.generateAllShapes();

        Debug::glErrorCheck();

        update(); // asks for a PaintGL() call to occur
    }
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
