#version 330 core

vec2 squareComplex(vec2 c)
{
    return vec2((c.x * c.x) - (c.y * c.y), 2.0f * c.x * c.y);
}

uniform vec2 u_offset;
uniform vec2 u_scale;
uniform int u_iterations;
uniform vec2 u_screen;

layout(location = 0) out vec4 color;

void main()
{
    vec2 coords = vec2(gl_FragCoord.x, u_screen.y - gl_FragCoord.y);

    vec2 lerp = (coords / u_scale) + u_offset;

    vec2 c = lerp;
    vec2 z = vec2(0.0f);

    int n = 0;
    while(length(z) < 2.0f && n < u_iterations)
    {
        z = squareComplex(z) + c;
        ++n;
    }

    float f = 0.1 * float(n);

    float r = 0.5f * sin(f) + 0.5f;
    float g = 0.5f * sin(f + 2.094f) + 0.5f;
    float b = 0.5f * sin(f + 4.188f) + 0.5f;

    color = vec4(r, g, b, 1.0f);
};