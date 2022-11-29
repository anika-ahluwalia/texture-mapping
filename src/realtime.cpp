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
      camera({ metadata.cameraData, 0, 0, 0, 0 }),
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
    param1 = settings.shapeParameter1;
    param2 = settings.shapeParameter2;
    nearPlane = settings.nearPlane;
    farPlane = settings.farPlane;
}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

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


    // clearing screen and loading shader
    glClearColor(0, 0, 0, 255);
    m_shader = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert", ":/resources/shaders/default.frag");

    // load and bind VBOs and VAOs for each shape
    gl = GLHelper(settings.shapeParameter1, settings.shapeParameter2);
    gl.generateAllShapes();

    // setting a flag to let us know that it is initialized
    is_intialized = true;
}

void Realtime::paintGL() {

    std::vector<RenderShapeData> shapes = metadata.shapes;

    // clear screen color and depth before painting
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    GLuint vao;
    std::vector<float> shape_data;

    // looping through each primitive in the scene
    for (int index = 0; index < shapes.size(); index++) {

        if (settings.extraCredit2) {
            glm::vec4 objectMiddle = glm::vec4{0.f, 0.f, 0.f, 1.f};
            glm::vec4 objectInWorldSpace = shapes[index].ctm * objectMiddle;
            float distance = glm::length(objectInWorldSpace - camera_pos);
            int param = fmax(fmin(80 / distance, 25), 3);
            gl.makeOneShape(shapes[index].primitive.type, param, param);
        }

        // setting vao and shape data based on the type of the primitive
        switch (shapes[index].primitive.type) {
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

        // binding vao and shader
        glBindVertexArray(vao);
        glUseProgram(m_shader);

        // passing in uniforms specific to this primitive
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "modelMatrix"), 1, GL_FALSE, &shapes[index].ctm[0][0]);
        glUniformMatrix3fv(glGetUniformLocation(m_shader, "invTpModelMatrix"), 1, GL_FALSE, &glm::inverse(glm::transpose(glm::mat3(shapes[index].ctm)))[0][0]);

        glm::vec4 ambient = metadata.globalData.ka * shapes[index].primitive.material.cAmbient;
        glUniform4fv(glGetUniformLocation(m_shader, "ambient"), 1, &ambient[0]);
        glm::vec4 diffuse = metadata.globalData.kd * shapes[index].primitive.material.cDiffuse;
        glUniform4fv(glGetUniformLocation(m_shader, "diffuseCoefficients"), 1, &diffuse[0]);
        glm::vec4 specular = metadata.globalData.ks * shapes[index].primitive.material.cSpecular;
        glUniform4fv(glGetUniformLocation(m_shader, "specularCoefficients"), 1, &specular[0]);

        glUniform1f(glGetUniformLocation(m_shader, "shininess"), shapes[index].primitive.material.shininess);

        // drawing!!
        glDrawArrays(GL_TRIANGLES, 0, shape_data.size() / 3);

        // unbinding vao and shader
        glBindVertexArray(0);
        glUseProgram(0);
    }
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // regenerates the camera & projection due to parameter change and re-renders the scene
    generateMatrices(metadata.cameraData);
    update(); // asks for a PaintGL() call to occur
}

void Realtime::generateMatrices(SceneCameraData& cameraData) {
    // creates a new camera with the updated parameters
    camera = Camera(cameraData, size().width(), size().height(), settings.nearPlane, settings.farPlane);

    this->makeCurrent();
    glUseProgram(m_shader);

    // sets the related uniform variables accordingly
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "viewMatrix"), 1, GL_FALSE, &camera.getViewMatrix()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "projectionMatrix"), 1, GL_FALSE, &camera.getProjectionMatrix()[0][0]);

    glm::vec4 origin = {0.f, 0.f, 0.f, 1.f};
    camera_pos = camera.getInverseViewMatrix() * origin;
    glUniform4fv(glGetUniformLocation(m_shader, "worldSpaceCameraPos"), 1, &camera_pos[0]);

    glUseProgram(0);
    this->doneCurrent();
}

