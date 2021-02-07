/* A simple program to show how to set up an X window for OpenGL rendering.
 * X86 compilation: gcc -o -L/usr/X11/lib   main main.c -lGL -lX11
 * X64 compilation: gcc -o -L/usr/X11/lib64 main main.c -lGL -lX11
 */
#include <iostream>

#include <GL/glew.h>
#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <vector>
#include <string>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi




#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>



/********************************** struct Vertex *********************************************************************/
struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texcoord;
};


/*******************************************************************************Vertex*********************************/
Vertex vertices[] =
        {
        //Position                               // Color                                           // Texcoords
        glm::vec3(0.0f, 0.5f, 0.f),             glm::vec3(1.f, 0.f, 0.f),   glm::vec2(0.f, 1.f),
        glm::vec3(-0.5f, -0.5f, 0.f),             glm::vec3(0.f, 1.f, 0.f), glm::vec2 (0.f, 0.f),
        glm::vec3(0.5f, - 0.5f, 0.f),             glm::vec3(0.f, 0.f, 1.f), glm::vec2 (1.f, 0.f)

        };
unsigned  nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint  indices[] = {0, 1 , 2};

unsigned nrOfIndices = sizeof(indices) / sizeof(indices);


/**********************************************************************************************************************/
// // vars for updateinput func
bool wasBpressed = false;
void updateInput(GLFWwindow *window) {

    if (glfwGetKey(window,GLFW_KEY_ESCAPE ) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && wasBpressed == false) {
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        wasBpressed = true;



    } else
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && wasBpressed == true)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        wasBpressed = false;
    }


}

/***********************************************************************************************************************/

bool loadShaders (GLuint &program) {

    char infoLog[512];
    GLint success;
    bool loadSuccess = true;

    std::string  temp = "";
    std::string src = "";

    std::ifstream infile;

    // Vertex

    infile.open("testshader.glsl");
    if (infile.is_open()) {
        while (std::getline(infile, temp))
            src += temp + "\n";

    } else
        std::cout << "Error: Shaders file not loaded " << "\n";
    infile.close();
    // creating vertex shader, this gives us an ID of a shader created in a opengl system
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar * vertSrc = src.c_str();
    int vs_string_length = src.length();
    glShaderSource(vertexShader,1, &vertSrc, &vs_string_length );
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {

        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error: Shaders failed compilation" << "\n";
        std::cout << infoLog << "\n";
        loadSuccess = false;

    }

    temp = "";
    src = "";

    // Fragment



    infile.open("fragmentshader.glsl");
    if (infile.is_open()) {
        while (std::getline(infile, temp))
            src += temp + "\n";

    } else
        std::cout << "Error: Fragment shader file not loaded " << "\n";
    infile.close();
    // creating vertex shader, this gives us an ID of a shader created in a opengl system
    GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar * fragSrc = src.c_str();
    int fs_string_length = src.length();

    glShaderSource(fragmentshader,1, &fragSrc, &fs_string_length );
    glCompileShader(fragmentshader);

    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);

    if (!success) {

        glGetShaderInfoLog(fragmentshader, 512, NULL, infoLog);
        std::cout << "Error: FragmentShaders failed compilation" << "\n";
        std::cout << infoLog << "\n";
        loadSuccess = false;

    }

    // Program

    program = glCreateProgram();
    glAttachShader(program,vertexShader);
    glAttachShader(program,fragmentshader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {

        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "Shader Program linking failed!" << "\n";
        std::cout << infoLog << "\n";
        loadSuccess = false;
    }


    // End
    glUseProgram(0);
    glDeleteShader(fragmentshader);
    glDeleteShader(vertexShader);

    return loadSuccess;
}

void framebuffer_resize_fallback(GLFWwindow * window, int fbW, int fbH) {
    glViewport(0, 0, fbW, fbH);

}

int main() {

// init glfw
    glfwInit();

// create window

    const int WindowHeight = 600;
    const int WindowWidth = 800;
    int framebufferWidth = 0;
    int framebufferHeight = 0;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);


    GLFWwindow *window = glfwCreateWindow(WindowWidth, WindowHeight, "Pierwsze okno", NULL, NULL);

    glfwSetFramebufferSizeCallback(window, framebuffer_resize_fallback);
    // commented fixed window size because i changed now resizable to true
    //glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    //glViewport(0, 0, framebufferWidth, framebufferHeight);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Error glewInit" << "\n";
        glfwTerminate();
    }

    // opengl options
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    // shader
    GLuint core_program;
    if (!loadShaders(core_program)) {
        glfwTerminate();
    }

    // VAO Buffer
    GLuint VAO;
    glCreateVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    // VBO Vertex Buffer
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO Elements buffer
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(GLvoid *) offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(GLvoid *) offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    // Texture Coordinates
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),(GLvoid *) offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(2);

    // Bind VAO

    glBindVertexArray(0);




    // main loop
    while(!glfwWindowShouldClose(window)) {
        // function for taking inputs
        glfwPollEvents();

        updateInput(window);

        //clear viewport with color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Use a shader program
        glUseProgram(core_program);
        // bind vertex array object
        glBindVertexArray(VAO);

        // Draw
        glDrawElements(GL_TRIANGLES, nrOfVertices, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);


        // end draw
        // swaping buffers from draw to display
        glfwSwapBuffers(window);
        glFlush();

    }

    // terminating
    glfwDestroyWindow(window);
    glDeleteProgram(core_program);
    glfwTerminate();
}
