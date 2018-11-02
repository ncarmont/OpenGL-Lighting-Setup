#include <iostream>

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"

const GLint WIDTH = 800, HEIGHT = 600;

int main(){
    // Init GLFW
    glfwInit();
    
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE);
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "My Learn OpenGL Ligthing Setup", nullptr, nullptr);
    int screenWidth, screenHeight;
    
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    if( nullptr == window){
        std::cout<< "Failed to createa GLFW window"<< std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    
    if( GLEW_OK != glewInit()){
        std::cout<< "Failed to initialise GLEW" << std::endl;
        return -1;
    }
    // Define the viewport dimensions
    glViewport(0,0, screenWidth, screenHeight);
    
    Shader ourShader ("resources/shaders/core.vs", "resources/shaders/core.frag");
    
    
    
    GLfloat vertices[]=
    {
         // position             // color
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,  // bottom left
        0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,  // bottom right
        0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f,  // middle top
    };
    
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid *) 0 );
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid *) (3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    
    while( !glfwWindowShouldClose(window) ){
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear( GL_COLOR_BUFFER_BIT);
        
        ourShader.Use();
        glBindVertexArray( VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    // Termionate GLFW, clearing any resources allocated to GLFW
    glfwTerminate();
    return 0;
}
