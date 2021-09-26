
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
    GLFWwindow* window;
    
    if (!glfwInit())
        return -1;
    
    window = glfwCreateWindow(640, 480, "Hello Mandelbrot", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "glewInit error\n";
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
    
        glfwSwapBuffers(window);
    
        glfwPollEvents();
    }
}