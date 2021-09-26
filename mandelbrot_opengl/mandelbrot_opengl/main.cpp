
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static auto compile_shader(const std::string& source, unsigned int type)
{
    auto id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        std::string message;
        message.reserve(length);

        glGetShaderInfoLog(id, length, &length, message.data());

        std::cout << message << '\n';
    }

    return id;
}

static auto create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
    auto program = glCreateProgram();
    auto vs = compile_shader(vertex_shader, GL_VERTEX_SHADER);
    auto fs = compile_shader(fragment_shader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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

    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    std::string vertex_shader =
        "\n"
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = position;\n"
        "}\n";

    std::string fragment_shader =
        "\n"
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 colour;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    colour = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    auto shader = create_shader(vertex_shader, fragment_shader);
    glUseProgram(shader);
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    
        glfwSwapBuffers(window);
    
        glfwPollEvents();
    }
}