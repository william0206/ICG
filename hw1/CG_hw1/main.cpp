//
//  main.cpp
//  CG_hw1
//
//  Created by william chen on 2019/9/30.
//  Copyright © 2019 william chen. All rights reserved.
//
//Your Student ID is 0613457. (Modify it)

#ifdef __APPLE__//for macox
    #include <OpenGL/gl.h>
    #include <GLUT/glut.h>
#else//for Windows
    #include "glut.h"
    #define M_PI 3.14159265358979323846
#endif

#include <iostream>
#include "math.h"
using namespace std;

void display();
void reshape(int _width, int _height);
void keyboard(unsigned char key, int x, int y);
void idle();
void drawSphere(double r, int lats, int longs,float R,float G,float B);//draw sphere and set color
void lighting();
void setcolor(float R,float G,float B);//setcolor function
void setRevolution(float r, float degree);//set revolution matrix

int width = 400, height = 400;
float X = 1; //default degree value, you can adjust it
GLfloat Y = 0.5; //default radius value, you can adjust it

int degree=0;//increase in idle(), from 0~359
bool oflag=true, pflag=true;//flag for rotating or stop, default set to true


void lighting()
{
    // enable lighting
    glEnable(GL_LIGHTING);
    //Add directed light
    GLfloat diffuseColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat ambientColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat position[] = { 0.0f, 10.0f, 0.0f};
    glEnable(GL_LIGHT0);                                //open light0
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);        //set diffuse color of light0
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);        //set ambient color of light0
    glLightfv(GL_LIGHT0, GL_POSITION, position);        //set position of light0
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("HW1");

    lighting();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}

void display()
{
    X=degree*365;//speed up the rotation
    // TO DO: Set the MVP transform
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f,30.0f,50.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, width/(GLfloat)height, 0.1, 1000);
    
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_CULL_FACE);        //remove back face
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);        //normalized normal
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor4f(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    // TO DO: Draw the Sun, Earth, Moon
    //draw Sun
    drawSphere(7*Y, 240, 60, 1, 0.2, 0.2);
    //draw Earth or Octahedron
    //earth
    glPushMatrix();
        setRevolution(18, X);//繞太陽公轉
        glRotatef(-23.5, 0, 0, 1);//傾斜23.5度
        glRotatef(X, 0, 1, 0);//設定自轉
        if(oflag==true){
            drawSphere(2*Y, 180, 360, 0.2, 0.5, 0.8);//function for drawing earth
        }else{
            drawSphere(2*Y, 2, 4, 0.2, 0.5, 0.8);//function for drawing octahedron
        }
    glPopMatrix();
    //draw Moon
    glPushMatrix();
        setRevolution(18, X);//跟著地球對太陽公轉
        glRotatef(X/28, 0, 1, 0);//繞地球公轉的半徑旋轉
        glTranslatef(0.0, 0.0f, -3.0f);//推出去到繞地球公轉的半徑
        glRotatef(X/28, 0, 1, 0);//月球自轉
        drawSphere(0.5, 240, 60, 0.2, 1, 0.2);
    glPopMatrix();
    
    //draw Cylinder
    glPushMatrix();
        setRevolution(18, X);//跟著地球對太陽公轉
        glRotatef(-23.5, 0, 0, 1);//設定軸的傾斜角度
        glRotatef(90, 1, 0, 0);//將軸轉到y軸上
        glTranslatef(0.0, 0.0f, -8*Y/2);//把軸移到畫面中間
        //functions for drawing cylinder
        GLUquadricObj *quadratic = gluNewQuadric();
        gluCylinder(quadratic,0.1f,0.1f,8*Y,32,32);
    glPopMatrix();
    
    glutSwapBuffers();
}
void reshape(int _width, int _height) {
    width = _width;
    height = _height;
}

void keyboard(unsigned char key, int x, int y) {
    // TO DO: Set the keyboard control
    switch (key) {
        case 'o':
            oflag=(!oflag);
            break;
        case 'O':
            oflag=(!oflag);
            break;
        case 'p':
            pflag=(!pflag);
            break;
        case 'P':
            pflag=(!pflag);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void idle() {
    if(pflag==true){
        degree=(degree+1)%360;//讓角度持續增加並循環
    }
    glutPostRedisplay();
}

void drawSphere(double r, int stack, int slice, float R,float G,float B) {
       for(int i = 0; i <= stack; i++){
          double phy = M_PI * ( 0.5 + (double)(i - 1)/stack ) ;
          double z0=sin(phy), zr0=cos(phy);
          double phy1= M_PI * ( 0.5 + (double)i/stack );
          double z1=sin(phy1), zr1=cos(phy1);
           
          setcolor(R, G, B);
          
          glBegin(GL_QUAD_STRIP);
          for(int j = 0; j <= slice; j++){
              double theta=2 * M_PI * (double)(j - 1)/slice;
              double x=cos(theta), y=sin(theta);
              glNormal3f(x * zr0, y * zr0, z0);
              glVertex3f(r * x * zr0, r * y * zr0, r * z0);
              glNormal3f(x * zr1, y * zr1, z1);
              glVertex3f(r * x * zr1, r * y * zr1, r * z1);
          }
          glEnd();
      }
}

void setcolor(float R, float G, float B){
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(R, G, B);
}

void setRevolution(float r, float degree){
    GLdouble x = -r * sin(degree/365*M_PI/180);
    GLdouble z = -r * cos(degree/365*M_PI/180);
    glTranslatef(x, 0.0f, z);
}
