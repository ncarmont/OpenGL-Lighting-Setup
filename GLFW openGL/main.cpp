#include <iostream>

//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

#include "SOIL2/SOIL2.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    
    glEnable( GL_BLEND);
    
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    

    // Build and compile our shader program
    Shader ourShader ("resources/shaders/core.vs", "resources/shaders/core.frag");
    
    GLfloat vertices[]=
    {
         // position             // color           // texture coordinates
       0.5f, 0.32f, 0.0f,         1.0f, 0.0f,0.0f,   1.0f, 1.0f,  // Top Right
       0.5f, -0.32f, 0.0f,        1.0f, 1.0f, 1.0f,  1.0f, 0.0f,  // Bottom Right
      -0.5f, -0.32f, 0.0f,        1.0f, 0.0f, 0.0f,  0.0f, 0.0f,   // Bottom Left
        -0.5f, 0.32f,0.0f,        1.0f, 0.0f, 1.0f,  0.0f, 1.0f,   // Top Left
    };
    
    GLuint indices[] = {
        0, 1, 3, // First Triangle
        1, 2, 3, // Second Triangle
        
    };
    
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers( 1, &EBO );
    
    // Bind the Vertex Array Objectv first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices, GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *) 0 );
    glEnableVertexAttribArray(0);
    
    // Color Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *) (3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
//    Texture coordinates attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *) (6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0); //unbind VAO
    
    GLuint texture;
    int width, height;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D, texture );
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    unsigned char *image = SOIL_load_image("resources/images/elon.png", &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
    // Game Loop
    while( !glfwWindowShouldClose(window) ){
        
        // Check if any events have been activated (key pressed, mouse, etc).
        glfwPollEvents();
        
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear( GL_COLOR_BUFFER_BIT);
        
        // Draw the triangle
        ourShader.Use();
        
        
        glm::mat4 transform(1);
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (GLfloat)glfwGetTime()*-5.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        
        GLint transformLocation = glGetUniformLocation( ourShader.Program,  "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
        
        
        glActiveTexture( GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(ourShader.Program,"ourTexture"),0);
        
        
        glBindVertexArray( VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    // Termionate GLFW, clearing any resources allocated to GLFW
    glfwTerminate();
    return 0;
}
