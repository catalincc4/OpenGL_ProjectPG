#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp" //core glm functionality
#include "glm/gtc/matrix_transform.hpp" //glm extension for generating common transformation matrices
#include "glm/gtc/matrix_inverse.hpp" //glm extension for computing inverse matrices
#include "glm/gtc/type_ptr.hpp" //glm extension for accessing the internal data structure of glm types

#include "Window.h"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Model3D.hpp"

#include <iostream>
#include "SkyBox.hpp"
#include "main.h"


// window
gps::Window myWindow;

// matrices
glm::mat4 model;
glm::mat4 modelHouseDoor;
glm::mat4 modelGarageDoor;
glm::mat4 modelCastelDoor;
glm::mat4 modelFrontWhells;
glm::mat4 modelBackWhells;
glm::mat4 modelCarBody;
glm::mat4 modelSphereLight;
glm::mat4 modelLamp;
glm::mat4 modelLampGarage;
glm::mat4 modelWindows;
glm::mat4 modelQuad;
glm::mat4 view;
glm::mat4 projection;
glm::mat3 normalMatrix;
glm::mat4 a;
glm::mat4 b;

int presentationFlag = 1;


float carSpeed = 1.0f;

// light parameters
glm::vec3 lightDir;
glm::vec3 lightColor;
glm::mat4 lightRotation;

time_t start, end;

GLfloat lightAngle;

// shader uniform locations
GLuint modelLoc;

GLuint viewLoc;
GLuint projectionLoc;
GLuint normalMatrixLoc;
GLuint lightDirLoc;
GLuint lightColorLoc;

// camera
gps::Camera myCamera(
    glm::vec3(-0.145598f, 7.95246f, 58.7122f),
    glm::vec3(0.802952f, 6.7929f, 4.43287f),
    glm::vec3(0.0f, 1.0f, 0.0f));

float yaw = 0, pitch = 0, lastX, lastY;
bool firstMouse = false;

int openCastelDoor = 1;
int openHouseDoor = 1;
int openGarageDoor = 1;

bool moveCarForward = false;
bool moveCarForward1 = false;
bool moveCarForward2 = false;
bool moveCarForward3 = false;
bool moveCarBackward = false;
bool moveCarBackward1 = false;
bool moveCarBackward2 = false;
bool moveCarBackward3 = false;
bool lightRotate = false;
bool lightRotate2 = false;
float angleHouseDoor = 0.0f;
float angleGarageDoor = 0.0f;
float angleCastelDoor = 0.0f;

float translatex = 0.0f;
float translatey = 0.0f;
float translatez = 0.0f;
float angleTranslate = 0.0f;
bool startAgain = false;

GLfloat cameraSpeed = 0.007f;

GLboolean pressedKeys[1024];

// models
gps::Model3D map;
gps::Model3D garageDoor;
gps::Model3D castelDoor;
gps::Model3D houseDoor;
gps::Model3D frontWhells;
gps::Model3D backWhells;
gps::Model3D carBody;
gps::Model3D sphereLight;
gps::Model3D lamp;
gps::Model3D lampGarage;
gps::Model3D windows;
gps::SkyBox skyBox;
gps::SkyBox skyBox1;
gps::Model3D quad;

bool depthMapOn = false;



const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;

GLfloat angle;

GLfloat fogDensity = 0.0f;

float cameraRotate = 0.009f;


int retina_width, retina_height;

float positionx;
float positiony;
float positionz;
float targetx;
float targety;
float targetz;

// shaders
gps::Shader myBasicShader;
gps::Shader skyBoxShader;
gps::Shader quadShader;
gps::Shader depthMapShader;

std::vector<const GLchar*> faces;
std::vector<const GLchar*> faces1;

std::ifstream f("presentation.txt");

int modeSelect = 0;

GLuint shadowMapFBO;
GLuint depthMapTexture;
int renderReflection = 1;

bool showDepthMap;
float timeToWait = 10.0f;
bool moveCarP = false;



