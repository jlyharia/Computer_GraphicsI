/**********************************************************************
    Spring 2012
    CS 4554 Computer Graphics I
    Assignment 2 : 3D Display System
    Student Name : YiHung Lee
    Gw ID        : G42957795
    Implement :
        1: Load geometric data from file with .d extension
        2: Translate, rotate, and scale the object by keyboard
        3: Fill polygon
        4: Two lighting effect
        5: Texture mapping
        6: Back face culling

    Instruction :
        1: press '[' or ']' to scale up or down
        2: press 'w' or 's' to rotate object along X axis
        3: press 'a' or 'd' to rotate object along Y axis
        4: press 'q' or 'e' to rotate object along Z axis
        5: press left, right,up or down to move object
        6: press 'Esc' to leave program

    Note:
        1: This system only works well in triangle polygon file
 **********************************************************************/


#include <iostream>
#include <stdlib.h>
#include <vector>
#include <GL/glut.h>
#include "loadModel.h"
#include "imageloader.h"
using namespace std;

float _scale = 1.0f;
float X_angle = -90.0f;
float Y_angle = 0.0f;
float Z_angle = 0.0f;
float _yPosition = -5.0f;
float _xPosition = 0.0f;
GLuint _textureId; //The id of the texture

vector< vector<float> > vertex;
vector< vector<int> > polygon;

void vector_subtraction(float *sub, float vector_1[3], float vector_2[3])
{
    //vector_2 - vector_1
    for(int i = 0 ; i < 3 ; i++)
        sub[i] = vector_2[i] - vector_1[i];
}
void cross_product3D(float *normal, float vector_1[3], float vector_2[3])
{//compute cross product
    normal[0] = vector_1[1]*vector_2[2] - vector_2[1]*vector_1[2];
    normal[1] = -(vector_1[0]*vector_2[2] - vector_2[0]*vector_1[2]);
    normal[2] = vector_1[0]*vector_2[1] - vector_2[0]*vector_1[1];
}
//Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27: //Escape key
			exit(0); //Exit the program
			break;
        case ']': //scale up
            _scale +=0.1;
            break;
        case '[': //scale down
            _scale -=0.1;
            break;
        case 'w': //rotate X axis
            X_angle -=1;
            break;
        case 's': //rotate X axis
            X_angle +=1;
            break;
        case 'd': //rotate Y axis
            Y_angle +=1;
            break;
        case 'a': //rotate Y axis
            Y_angle -=1;
            break;
        case 'e': //rotate Z axis
            Z_angle +=1;
            break;
        case 'q': //rotate Z axis
            Z_angle -=1;
            break;
        default:
            break;
	}
}
void ConvertArray(float array[][3] ,int num_vertex ,
                      vector< vector<float> > vertex )
{
    for(int i=0;i<num_vertex;i++)
        for(int j = 0; j<3; j++)
            array[i][j] =  vertex[i][j];
}
GLuint loadTexture(Image* image)
{
    GLuint textureId;
    glGenTextures(1, &textureId); //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                 0,                            //0 for now
                 GL_RGB,                       //Format OpenGL uses for image
                 image->width, image->height,  //Width and height
                 0,                            //The border of the image
                 GL_RGB, //GL_RGB, because pixels are stored in RGB format
                 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                 //as unsigned numbers
                 image->pixels);               //The actual pixel data
    return textureId; //Returns the id of the texture
}
//Initializes 3D rendering
void initRendering()
{
	glClearColor(0.7f, 0.9f, 1.0f, 1.0f); //Change the background to sky blue
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_CULL_FACE); //Enable back face culling
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1); //Enable light #1
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading
	Image* image = loadBMP("1213316590-10.bmp");
    _textureId = loadTexture(image);
    delete image;
}

//Called when the window is resized
void handleResize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0, (double)w / (double)h, 1.0, 200.0);
}


//Draws the 3D scene
void drawScene()
{
    float array[vertex.size()][3];
	ConvertArray(array,vertex.size(),vertex);

	/**************************************************/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective

    glColor4f(1,1,1,1);
    glTranslatef(_xPosition, _yPosition , -10.0f);
    glScalef(_scale,_scale,_scale);
    //--------------------light 0------------------------
    GLfloat ambientColor[] = {0.5f, 0.5f, 0.5f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    GLfloat lightColor0[] = {1.0f, 0.5f, 0.0f, 1.0f}; //Color

	GLfloat lightPos0[] = {4.0f, -4.0f, -8.0f, 1.0f}; //Positioned at (4, -4, - 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    //---------------------light 1----------------------
    GLfloat lightColor1[] = {0.25f, 0.23f, 0.71f, 2.0f}; //Color

	GLfloat lightPos1[] = {-3.0f, -2.0f, 0.0f, 0.0f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    //---------------------texture mapping--------------------------
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //-------------------------------------------------

    glRotatef(X_angle, 1.0f, 0.0f, 0.0f);
    glRotatef(Y_angle, 0.0f, 1.0f, 0.0f);
    glRotatef(Z_angle, 0.0f, 0.0f, 1.0f);
    float surfaceNormal[3];
    float vector_1[3];
    float vector_2[3];
    for(int i=0;i< (int)polygon.size(); i++)
    {
        vector_subtraction(vector_1,array[polygon[i][2]-1],
                           array[polygon[i][1]-1]);//vector 1
        vector_subtraction(vector_2,array[polygon[i][2]-1],
                           array[polygon[i][3]-1]);//vector 2
        cross_product3D(surfaceNormal,vector_1,vector_2);//polygon's normal
        glBegin(GL_POLYGON);
            glNormal3fv(surfaceNormal);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3fv(array[polygon[i][1]-1]);
            glTexCoord2f(0.5f, 0.5f);
            glVertex3fv(array[polygon[i][2]-1]);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3fv(array[polygon[i][3]-1]);
        glEnd();

    }


	glutSwapBuffers(); //Send the 3D scene to the screen
}
void update(int value)
{
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}
void special_key(int key, int x, int y)//up down right left
{
    switch (key)
    {

    case GLUT_KEY_UP://up

        _yPosition += 1;

        break;
    case GLUT_KEY_DOWN://down

        _yPosition -= 1;

        break;
    case GLUT_KEY_RIGHT://up

        _xPosition += 1;

        break;
    case GLUT_KEY_LEFT://down

        _xPosition -= 1;

        break;
    default:

        break;
    }

}

int main(int argc, char** argv)
{
    ///*************************************************
    loadModel model_1;
    model_1.setfileName("queen.d");

    model_1.LoadData();
	vector< vector<float> > temp_vertex = model_1.vec_vertex;
	vertex = temp_vertex;
    //put vertices data in vector vertex
    vector< vector<int> > temp_polygon = model_1.vec_polygon;
    polygon = temp_polygon;

    ///*************************************************


	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800 , 600); //Set the window size

	//Create the window
	glutCreateWindow("Project 3 - 3D Display System");
	initRendering(); //Initialize rendering

	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutReshapeFunc(handleResize);
	//-------------------------------------------
    glutTimerFunc(25, update, 0); //Add a timer
    glutSpecialFunc(special_key);
    glutKeyboardFunc(handleKeypress);

	glutMainLoop();
	return 0; //This line is never reached
}
