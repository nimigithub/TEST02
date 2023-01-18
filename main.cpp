#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
};
 
static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
 
static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";
 
static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static bool isFullScreen = false;
static int prev_x, prev_y, prev_width, prev_height;

void toggleFullScreen(GLFWwindow* window) {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (!isFullScreen) {
        glfwGetWindowPos(window, &prev_x, &prev_y);
        glfwGetWindowSize(window, &prev_width, &prev_height);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        isFullScreen = true;
    }
    else {
        glfwSetWindowMonitor(window, NULL, prev_x, prev_y, prev_width, prev_height, GLFW_DONT_CARE);
        isFullScreen = false;
    }
}

 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    else {
        //std::cout << "key: " << key << " scancode: " << scancode << " action: " << action << " mods: " << mods << std::endl;        
    }

    if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        toggleFullScreen(window);
    }
}

void thread_func_f2()
{
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;
 
    glfwSetErrorCallback(error_callback);
 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_SAMPLES, 4);
 
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        std::cout << "glfwCreateWindow() failed" << std::endl;
        return;
    }
 
    glfwSetKeyCallback(window, key_callback);
 
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
    }

    //gladLoadGL(glfwGetProcAddress);

    glfwSwapInterval(1);
 
    // NOTE: OpenGL error checks have been omitted for brevity
 
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
 
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
 
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
 
    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");
 
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) (sizeof(float) * 2));

    glEnable(GL_MULTISAMPLE);
 
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        mat4x4 m, p, mvp;
 
        glfwGetFramebufferSize(window, &width, &height); 
        ratio = width / (float) height;
 
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
 
        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, (float) glfwGetTime() * 0.5f);
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);
 
        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);
 
        glfwSwapBuffers(window); 
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
}

void thread_func_f1()
{
    std::string windowName = "Lenna";
    std::cout << "Running in thread" << std::endl;
    using namespace cv;
    Mat image = imread("./Res/Lenna.png", 1);
    Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(1920, 1080));
    bool bFullScreen = false;

    while (true) {
        if (bFullScreen)
            imshow(windowName.c_str(), resizedImage); // Show our image inside the created window.
        else
            imshow(windowName.c_str(), image); // Show our image inside the created window.

        int key = cv::waitKey(16); // Wait for any keystroke in the window
        if (key == 27) {
            break;
        }
        else {
            if (key == 'A') {
                std::cout << "Hi A" << std::endl;
            }
            if (key == 'F') {
                cv::setWindowProperty(windowName.c_str(), cv::WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
                std::cout << "Enter Full Screen" << std::endl;
                bFullScreen = true;
            }
            if (key == 'f') {
                cv::setWindowProperty(windowName.c_str(), cv::WND_PROP_FULLSCREEN, WINDOW_NORMAL);
                std::cout << "Exit Full Screen" << std::endl;
                bFullScreen = false;
            }
        }
    }
    cv::destroyAllWindows(); //destroy all window
}
 
int main(void)
{
    if (!glfwInit()) { // call once for the whole program
        std::cout << "glfwInit() failed" << std::endl;
        return -1;
    }

    std::thread t1(thread_func_f1);
    std::thread t2(thread_func_f2);

    t1.join();
    t2.join();

    glfwTerminate(); // same - terminate once

    return 0;
}