GLenum glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
    fprintf(stdout, "Window resized! New width: %d , and height: %d\n", width, height);
    //TODO
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (pressedKeys[GLFW_KEY_N]) {
        fogDensity += 0.005f;
        myBasicShader.useShaderProgram();
        GLuint fogLoc = glGetUniformLocation(myBasicShader.shaderProgram, "fogDensity");
        glUniform1f(fogLoc, fogDensity);


    }
    if (pressedKeys[GLFW_KEY_M]) {
        fogDensity -= 0.005f;
        myBasicShader.useShaderProgram();
        GLuint fogLoc = glGetUniformLocation(myBasicShader.shaderProgram, "fogDensity");
        glUniform1f(fogLoc, fogDensity);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            pressedKeys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            pressedKeys[key] = false;
        }
    }
    if (pressedKeys[GLFW_KEY_O]) {
        presentationFlag = -1;

    }

    if (pressedKeys[GLFW_KEY_0]) {
        modeSelect += 1;
        myBasicShader.useShaderProgram();
        GLuint modeLoc = glGetUniformLocation(myBasicShader.shaderProgram, "modeSelect");
        glUniform1i(modeLoc, modeSelect % 3);
    }

    if (pressedKeys[GLFW_KEY_9]) {
        std::cout << myCamera.cameraPosition.x << " " << myCamera.cameraPosition.y << " " << myCamera.cameraPosition.z << std::endl
            << myCamera.cameraTarget.x << " " << myCamera.cameraTarget.y << " " << myCamera.cameraTarget.z << "\n\n";
    }

    if (pressedKeys[GLFW_KEY_8]) {
        depthMapOn = !depthMapOn;
    }




}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    //if (!firstMouse)
    //{
    //    lastX = xpos;
    //    lastY = ypos;
    //    firstMouse = true;
    //}

    //float xoffset = lastX - xpos;
    //float yoffset = ypos - lastY;
    //lastX = xpos;
    //lastY = ypos;

    //float sensitivity = 0.00001f;
    //xoffset *= sensitivity;
    //yoffset *= sensitivity;

    //yaw += xoffset;
    //pitch += yoffset;


    //myCamera.rotate(yaw, pitch);
    //view = myCamera.getViewMatrix();
    //myBasicShader.useShaderProgram();
    //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    //// compute normal matrix for map
    //normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
}

void processMovement() {

    if (pressedKeys[GLFW_KEY_1]) {
        openHouseDoor *= -1;
    }

    if (pressedKeys[GLFW_KEY_2]) {
        openCastelDoor *= -1;
    }

    if (pressedKeys[GLFW_KEY_3]) {
        openGarageDoor *= -1;
    }

    if (pressedKeys[GLFW_KEY_4]) {
        moveCarForward = true;
        moveCarForward1 = true;
        moveCarForward2 = true;
        moveCarForward3 = true;
    }

    if (pressedKeys[GLFW_KEY_5]) {
        moveCarBackward = true;
        moveCarBackward1 = true;
        moveCarBackward2 = true;
        moveCarBackward3 = true;
    }

    if (pressedKeys[GLFW_KEY_6]) {
        carSpeed += 0.04f;
    }

    if (pressedKeys[GLFW_KEY_7]) {
        carSpeed -= 0.04f;;
    }



    if (pressedKeys[GLFW_KEY_LEFT]) {

        /*     if (presentationFlag == -1) {
                 f << 1 << " ";
             }*/
        myCamera.move(gps::MOVE_LEFT, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for map
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }


    if (pressedKeys[GLFW_KEY_UP]) {
        /*   if (presentationFlag == -1) {
               f << 2 << " ";
           }*/
        myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for map
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_DOWN]) {
        //if (presentationFlag == -1) {
        //    f << 3 << " ";
        //}
        myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for map
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_RIGHT]) {
        //if (presentationFlag == -1) {
        //    f << 4 << " ";
        //}
        myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for map
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_W]) {
        //if (presentationFlag == -1) {
        //    f << 5 << " ";
        //}

        myCamera.rotate(cameraRotate, 0.0f);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for map
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_S]) {

        //if (presentationFlag == -1) {
        //    f << 6 << " ";
        //}

        myCamera.rotate(-cameraRotate, 0.0f);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for map
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_A]) {
        //if (presentationFlag == -1) {
        //    f << 7 << " ";
        //}

        myCamera.rotate(0.0f, cameraRotate);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for map
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_D]) {
        //if (presentationFlag == -1) {
        //    f << 8 << " ";
        //}
        myCamera.rotate(0.0f, -cameraRotate);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // compute normal matrix for map
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

if (pressedKeys[GLFW_KEY_P]) {

    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}

if (pressedKeys[GLFW_KEY_F]) {

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
if (pressedKeys[GLFW_KEY_L]) {

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

if (pressedKeys[GLFW_KEY_K]) {

    lightAngle += 2.0f;
    lightRotate = true;
    myBasicShader.useShaderProgram();
    view = myCamera.getViewMatrix();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    lightRotation = glm::rotate(model, glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));
}
if (pressedKeys[GLFW_KEY_J]) {
    lightAngle -= 2.0f;
    lightRotate2 = true;
    myBasicShader.useShaderProgram();
    view = myCamera.getViewMatrix();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    lightRotation = glm::rotate(model, glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));
}

if (pressedKeys[GLFW_KEY_O]) {

    presentationFlag *= -1;
    std::cout << presentationFlag << "\n";
}


}

