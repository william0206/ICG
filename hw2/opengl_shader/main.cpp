
#include <iostream>
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <FreeImage.h>
#include <math.h>
#include "SOIL2/SOIL2.h"

#define PI 3.14159265358

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other includes
#include "Shader.h"

// Window dimensions
const GLuint WIDTH = 600, HEIGHT = 600;
GLuint texture;
GLuint VBO, VAO;
int number_of_vertices=360*(180+1)*2;

struct v {
    GLfloat position[3];
    GLfloat texcoord[2];
};
double angle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2){
    double dot= x1*x2+y1*y2+z1*z2;
    double length= sqrt(x1*x1+y1*y1+z1*z1)*sqrt(x2*x2+y2*y2+z2*z2);
    //double length= x2*x2;
    
    return acos(dot/length);
}
void textureInit() {
    glEnable(GL_TEXTURE_2D);
        FIBITMAP *pIimage = FreeImage_Load(FreeImage_GetFileType("resources/earth_texture_map.jpg", 0), "resources/earth_texture_map.jpg");
        FIBITMAP *p32BitsImage = FreeImage_ConvertTo32Bits(pIimage);
        int iWidth = FreeImage_GetWidth(pIimage);
        int iHeight = FreeImage_GetHeight(pIimage);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, iWidth, iHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void *)FreeImage_GetBits(p32BitsImage));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
        glBindTexture(GL_TEXTURE_2D, 0);

}
void drawearth(v *vertices, int number_of_vertices){
    int slice=360, stack=180;
    //double x, y, z;
    double slice_step = 2 * PI / slice, stack_step = PI / stack;
    int count=0;
    for (int i = 0; i < slice; i++) {
        for (int j = 0; j < (stack + 1); j++) {
            vertices[count].position[0]= sin(j * stack_step) * cos(i*slice_step);
            vertices[count].position[1]= cos(j * stack_step);
            vertices[count].position[2]= sin(j * stack_step) * sin(i*slice_step);
            GLfloat x1=vertices[count].position[0];
            GLfloat y1=vertices[count].position[1];
            GLfloat z1=vertices[count].position[2];
            if(z1<0)
                vertices[count].texcoord[0]= angle(x1, 0, z1, sqrt(1-y1*y1), 0, 0)/2/PI;
            else
                vertices[count].texcoord[0]= 1-angle(x1, 0, z1, sqrt(1-y1*y1), 0, 0)/2/PI;
            vertices[count].texcoord[1]= (y1+1)/2;
            
            count++;
            
            vertices[count].position[0]= sin(j * stack_step) * cos((i + 1)*slice_step);
            vertices[count].position[1]= cos(j * stack_step);
            vertices[count].position[2]= sin(j * stack_step) * sin((i + 1)*slice_step);
            GLfloat x2=vertices[count].position[0];
            GLfloat y2=vertices[count].position[1];
            GLfloat z2=vertices[count].position[2];
            if(z2<0)
                vertices[count].texcoord[0]= angle(x2, 0, z2, sqrt(1-y2*y2), 0, 0)/2/PI;
            else
                vertices[count].texcoord[0]= 1-angle(x2, 0, z2, sqrt(1-y2*y2), 0, 0)/2/PI;
            vertices[count].texcoord[1]= (y2+1)/2;
            
            count++;
        }
    }
}
void shaderinit(int number_of_vertices, v *vertices){
    
    glGenVertexArrays( 1, &VAO );
    glGenBuffers( 1, &VBO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray( VAO );
    //draw sphere
    drawearth(vertices, number_of_vertices);
    
    // Set up vertex data (and buffer(s)) and attribute pointers
    glBufferData( GL_ARRAY_BUFFER, sizeof(v)*number_of_vertices, vertices, GL_STATIC_DRAW );
    
    // Position attribute
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( v ), ( void * ) 0 );
    
    //texture attribute
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( v ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 ); // Unbind VAO
}
// The MAIN function, from here we start the application and run the game loop
int main( )
{
    // Init GLFW*****************************************************************************************************************
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr );
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    
    if ( nullptr == window ){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent( window );
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) ){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    //***************************************************************************************************************************
    // Define the viewport dimensions
    glViewport( 0, 0, screenWidth, screenHeight );
    //
    glEnable(GL_DEPTH_TEST);
    //
    // Build and compile our shader program
    Shader ourShader( "resources/shaders/core.vs", "resources/shaders/core.frag" );
    //
    
    v *vertices= new v[number_of_vertices];
    //setup shader
    shaderinit(number_of_vertices, vertices);
    //
    //setup texture
    textureInit();
    //
    // Game loop
    while ( !glfwWindowShouldClose( window ) )
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents( );
        
        // Render
        // Clear the colorbuffer
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //
        ourShader.Use( );
        //
        //set up MVP
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (GLfloat)512  / (GLfloat)512, 1.0f, 500.0f);

        glm::mat4 View = glm::lookAt(
        glm::vec3(0,0,3), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
        glm::mat4 Model = glm::mat4(1.0f);
        Model = glm::rotate(Model,(GLfloat)glfwGetTime()*1.0f, glm::vec3(0.0f,1.0f,0.0f));
        glm::mat4 mvp = Projection * View * Model;
        
        
        GLuint MatrixID = glGetUniformLocation(ourShader.Program, "MVP");
        GLuint TextureID = glGetUniformLocation(ourShader.Program, "TEXTURE");
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(TextureID, 0);
        
        glBindVertexArray( VAO );
        glDrawArrays( GL_TRIANGLE_STRIP, 0, number_of_vertices);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, NULL);
        // Swap the screen buffers
        glfwSwapBuffers( window );
    }
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 1, &VAO );
    glDeleteBuffers( 1, &VBO );
    delete [] vertices;
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate( );
    
    return EXIT_SUCCESS;
}

