/** graphic libs **/
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/** C++ libs **/
#include <iostream>
/** C lib **/
#include <stdlib.h>
#include <math.h>
#define MANDELBROT_SET 0

static float vertexes[8000 * 4000];
static double depth = 0.6;
static int width, height;
static GLuint vertex_VBO;


GLuint create_shader_program(const char * vertex_file_name, const char * fragment_file_name);
static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
static void change_window_size(GLFWwindow * window)
{
    glfwGetFramebufferSize(window, &width, &height);
    for(int i = 0; i < height; i++)
    for(int j = 0; j < width; j++)
    {
        vertexes[2*i*width+j*2]   = -1 + 2*j/(double)width;
        vertexes[2*i*width+j*2+1] = -1 + 2*i/(double)height;
    }
    glViewport(0, 0, width, height);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, 2*width*height*sizeof(vertexes[0]), vertexes, GL_STATIC_DRAW);
}
static void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    for(int i = 0; i < height; i++)
    for(int j = 0; j < width; j++)
    {
        vertexes[2*i*width+j*2]   = -1 + 2*j/(double)width;
        vertexes[2*i*width+j*2+1] = -1 + 2*i/(double)height;
    }
    glViewport(0, 0, width, height);
    change_window_size(window);
}
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(yoffset > 0) depth -= depth/40.0L;
    else depth += depth/40.0L;
}
static GLuint load_texture(const char * filename)
{
    GLuint texture;
    int width, height, cnt;
    unsigned char * data = stbi_load(filename, &width, &height, &cnt, 0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, cnt == 3 ? GL_RGB : GL_RGBA,
                                    width, height, 0,
                                    cnt == 3 ? GL_RGB : GL_RGBA,
                                    GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    return texture;
}
int main(int n, char ** v)
{
    if(!glfwInit()) exit(EXIT_FAILURE);
    static GLFWwindow * window = glfwCreateWindow(1280, 720, "My Title", NULL, NULL);
    if(!window) { glfwTerminate(); exit(EXIT_FAILURE); }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);
    glGenBuffers(1, &vertex_VBO);

    static GLuint program = create_shader_program(NULL, v[1]);
    static GLuint texture = load_texture("tex/Gradient.png");
    static double position_x = 0, position_y = 0;
    static double Re = -0.7269;
    static double Im = 0.1889;

    change_window_size(window);
    glUseProgram(program);
    glEnableClientState(GL_VERTEX_ARRAY);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glVertexPointer(2, GL_FLOAT, 0, NULL);

    while (!glfwWindowShouldClose(window))
    {
        //depth = 1/((std::pow(glfwGetTime(), 2.1) + 10)/10);
        Re = 0.7885*std::cos(glfwGetTime()/5);
        Im = 0.7885*std::sin(glfwGetTime()/5);

        int u_resolution= glGetUniformLocation(program, "u_resolution"  );
        int u_tex       = glGetUniformLocation(program, "u_tex"         );
        int u_depth     = glGetUniformLocation(program, "u_depth"       );
        int u_position  = glGetUniformLocation(program, "u_position"    );
        int u_Re        = glGetUniformLocation(program, "u_Re"          );
        int u_Im        = glGetUniformLocation(program, "u_Im"          );

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position_y+=65*depth;
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position_y-=65*depth;
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position_x-=65*depth;
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position_x+=65*depth;

        glUniform1i(u_tex,          0                               );
        glUniform2f(u_resolution,   (float)width, (float)height     );
        glUniform1f(u_depth,        (float)depth                    );
        glUniform2f(u_position,     (float)position_x/depth,
                                    (float)position_y/depth         );
        glUniform1f(u_Re,           (float)Re                       );
        glUniform1f(u_Im,           (float)Im                       );


        glDrawArrays(GL_POINTS, 0, width*height);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