void presentation() {
    time(&end);
    float time_taken = float(end - start);
    if ((abs(time_taken - timeToWait) <= 1e-5)) {
        moveCarP = false;
        startAgain = false;
        myBasicShader.useShaderProgram();
        GLuint modeLoc = glGetUniformLocation(myBasicShader.shaderProgram, "modeSelect");
        glUniform1i(modeLoc, 0);
    }
    else if (moveCarP) {
        if ((abs(time_taken - timeToWait) <= 5.0f)) {
            moveCarForward = true;
            moveCarForward1 = true;
            moveCarForward2 = true;
            moveCarForward3 = true;
            carSpeed += 0.1f;
        }
        else {
            moveCarBackward = true;
            moveCarBackward1 = true;
            moveCarBackward2 = true;
            moveCarBackward3 = true;
            carSpeed -= 0.1f;
        }
    }

    if ((presentationFlag == -1) && !startAgain) {
        int x = 0;
        
        if (!f.eof()) {
            f >> x;
            if (x == 1) {
                myCamera.move(gps::MOVE_LEFT, cameraSpeed);
                //update view matrix

            }
            else if (x == 2) {
                myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
                //update view matrix

            }
            else if (x == 3) {
                myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
                //update view matrix

            }
            else if (x == 4) {
                myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
                //update view matrix

            }
            else if (x == 5) {

                myCamera.rotate(cameraRotate, 0.0f);
                //update view matrix


            }
            else if (x == 6) {
                myCamera.rotate(-cameraRotate, 0.0f);
                //update view matrix

            }
            else if (x == 7) {
                myCamera.rotate(0.0f, cameraRotate);
                //update view matrix

            }
            else if (x == 8) {
                myCamera.rotate(0.0f, -cameraRotate);
                //update view matrix


            }
            view = myCamera.getViewMatrix();
            myBasicShader.useShaderProgram();
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            // compute normal matrix for map
            normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
          
            if ((abs(myCamera.cameraPosition.x - 1.74466) <= 1e-4) && (abs(myCamera.cameraPosition.y - 4.02137) <= 1e-4) && (abs(myCamera.cameraPosition.z + 14.1865) <= 1e-4) &&
                (abs(myCamera.cameraTarget.x + 25.2373) <= 1e-4) && (abs(myCamera.cameraTarget.y + 0.553792) <= 1e-4) && (abs(myCamera.cameraTarget.z - 32.7133) <= 1e-4)) {
                moveCarP = true;
                time(&start);
                timeToWait = 10.0f;
                startAgain = true;
            }

            if ((abs(myCamera.cameraPosition.x + 18.8167) <= 1e-4) && (abs(myCamera.cameraPosition.y - 2.35528) <= 1e-4) && (abs(myCamera.cameraPosition.z + 11.587) <= 1e-3) &&
                (abs(myCamera.cameraTarget.x + 34.637) <= 1e-4) && (abs(myCamera.cameraTarget.y + 3.67897) <= 1e-4) && (abs(myCamera.cameraTarget.z + 63.1807) <= 1e-4)) {
                openHouseDoor = -1;
                time(&start);
                timeToWait = 2.0f;
                startAgain = true;
            }

            if ((abs(myCamera.cameraPosition.x +20.5957) <= 1e-4) && (abs(myCamera.cameraPosition.y - 1.95201) <= 1e-4) && (abs(myCamera.cameraPosition.z + 17.3887) <= 1e-4) &&
                (abs(myCamera.cameraTarget.x + 19.6492) <= 1e-4) && (abs(myCamera.cameraTarget.y + 1.64853) <= 1e-4) && (abs(myCamera.cameraTarget.z +71.5621) <= 1e-4)) {
               // openHouseDoor = -1;
                myBasicShader.useShaderProgram();
                GLuint modeLoc = glGetUniformLocation(myBasicShader.shaderProgram, "modeSelect");
                glUniform1i(modeLoc,  1);
                time(&start);
                timeToWait = 4.0f;
                startAgain = true;
            }
            if ((abs(myCamera.cameraPosition.x - 4.1216) <= 1e-4) && (abs(myCamera.cameraPosition.y - 3.98648) <= 1e-4) && (abs(myCamera.cameraPosition.z + 35.0418) <= 1e-4) &&
                (abs(myCamera.cameraTarget.x - 56.5485) <= 1e-4) && (abs(myCamera.cameraTarget.y + 2.04776) <= 1e-4) && (abs(myCamera.cameraTarget.z + 47.834) <= 1e-4)) {
                openGarageDoor = -1;
                time(&start);
                timeToWait = 2.0f;
                startAgain = true;
            }
        
            if ((abs(myCamera.cameraPosition.x - 3.55742) <= 1e-4) && (abs(myCamera.cameraPosition.y - 3.24972) <= 1e-4) && (abs(myCamera.cameraPosition.z + 52.4158) <= 1e-4) &&
                (abs(myCamera.cameraTarget.x + 13.7262) <= 1e-4) && (abs(myCamera.cameraTarget.y - 6.48657) <= 1e-4) && (abs(myCamera.cameraTarget.z + 103.791) <= 1e-4)) {
                openCastelDoor = -1;
                time(&start);
                timeToWait = 2.0f;
                startAgain = true;
            }

        }
        else {

            f.close();
            presentationFlag = 1;
        }
    }




}


