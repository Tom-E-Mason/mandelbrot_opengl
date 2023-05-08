
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
        -1.0f,  1.0f,
         1.0f,  1.0f,
        -1.0f, -1.0,
         1.0f, -1.0f
    };

    enum
    {
        POSITION_VB,
        NUM_BUFFERS,
    };

    uint32_t vertex_array_obj;
    glGenVertexArrays(1, &vertex_array_obj);
    glBindVertexArray(vertex_array_obj);

    unsigned int buffers[NUM_BUFFERS];
    glGenBuffers(NUM_BUFFERS, buffers);

    glGenBuffers(NUM_BUFFERS, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

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
    int iterations = 2048;
    glUniform1i(u_iterations, iterations);

    int u_screen = glGetUniformLocation(program, "u_screen");
    glUniform2f(u_screen, (float)screen_width, (float)screen_height);

    double mouse_x = 0, mouse_y = 0;
    double prev_mouse_x = -1, prev_mouse_y;

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
        const auto world_mouse_x_before_zoom = mouse_x / scalex + offsetx;
        const auto world_mouse_y_before_zoom = mouse_y / scaley + offsety;

        const auto i_state = glfwGetKey(window, GLFW_KEY_I);
        if (i_state == GLFW_PRESS)
        {
            scalex *= 1.1;
            scaley *= 1.1;
            std::cout << "Zooming in, scale = " << scalex << ", " << scaley << '\n';
        }

        const auto o_state = glfwGetKey(window, GLFW_KEY_O);
        if (o_state == GLFW_PRESS)
        {
            scalex /= 1.1;
            scaley /= 1.1;
            std::cout << "Zooming out, scale = " << scalex << ", " << scaley << '\n';
        }

        const auto world_mouse_x_after_zoom = mouse_x / scalex + offsetx;
        const auto world_mouse_y_after_zoom = mouse_y / scaley + offsety;

        offsetx -= world_mouse_x_after_zoom - world_mouse_x_before_zoom;
        offsety -= world_mouse_y_after_zoom - world_mouse_y_before_zoom;

        const auto mouse_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
        if (mouse_state == GLFW_PRESS)
        {
            if (prev_mouse_x != -1)
            {
                offsetx -= (mouse_x - prev_mouse_x) / scalex;
                offsety -= (mouse_y - prev_mouse_y) / scaley;
            }

            prev_mouse_x = mouse_x;
            prev_mouse_y = mouse_y;
        }
        else
        {
            prev_mouse_x = -1;
        }

        prev_mouse_x = mouse_x;
        prev_mouse_y = mouse_y;
    }
}
