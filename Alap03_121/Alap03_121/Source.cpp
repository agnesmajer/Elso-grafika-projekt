#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

GLuint renderingProgram;
GLuint vao[1];

float circleX=300.0f;
float circleY=300.0f;
float velX= 0.0f;
float velY=0.0f;
float lineYOffset= 0.0f;
bool isMoving=false;
bool keyboard[1024]= {false};

float toRadians(float degrees){ 
    return degrees*3.14159f /180.0f; 
}

//shader beolvasó függvény
string readShaderSource(const char* filePath){
    string content;
    ifstream fileStream(filePath, ios::in);
    if (!fileStream.is_open()){
        return "";
    }
    string line= "";
    while (!fileStream.eof()){
        getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

GLuint createShaderProgram(){
    string vertShaderStr= readShaderSource("vertexShader.glsl");
    string fragShaderStr=readShaderSource("fragmentShader.glsl");

    const char* vertShaderSrc= vertShaderStr.c_str();
    const char* fragShaderSrc=fragShaderStr.c_str();

    GLuint vShader= glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vShader);

    GLuint fShader=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fShader);

    GLuint vfProgram=glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    return vfProgram;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key< 0 || key>=1024){
        return;
    }
    if (action==GLFW_PRESS){
        keyboard[key]= true;
    }
    else if (action==GLFW_RELEASE){
        keyboard[key]= false;
    }

    //ahhoz, hogy el kezdjen pattogni meg kell nyomni az s gombot
    if (key==GLFW_KEY_S && action==GLFW_PRESS&& !isMoving){
        float angle= toRadians(25.0f); 
        float speed= 3.5f;
        velX=cos(angle)*speed;
        velY=sin(angle)*speed;
        isMoving =true;
    }
}

void display(GLFWwindow* window, double currentTime){
    glClearColor(1.0f, 1.0f, 0.7f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);

    //szakasz mozgatása 
    if (keyboard[GLFW_KEY_UP]) lineYOffset+= 3.0f;
    if (keyboard[GLFW_KEY_DOWN]) lineYOffset-= 3.0f;

    //kör mozgása és visszapattanás 
    if (isMoving){
        circleX+= velX;
        circleY+= velY;
        if (circleX+50> 600 || circleX-50< 0) velX*=-1.0f; //ha a kör eléri a kép szélét, akkor megfordítjuk a sebesség előjelét
        if (circleY+50> 600 || circleY-50< 0) velY*=-1.0f;
    }

    float currentLineY=300.0f+lineYOffset;
    //megnézzük, hogy a kör középpontja és a kék szakasz függőlegesen elég közel vannnak e egymáshoz
    bool intersecting=(abs(circleY-currentLineY)<50.0f && circleX>150.0f && circleX<450.0f);

    glUniform2f(glGetUniformLocation(renderingProgram, "u_circlePos"), circleX, circleY);
    glUniform1f(glGetUniformLocation(renderingProgram, "u_lineYOffset"), lineYOffset);
    glUniform1i(glGetUniformLocation(renderingProgram, "u_isIntersecting"), (int)intersecting);

    glPointSize(600.0f);
    glDrawArrays(GL_POINTS, 0, 1);
}

int main(void){
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(600, 600, "Elso beadando", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glewInit();

    renderingProgram=createShaderProgram();
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    while (!glfwWindowShouldClose(window)){
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}