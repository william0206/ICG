
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
#include "Shader.h"
using namespace std;

// Window dimensions
const GLuint WIDTH = 600, HEIGHT = 600;
//GLuint texture[2];
GLuint VBO[2], VAO[2];
//int number_of_vertices=360*(180+1)*2;
int number_of_vertices=20*(10+1)*2;

struct v {
    GLfloat position[3];
    GLfloat color[3];
    GLfloat normal[3];
};

float Kd=0.0f, Ks=0.0f;
bool flag= true;

void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mods);

void drawearth(double r,v *vertices, int number_of_vertices, int slice, int stack, GLfloat R, GLfloat G, GLfloat B){
    
    double slice_step = 2 * PI / slice, stack_step = PI / stack;
    int count=0;
    for (int i = 0; i < slice; i++) {
        for (int j = 0; j < (stack + 1); j++) {
            vertices[count].position[0]= r*sin(j * stack_step) * cos(i*slice_step);
            vertices[count].position[1]= r*cos(j * stack_step);
            vertices[count].position[2]= r*sin(j * stack_step) * sin(i*slice_step);
            vertices[count].color[0]= R;
            vertices[count].color[1]= G;
            vertices[count].color[2]= B;
            vertices[count].normal[0]= vertices[count].position[0];
            vertices[count].normal[1]= vertices[count].position[1];
            vertices[count].normal[2]= vertices[count].position[2];
            count++;
            
            vertices[count].position[0]= r*sin(j * stack_step) * cos((i + 1)*slice_step);
            vertices[count].position[1]= r*cos(j * stack_step);
            vertices[count].position[2]= r*sin(j * stack_step) * sin((i + 1)*slice_step);
            vertices[count].color[0]= R;
            vertices[count].color[1]= G;
            vertices[count].color[2]= B;
            vertices[count].normal[0]= vertices[count].position[0];
            vertices[count].normal[1]= vertices[count].position[1];
            vertices[count].normal[2]= vertices[count].position[2];
            count++;
        }
    }
}
void shaderinit(int number_of_vertices, v *vertices){//VAO? VBO?
    
    glGenVertexArrays( 2, VAO );
    glGenBuffers( 2, VBO );
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray( VAO[0] );
    glBindBuffer( GL_ARRAY_BUFFER, VBO[0] );
    //draw sphere
    //drawearth(1, vertices, number_of_vertices, 360, 180, 0.5f, 0.8f, 0.9f);
    drawearth(1, vertices, number_of_vertices, 20, 10, 0.5f, 0.8f, 0.9f);
    // Set up vertex data (and buffer(s)) and attribute pointers
    glBufferData( GL_ARRAY_BUFFER, sizeof(v)*number_of_vertices, vertices, GL_STATIC_DRAW );
    
    // Position attribute
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( v ), ( void * ) 0 );
    //color attribute
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( v ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
    //normal attrubute
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, sizeof( v ), ( GLvoid * )( 6 * sizeof( GLfloat ) ) );
    //add second VAO
    
    glBindVertexArray( VAO[1] );
    glBindBuffer( GL_ARRAY_BUFFER, VBO[1] );
    //drawearth(0.05, vertices, number_of_vertices, 360, 180, 0.4f, 0.5f, 0.0f);
    drawearth(0.05, vertices, number_of_vertices, 20, 10, 0.4f, 0.5f, 0.0f);
    glBufferData( GL_ARRAY_BUFFER, sizeof(v)*number_of_vertices, vertices, GL_STATIC_DRAW );
    
    // Position attribute
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( v ), ( void * ) 0 );
    //color attribute
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( v ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
    //normal attrubute
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, sizeof( v ), ( GLvoid * )( 6 * sizeof( GLfloat ) ) );
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 ); // Unbind VAO
}
// The MAIN function, from here we start the application and run the game loop
int main( )
{
    // Init GLFW**************************************************************************
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr );
    
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    if ( nullptr == window ){
        cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent( window );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) ){
        cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    //***************************************************************************************
    // Define the viewport dimensions
    glViewport( 0, 0, screenWidth, screenHeight );
    //depth
    glEnable(GL_DEPTH_TEST);
    // Build and compile our shader program
    //Shader ourShader( "resources/shaders/core_g.vs", "resources/shaders/core_g.frag" );
    
    v *vertices= new v[number_of_vertices];
    //setup shader
    shaderinit(number_of_vertices, vertices);
    //setup texture
    //textureInit();
    //
    
    // Game loop
    while ( !glfwWindowShouldClose( window ) )
    {
        
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents( );
        
        Shader ourShader( "resources/shaders/core.vs", "resources/shaders/core.frag", "resources/shaders/core_g.vs", "resources/shaders/core_g.frag", flag);
        
        
        // Render
        // Clear the colorbuffer
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //
        ourShader.Use();
        //set phong parameters
        GLuint kd = glGetUniformLocation(ourShader.Program, "kd");
        glUniform1f(kd, Kd);
        GLuint ks = glGetUniformLocation(ourShader.Program, "ks");
        glUniform1f(ks, Ks);
        //
        //setup lightpos & viewpos
        GLuint lightPos = glGetUniformLocation(ourShader.Program, "lightPos");
        GLuint viewPos = glGetUniformLocation(ourShader.Program, "viewPos");
        glUniform3f(lightPos, 1.1*1.1, 1.0*1.1, 1.3*1.1);
        glUniform3f(viewPos, 0.0 ,0.0 ,5.6);
        //setup MVP
        GLuint model =glGetUniformLocation(ourShader.Program, "model");
        GLuint view = glGetUniformLocation(ourShader.Program, "view");
        GLuint proj = glGetUniformLocation(ourShader.Program, "proj");
        //GLuint TextureID = glGetUniformLocation(ourShader.Program, "TEXTURE");//0??
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (GLfloat)512  / (GLfloat)512, 1.0f, 500.0f);

        glm::mat4 View = glm::lookAt(
        glm::vec3(0,0,5.6), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
        
        {
        glm::mat4 Model = glm::mat4(1.0f);
        //Model = glm::rotate(Model,(GLfloat)glfwGetTime()*1.0f, glm::vec3(0.0f,1.0f,0.0f));
        
        glUniformMatrix4fv(model, 1, GL_FALSE, &Model[0][0]);
        glUniformMatrix4fv(view, 1, GL_FALSE, &View[0][0]);
        glUniformMatrix4fv(proj, 1, GL_FALSE, &Projection[0][0]);
        
        
        //do drawing**********************************************
        
        glBindVertexArray( VAO[0] );
        glDrawArrays( GL_TRIANGLE_STRIP, 0, number_of_vertices);
        }
        {
        //second object
        glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::translate(Model, glm::vec3(1.1f, 1.0f, 1.3f));
        
        glUniformMatrix4fv(model, 1, GL_FALSE, &Model[0][0]);
        glUniformMatrix4fv(view, 1, GL_FALSE, &View[0][0]);
        glUniformMatrix4fv(proj, 1, GL_FALSE, &Projection[0][0]);
        
        //do drawing**********************************************
        glBindVertexArray( VAO[1] );
        glDrawArrays( GL_TRIANGLE_STRIP, 0, number_of_vertices);
        
        }
        glBindVertexArray(0);
        //********************************************************
        //glBindTexture(GL_TEXTURE_2D, NULL);
        // Swap the screen buffers
        glfwSwapBuffers( window );
    }
    
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays( 2, VAO );
    glDeleteBuffers( 2, VBO );
    delete [] vertices;
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate( );
    
    return EXIT_SUCCESS;
}

void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mods ){
    //cout<<key<<endl;
    if(key == GLFW_KEY_B){
        if(action == GLFW_PRESS)
            flag=!flag;
    }else if(key == GLFW_KEY_1){
        if(action == GLFW_PRESS){
            if(Ks>0.1f)
                Ks-=0.1f;
        }
    }else if(key == GLFW_KEY_2){
        if(action == GLFW_PRESS){
            if(Ks<1.0f)
                Ks+=0.1f;
        }
    }else if(key == GLFW_KEY_3){
        if(action == GLFW_PRESS){
            if(Kd>0.1f)
                Kd-=0.1f;
        }
    }else if(key == GLFW_KEY_4){
        if(action == GLFW_PRESS){
            if(Kd<1.0f)
                Kd+=0.1f;
        }
    }else{
        //do nothing
    }
}