void initOpenGLWindow() {
    myWindow.Create(1800, 1000, "OpenGL Project Core");
}

void setWindowCallbacks() {
    glfwSetWindowSizeCallback(myWindow.getWindow(), windowResizeCallback);
    glfwSetKeyCallback(myWindow.getWindow(), keyboardCallback);
    glfwSetCursorPosCallback(myWindow.getWindow(), mouseCallback);
}

void initOpenGLState() {
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    //glEnable(GL_CULL_FACE); // cull face
    //glCullFace(GL_BACK); // cull back face
    glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

}

void initModels() {
    map.LoadModel("models/map/map.obj");
    castelDoor.LoadModel("models/castelDoor/castelDoor.obj");
    houseDoor.LoadModel("models/houseDoor/houseDoor.obj");
    garageDoor.LoadModel("models/garageDoor/garageDoor.obj");
    backWhells.LoadModel("models/rearWhell/rearWhells.obj");
    frontWhells.LoadModel("models/frontWhell/frontWhells.obj");
    carBody.LoadModel("models/alfaRomeCarBody/carBodyAlfaRome.obj");
    sphereLight.LoadModel("models/sphereLight/sphere.obj");
    lamp.LoadModel("models/lamp/lamp.obj");
    lampGarage.LoadModel("models/lampGarage/lampGarage.obj");
    windows.LoadModel("models/carWindows/carWindows.obj");
    quad.LoadModel("models/quad/quad.obj");

    faces.push_back("skybox/skybox1/right.tga");
    faces.push_back("skybox/skybox1/left.tga");
    faces.push_back("skybox/skybox1/top.tga");
    faces.push_back("skybox/skybox1/bottom.tga");
    faces.push_back("skybox/skybox1/back.tga");
    faces.push_back("skybox/skybox1/front.tga");


    skyBox.Load(faces);


    faces1.push_back("skybox/skybox2/purplenebula_rt.tga");
    faces1.push_back("skybox/skybox2/purplenebula_lf.tga");
    faces1.push_back("skybox/skybox2/purplenebula_up.tga");
    faces1.push_back("skybox/skybox2/purplenebula_dn.tga");
    faces1.push_back("skybox/skybox2/purplenebula_bk.tga");
    faces1.push_back("skybox/skybox2/purplenebula_ft.tga");


    skyBox1.Load(faces1);

}

void initShaders() {
    myBasicShader.loadShader(
        "shaders/basic.vert",
        "shaders/basic.frag");
    skyBoxShader.loadShader(
        "shaders/skyboxShader.vert",
        "shaders/skyboxShader.frag");

    quadShader.loadShader(
        "shaders/screenQuad.vert",
        "shaders/screenQuad.frag");

    depthMapShader.loadShader(
        "shaders/depthMap.vert",
        "shaders/depthMap.frag");


}

