
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
    
    int screen_width = 1280;
    int screen_height = 720;
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
        -1.0f,  1.0f, 0.0f, // tl
         1.0f,  1.0f, 0.0f, // tr
        -1.0f, -1.0,  0.0f, // bl
         1.0f, -1.0f, 0.0f  // br
    };

    float tex_coords[] =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
    };

    enum
    {
        POSITION_VB,
        TEXCOORD_VB,
        NUM_BUFFERS,
    };

    uint32_t vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
    glBindVertexArray(vertex_array_obj);

    unsigned int buffers[NUM_BUFFERS];
    glGenBuffers(NUM_BUFFERS, buffers);

    glGenBuffers(2, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), tex_coords, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);

    auto vertex_shader = shader("shaders/v.glsl", GL_VERTEX_SHADER);
    auto fragment_shader = shader("shaders/fragment_shader.glsl", GL_FRAGMENT_SHADER);

    const auto program = create_program(vertex_shader, fragment_shader);
    glUseProgram(program);

    int u_offset = glGetUniformLocation(program, "u_offset");
    float offsetx = 0.0f, offsety = 0.0f;
    glUniform2f(u_offset, offsetx, offsety);

    int u_scale = glGetUniformLocation(program, "u_scale");
    float scalex = (float)screen_width / 2, scaley = (float)screen_height;
    glUniform2f(u_scale, scalex, scaley);

    int u_iterations = glGetUniformLocation(program, "u_iterations");
    if (u_iterations == -1) {
        return 0;
    }
    int iterations = 4096;
    glUniform1i(u_iterations, iterations);

    int u_screen = glGetUniformLocation(program, "u_screen");
    glUniform2f(u_screen, (float)screen_width, (float)screen_height);

    double mouse_x = 0, mouse_y = 0;
    double prev_mouse_x = -1, prev_mouse_y = 0;

    bool i_down = false;
    bool o_down = false;
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform2f(u_offset, offsetx, offsety);
        glUniform2f(u_scale, scalex, scaley);

        glBindVertexArray(vertex_array_obj);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    
        glfwPollEvents();

        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        auto i_state = glfwGetKey(window, GLFW_KEY_I);
        if (i_state == GLFW_PRESS)
        {
            i_down = true;
        }
        else if (i_down)
        {
            i_down = false;
            scalex *= 1.5;
            scaley *= 1.5;
            std::cout << "Zooming in, scale = " << scalex << ", " << scaley << '\n';
        }

        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        auto o_state = glfwGetKey(window, GLFW_KEY_O);
        if (o_state == GLFW_PRESS)
        {
            o_down = true;
        }
        else if (o_down)
        {
            o_down = false;
            scalex /= 1.5;
            scaley /= 1.5;
            std::cout << "Zooming out, scale = " << scalex << ", " << scaley << '\n';
        }

        /*if (prev_mouse_x != -1) {
            offsetx += mouse_x - prev_mouse_x;
            offsety += mouse_y - prev_mouse_y;
        }*/

        prev_mouse_x = mouse_x;
        prev_mouse_y = mouse_y;
    }
}