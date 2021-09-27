
#include <iostream>

#include "opengl-util.h"

static auto create_program(const shader& vertex_shader, const shader& fragment_shader)
{
    auto program = glCreateProgram();

    glAttachShader(program, vertex_shader.id());
    glAttachShader(program, fragment_shader.id());

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertex_shader.id());
    glDeleteShader(fragment_shader.id());

    return program;
}

int main()
{
    GLFWwindow* window;
    
    if (!glfwInit())
        return -1;
    
    int screen_width = 640;
    int screen_height = 480;
    window = glfwCreateWindow(screen_width, screen_height, "Hello Mandelbrot", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "glewInit error\n";

    float positions[] =
    {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,
        -1.0f,  1.0f,
    };

    uint32_t indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

    unsigned int index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(float), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    auto vertex_shader = shader("shaders/v.glsl", GL_VERTEX_SHADER);
    auto fragment_shader = shader("shaders/fragment_shader.glsl", GL_FRAGMENT_SHADER);

    const auto program = create_program(vertex_shader, fragment_shader);
    glUseProgram(program);

    int u_offset = glGetUniformLocation(program, "u_offset");
    float offsetx = 0.0f, offsety = 0.0f;
    glUniform2f(u_offset, offsetx, offsety);

    int u_scale = glGetUniformLocation(program, "u_scale");
    float scalex = (float)screen_width * 2, scaley = (float)screen_height;
    glUniform2f(u_scale, scalex, scaley);

    int u_iterations = glGetUniformLocation(program, "u_iterations");
    int iterations = 4096;
    glUniform1d(u_iterations, iterations);

    int u_screen = glGetUniformLocation(program, "u_iterations");
    glUniform2f(u_screen, (float)screen_width, (float)screen_height);
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    
        glfwSwapBuffers(window);
    
        glfwPollEvents();
    }
}