void initUniforms() {
    myBasicShader.useShaderProgram();

    modelHouseDoor = glm::mat4(1.0f);
    modelGarageDoor = glm::mat4(1.0f);
    modelCastelDoor = glm::mat4(1.0f);
    modelFrontWhells = glm::mat4(1.0f);
    modelBackWhells = glm::mat4(1.0f);
    modelCarBody = glm::mat4(1.0f);
    modelSphereLight = glm::mat4(1.0f);
    modelLamp = glm::mat4(1.0f);
    modelLampGarage = glm::mat4(1.0f);
    modelWindows = glm::mat4(1.0f);
    modelQuad = glm::mat4(1.0f);


    // create model matrix for map
    model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    modelLoc = glGetUniformLocation(myBasicShader.shaderProgram, "model");


    // get view matrix for current camera
    view = myCamera.getViewMatrix();
    viewLoc = glGetUniformLocation(myBasicShader.shaderProgram, "view");
    // send view matrix to shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // compute normal matrix for map
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    normalMatrixLoc = glGetUniformLocation(myBasicShader.shaderProgram, "normalMatrix");

    // create projection matrix
    projection = glm::perspective(glm::radians(45.0f),
        (float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height,
        0.1f, 10000.0f);
    projectionLoc = glGetUniformLocation(myBasicShader.shaderProgram, "projection");
    // send projection matrix to shader
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //set the light direction (direction towards the light)
    lightDir = glm::vec3(-2.069f + 20.0f, 47.693f, 25.8527f);
    //lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
    lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    lightDirLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightDir");
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

    // send light dir to shader
    //glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

    //set light color
    lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
    lightColorLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightColor");
    // send light color to shader
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

}

glm::mat4 computeLightSpaceTrMatrix() {
    //TODO - Return the light-space transformation matrix
    glm::mat4 lightView = glm::lookAt(glm::inverseTranspose(glm::mat3(lightRotation)) * lightDir, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    const GLfloat near_plane = 0.1f, far_plane = 100.0f;
    glm::mat4 lightProjection = glm::ortho(-150.0f, 150.0f, -150.0f, 150.0f, near_plane, far_plane);

    glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;

    return lightSpaceTrMatrix;
}

void initFBO() {
    //create depth texture for FBO
    glGenFramebuffers(1, &shadowMapFBO);

    glGenTextures(1, &depthMapTexture);
    //create depth texture for FBO
    glGenTextures(1, &depthMapTexture);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    //attach texture to FBO
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void rendermap(gps::Shader shader, bool depthPass) {
    // select active shader program
    shader.useShaderProgram();

    //send map model matrix data to shader
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

    //send map normal matrix data to shader
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }


    // draw map
    map.Draw(shader);


}
void renderHouseDoor(gps::Shader shader, bool depthPass) {
    // select active shader program
    shader.useShaderProgram();
    if (openHouseDoor == -1) {
        if (angleHouseDoor > -80.0f) {
            modelHouseDoor = glm::translate(modelHouseDoor, glm::vec3(-21.095f, 1.2437f, -17.128f));
            modelHouseDoor = glm::rotate(modelHouseDoor, glm::radians(-1.0f), glm::vec3(0, 1, 0));
            modelHouseDoor = glm::translate(modelHouseDoor, glm::vec3(21.095f, -1.2437f, 17.128f));
            angleHouseDoor -= 1.0f;

        }
    }
    else if (openHouseDoor == 1) {
        if (angleHouseDoor < 0.0f) {
            modelHouseDoor = glm::translate(modelHouseDoor, glm::vec3(-21.095f, 1.2437f, -17.128f));
            modelHouseDoor = glm::rotate(modelHouseDoor, glm::radians(1.0f), glm::vec3(0, 1, 0));
            modelHouseDoor = glm::translate(modelHouseDoor, glm::vec3(21.095f, -1.2437f, 17.128f));
            angleHouseDoor += 1.0f;

        }
    }

    //send map model matrix data to shader
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelHouseDoor));

    //send map normal matrix data to shader
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }
    // draw map
    houseDoor.Draw(shader);


}
void renderCastelDoor(gps::Shader shader, bool depthPass) {
    // select active shader program
    shader.useShaderProgram();

    if (openCastelDoor == -1) {
        if (angleCastelDoor > -80.0f) {
            modelCastelDoor = glm::translate(modelCastelDoor, glm::vec3(-3.4373f, 3.4858f, -63.732f));
            modelCastelDoor = glm::rotate(modelCastelDoor, glm::radians(-1.0f), glm::vec3(0, 1, 0));
            modelCastelDoor = glm::translate(modelCastelDoor, glm::vec3(3.4373f, -3.4858f, 63.732f));
            angleCastelDoor -= 1.0f;

        }
    }
    else if (openCastelDoor == 1) {
        if (angleCastelDoor < 0.0f) {
            modelCastelDoor = glm::translate(modelCastelDoor, glm::vec3(-3.4373f, 3.4858f, -63.732f));
            modelCastelDoor = glm::rotate(modelCastelDoor, glm::radians(1.0f), glm::vec3(0, 1, 0));
            modelCastelDoor = glm::translate(modelCastelDoor, glm::vec3(3.4373f, -3.4858f, 63.732f));
            angleCastelDoor += 1.0f;

        }
    }

    //send map model matrix data to shader
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelCastelDoor));

    //send map normal matrix data to shader
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }
    // draw map
    castelDoor.Draw(shader);
}
void renderGarageDoor(gps::Shader shader, bool depthPass) {
    // select active shader program
    shader.useShaderProgram();

    if (openGarageDoor == -1) {
        if (angleGarageDoor > -80.0f) {
            modelGarageDoor = glm::translate(modelGarageDoor, glm::vec3(12.475f, 3.2543f, -37.594f));
            modelGarageDoor = glm::rotate(modelGarageDoor, glm::radians(-1.0f), glm::vec3(0, 0, 1));
            modelGarageDoor = glm::translate(modelGarageDoor, glm::vec3(-12.475f, -3.2543f, 37.594f));
            angleGarageDoor -= 1.0f;

        }
    }
    else if (openGarageDoor == 1) {
        if (angleGarageDoor < 0.0f) {
            modelGarageDoor = glm::translate(modelGarageDoor, glm::vec3(12.475f, 3.2543f, -37.594f));
            modelGarageDoor = glm::rotate(modelGarageDoor, glm::radians(1.0f), glm::vec3(0, 0, 1));
            modelGarageDoor = glm::translate(modelGarageDoor, glm::vec3(-12.475f, -3.2543f, 37.594f));
            angleGarageDoor += 1.0f;

        }
    }

    //send map model matrix data to shader
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelGarageDoor));

    //send map normal matrix data to shader
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }
    // draw map
    garageDoor.Draw(shader);
}
void renderfrontWhells(gps::Shader shader, bool depthPass) {
    // select active shader program
    shader.useShaderProgram();
    if (moveCarForward) {

        //rotate
        modelFrontWhells = glm::translate(modelFrontWhells, glm::vec3(-2.6546f, 0.68159f, 1.0834f));
        modelFrontWhells = glm::rotate(modelFrontWhells, glm::radians(1.0f * carSpeed), glm::vec3(1.0f, 0.0f, 0.0f));
        modelFrontWhells = glm::translate(modelFrontWhells, glm::vec3(2.6546f, -0.68159f, -1.0834f));
        //translate
        a = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.01f * carSpeed));
        modelFrontWhells = a * modelFrontWhells;

        moveCarForward = false;

    }

    if (moveCarBackward) {
        modelFrontWhells = glm::translate(modelFrontWhells, glm::vec3(-2.6546f, 0.68159f, 1.0834f));
        modelFrontWhells = glm::rotate(modelFrontWhells, glm::radians(-1.0f * carSpeed), glm::vec3(1, 0, 0));
        modelFrontWhells = glm::translate(modelFrontWhells, glm::vec3(2.6546f, -0.68159f, -1.0834f));

        a = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.01f * carSpeed));
        modelFrontWhells = a * modelFrontWhells;

        moveCarBackward = false;

    }
    //send map model matrix data to shader
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelFrontWhells));

    //send map normal matrix data to shader
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }
    // draw map
    frontWhells.Draw(shader);
}
void renderbackWhells(gps::Shader shader, bool depthPass) {
    // select active shader program
    shader.useShaderProgram();
    if (moveCarForward1) {
        modelBackWhells = glm::translate(modelBackWhells, glm::vec3(-2.7484f, 0.71341f, -3.8625f));
        modelBackWhells = glm::rotate(modelBackWhells, glm::radians(1.0f * carSpeed), glm::vec3(1, 0, 0));
        modelBackWhells = glm::translate(modelBackWhells, glm::vec3(2.7484f, -0.71341f, 3.8625f));

        glm::mat4 c = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.01f * carSpeed));
        modelBackWhells = c * modelBackWhells;

        //modelBackWhells = glm::translate(modelBackWhells, glm::vec3(0.0f, 0.0f, 0.01f));
        moveCarForward1 = false;

    }

    if (moveCarBackward1) {

        modelBackWhells = glm::translate(modelBackWhells, glm::vec3(-2.7484f, 0.71341f, -3.8625f));
        modelBackWhells = glm::rotate(modelBackWhells, glm::radians(-1.0f * carSpeed), glm::vec3(1, 0, 0));
        modelBackWhells = glm::translate(modelBackWhells, glm::vec3(2.7484f, -0.71341f, 3.8625f));

        b = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.01f * carSpeed));
        modelBackWhells = b * modelBackWhells;
        //modelBackWhells = glm::translate(modelBackWhells, glm::vec3(0.0f, 0.0f, -0.01f));
        moveCarBackward1 = false;

    }
    //send map model matrix data to shader
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelBackWhells));

    //send map normal matrix data to shader
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }
    // draw map
    backWhells.Draw(shader);
}
void renderCarBody(gps::Shader shader, bool depthPass) {
    // select active shader program
    shader.useShaderProgram();

    if (moveCarForward2) {
        modelCarBody = glm::translate(modelCarBody, glm::vec3(0.0f, 0.0f, 0.01f * carSpeed));
        moveCarForward2 = false;
    }

    if (moveCarBackward2) {
        modelCarBody = glm::translate(modelCarBody, glm::vec3(0.0f, 0.0f, -0.01f * carSpeed));
        moveCarBackward2 = false;
    }

    //send map model matrix data to shader
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelCarBody));

    //send map normal matrix data to shader
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        /*      glUniform3fv(glGetUniformLocation(shader.shaderProgram, "lightPoint"), 1, glm::value_ptr(glm::vec3(-3.2827f, 1.2749f, -2.0101f)));
              glUniform3fv(lightColorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));*/
    }
    // draw map
    carBody.Draw(shader);
}
void renderWindows(gps::Shader shader) {
    // select active shader program
    shader.useShaderProgram();

    if (moveCarForward3) {
        modelWindows = glm::translate(modelWindows, glm::vec3(0.0f, 0.0f, 0.01f * carSpeed));
        moveCarForward3 = false;
    }

    if (moveCarBackward3) {
        modelWindows = glm::translate(modelWindows, glm::vec3(0.0f, 0.0f, -0.01f * carSpeed));
        moveCarBackward3 = false;
    }
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //send map model matrix data to shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelWindows));

    //send map normal matrix data to shader
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    // draw map
    windows.Draw(shader);
}
void renderSkyBox(gps::Shader shader) {
    // select active shader program
    shader.useShaderProgram();

    skyBox.Draw(shader, view, projection);
}
void renderSkyBox1(gps::Shader shader) {
    // select active shader program
    shader.useShaderProgram();
    skyBox1.Draw(shader, view, projection);
}
void renderSphereLight(gps::Shader shader, bool depthPass) {
    // select active shader program
    shader.useShaderProgram();
    if (lightRotate) {
        glm::mat4 modelRotate = glm::translate(modelSphereLight, glm::vec3(-2.069f, 77.471f, -5.8527));
        modelRotate = glm::rotate(modelRotate, glm::radians(2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelRotate = glm::translate(modelRotate, glm::vec3(2.069f, -77.471f, 5.8527));

        glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f));


        modelSphereLight = modelRotate * modelTranslate;
        lightRotate = false;
    }
    if (lightRotate2) {
        glm::mat4 modelRotate = glm::translate(modelSphereLight, glm::vec3(-1.113f, 47.693f, -61.96f));
        modelRotate = glm::rotate(modelRotate, glm::radians(-2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelRotate = glm::translate(modelRotate, glm::vec3(1.1134f, -47.693f, 61.96f));

        glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, 0.0f));


        modelSphereLight = modelRotate * modelTranslate;
        lightRotate2 = false;
    }


    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSphereLight));

    //send map normal matrix data to shader
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    }
    sphereLight.Draw(shader);
}
void renderLamp(gps::Shader shader, bool depthPass) {
    // select active shader program
    shader.useShaderProgram();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLamp));
    glUniform3fv(glGetUniformLocation(shader.shaderProgram, "lightPoint"), 1, glm::value_ptr(glm::vec3(-20.429f, 1.7038f, -22.926f)));

    //send map normal matrix data to shader
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        if (modeSelect % 3 == 1) {
            glUniform3fv(lightColorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.0f)));
        }
        else {
            glUniform3fv(lightColorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        }
    }
    lamp.Draw(shader);
}
void renderLampGarage(gps::Shader shader, bool depthPass) {
    // select active shader program
    shader.useShaderProgram();
    glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelLampGarage));

    //send map normal matrix data to shader
    if (!depthPass) {
        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
        if (modeSelect % 3 == 1) {
            glUniform3fv(lightColorLoc, 1, glm::value_ptr(glm::vec3(0.9f, 1.0f, 0.0f)));
            glUniform3fv(glGetUniformLocation(shader.shaderProgram, "lightPoint"), 1, glm::value_ptr(glm::vec3(11.449f, 3.3909f, -41.083f)));
        }
        else {
            glUniform3fv(lightColorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        }
    }
    lampGarage.Draw(shader);
}
void drawObjects(gps::Shader shader, bool depthPass) {

    rendermap(shader, depthPass);

    renderHouseDoor(shader, depthPass);
    renderCastelDoor(shader, depthPass);
    renderGarageDoor(shader, depthPass);


    renderfrontWhells(shader, depthPass);
    renderLampGarage(shader, depthPass);

    renderbackWhells(shader, depthPass);


    renderCarBody(shader, depthPass);


}
void renderScene() {

    depthMapShader.useShaderProgram();

    glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"), 1, GL_FALSE, glm::value_ptr(computeLightSpaceTrMatrix()));

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);

    glClear(GL_DEPTH_BUFFER_BIT);

    drawObjects(depthMapShader, true);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



    if (depthMapOn) {
        glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);
        glClear(GL_COLOR_BUFFER_BIT);

        quadShader.useShaderProgram();

        //bind the depth map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMapTexture);
        glUniform1i(glGetUniformLocation(quadShader.shaderProgram, "depthMap"), 0);

        glDisable(GL_DEPTH_TEST);
        quad.Draw(quadShader);
        glEnable(GL_DEPTH_TEST);
    }
    else {
        glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);
        //render the scene

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myBasicShader.useShaderProgram();

        // render the map
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, depthMapTexture);
        glUniform1i(glGetUniformLocation(myBasicShader.shaderProgram, "shadowMap"), 3);

        glUniformMatrix4fv(glGetUniformLocation(myBasicShader.shaderProgram, "lightSpaceTrMatrix"),
            1,
            GL_FALSE,
            glm::value_ptr(computeLightSpaceTrMatrix()));

        drawObjects(myBasicShader, false);
        renderSphereLight(myBasicShader, false);
        GLuint alphaLoc = glGetUniformLocation(myBasicShader.shaderProgram, "alpha");
        glUniform1f(alphaLoc, 0.2f);
        renderWindows(myBasicShader);
        glUniform1f(alphaLoc, 1.0f);

        renderLamp(myBasicShader, false);
        if (modeSelect % 3 == 1) {
            renderSkyBox1(skyBoxShader);
        }
        else {
            renderSkyBox(skyBoxShader);
        }
       // glClear(GL_COLOR_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


}
void cleanup() {
    myWindow.Delete();
    //cleanup code for your own data
}
int main(int argc, const char* argv[]) {

    try {
        initOpenGLWindow();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    initOpenGLState();
    initModels();
    initShaders();
    initUniforms();
    initFBO();

    glCheckError();
    setWindowCallbacks();

    // application loop

   // renderScene();

    while (!glfwWindowShouldClose(myWindow.getWindow())) {
        processMovement();
        presentation();
        renderScene();
        glfwPollEvents();
        glfwSwapBuffers(myWindow.getWindow());
        glCheckError();
    }

    cleanup();

    return EXIT_SUCCESS;
}