void Realtime::sceneChanged() {

    // loading in the scene data
    SceneParser::parse(settings.sceneFilePath, metadata);
    // generating new matrices (because camera data could have changed)
    generateMatrices(metadata.cameraData);

    this->makeCurrent();
    glUseProgram(m_shader);

    // setting uniforms based on what changed in the new scene
    glUniform1i(glGetUniformLocation(m_shader, "numLights"), fmin(8, metadata.lights.size()));

    // passing in light colors and directions
    for (int i = 0; i < fmin(metadata.lights.size(), 8); i++) {
        SceneLightData light = metadata.lights[i];

        // modify empties ?? (ex. direction for point & spot light)

        std::string type_pos = "lightTypes[" + std::to_string(i) + "]";
        GLint type_loc = glGetUniformLocation(m_shader, type_pos.c_str());
        int type = light.type == LightType::LIGHT_DIRECTIONAL ? 0 : light.type == LightType::LIGHT_POINT ? 1 : 2;
        glUniform1i(type_loc, type);

        std::string dir_pos = "lightDirections[" + std::to_string(i) + "]";
        GLint dir_loc = glGetUniformLocation(m_shader, dir_pos.c_str());
        glUniform3f(dir_loc, light.dir[0], light.dir[1], light.dir[2]);

        std::string color_pos = "lightColors[" + std::to_string(i) + "]";
        GLint color_loc = glGetUniformLocation(m_shader, color_pos.c_str());
        glUniform4f(color_loc, light.color[0], light.color[1], light.color[2], light.color[3]);

        std::string pos_pos = "lightPositions[" + std::to_string(i) + "]";
        GLint pos_loc = glGetUniformLocation(m_shader, pos_pos.c_str());
        glUniform4f(pos_loc, light.pos[0], light.pos[1], light.pos[2], light.pos[3]);

        std::string func_pos = "lightFunctions[" + std::to_string(i) + "]";
        GLint func_loc = glGetUniformLocation(m_shader, func_pos.c_str());
        glUniform3f(func_loc, light.function[0], light.function[1], light.function[2]);

        std::string angle_pos = "lightAngles[" + std::to_string(i) + "]";
        GLint angle_loc = glGetUniformLocation(m_shader, angle_pos.c_str());
        glUniform1f(angle_loc, light.angle);

        std::string pen_pos = "lightPenumbras[" + std::to_string(i) + "]";
        GLint pen_loc = glGetUniformLocation(m_shader, pen_pos.c_str());
        glUniform1f(pen_loc, light.penumbra);
    }

    // adaptive level of detail
    if (settings.extraCredit1) {
        int param = 25 / metadata.shapes.size();
        gl = GLHelper(settings.shapeParameter1, settings.shapeParameter2);
        gl.generateAllShapes();
    }

    glUseProgram(0);
    this->doneCurrent();

    update(); // asks for a PaintGL() call to occur
}

void Realtime::settingsChanged() {

    if (is_intialized) {

        this->makeCurrent();

        // only updating and regenerating what is necessary based on which parameters changed

        // adaptive level of detail
        if (settings.extraCredit1 && !adaptiveShapes) {
            adaptiveShapes = true;
            int param = fmax(fmin(40 / metadata.shapes.size(), 25), 3);
            gl = GLHelper(param, param);
            gl.generateAllShapes();
        }
        if (settings.extraCredit2 && !adaptiveDistance) {
            adaptiveDistance = true;
        }

        if (param1 != settings.shapeParameter1 || param2 != settings.shapeParameter2 || (!settings.extraCredit1 && adaptiveShapes) || (!settings.extraCredit2 && adaptiveDistance)) {
            if (!settings.extraCredit1 && !settings.extraCredit2) {
                adaptiveShapes = false;
                // adaptiveDistance = false;
                gl = GLHelper(settings.shapeParameter1, settings.shapeParameter2);
                gl.generateAllShapes();
                param1 = settings.shapeParameter1;
                param2 = settings.shapeParameter2;
            }
        }

        if (nearPlane != settings.nearPlane || farPlane != settings.farPlane) {
            generateMatrices(metadata.cameraData);
            nearPlane = settings.nearPlane;
            farPlane = settings.farPlane;
        }

        this->doneCurrent();

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

    bool changeFlag = false;
    glm::vec3 translation_vec = glm::vec3 {0,0,0};

    // change position vector for translation
    glm::vec3 look = metadata.cameraData.look;
    glm::vec3 up = metadata.cameraData.up;

    if (m_keyMap[Qt::Key_W]) {
        translation_vec = 5.f * deltaTime * glm::normalize(look);
        changeFlag = true;
    }

    if (m_keyMap[Qt::Key_S]) {
        translation_vec = -5.f * deltaTime * glm::normalize(look);
        changeFlag = true;
    }

    if (m_keyMap[Qt::Key_A]) {
        translation_vec = -5.f * deltaTime * glm::normalize(glm::cross(look, up));
        changeFlag = true;
    }

    if (m_keyMap[Qt::Key_D]) {
        translation_vec = 5.f * deltaTime * glm::normalize(glm::cross(look, up));
        changeFlag = true;
    }

    if (m_keyMap[Qt::Key_Space]) {
        translation_vec = 5.f * deltaTime * glm::vec3 {0, 1, 0};
        changeFlag = true;
    }

    if (m_keyMap[Qt::Key_Control]) {
        translation_vec = 5.f * deltaTime * glm::vec3 {0, -1, 0};
        changeFlag = true;
    }

    if (changeFlag) {
        metadata.cameraData.pos = metadata.cameraData.pos + glm::vec4(translation_vec, 0);
        generateMatrices(metadata.cameraData);
    }

    update(); // asks for a PaintGL() call to occur
}
