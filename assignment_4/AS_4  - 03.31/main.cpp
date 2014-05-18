/**********************************************************************
    Spring 2012
    CS 4554 Computer Graphics I
    Project 4 : Illumination Model & Texture Mapping
    Student Name : YiHung Lee
    Gw ID        : G42957795
    Additional Implement :
        * Two light sources

    Instruction :
        1: Use 'w', 's', 'a', 'd' to move around the room
        2: Use arrow key to rotate camera angle
        3: press 'Esc' to leave program

    Note:
        1: This system only works well in triangle polygon file
        2: Camera may move out of the room
        3: Rotate up and down have limitation
 **********************************************************************/
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <vector>
#include <GL/glut.h>
#include "loadModel.h"
#include "imageloader.h"
#include <math.h>

#define PI 3.14159265
using namespace std;

//---gluLookAt parameter
GLdouble eyex = 0;
GLdouble eyey = 0;
GLdouble eyez = 0;
GLfloat up[3] = {0, 1, 0};
GLfloat Ref[3] = {0,0,-1};
//-------------Camera---
GLfloat _cameraUpAngle = 0;
GLfloat _cameraAngle = 0;
GLuint _textureId; //The id of the texture
//----------------object 1----------
vector< vector<float> > vertex;
vector< vector<float> > vertex_normal;
vector< vector<int> > polygon;
//----------------object 2----------
vector< vector<float> > vertex_2;
vector< vector<float> > vertex_normal_2;
vector< vector<int> > polygon_2;
//----------------object 3----------
vector< vector<float> > vertex_3;
vector< vector<float> > vertex_normal_3;
vector< vector<int> > polygon_3;
void setLight_0();
void setLight_1();
void cross_product3D(float *normal, float vector_1[3], float vector_2[3])
{//compute cross product
    normal[0] = vector_1[1]*vector_2[2] - vector_2[1]*vector_1[2];
    normal[1] = -(vector_1[0]*vector_2[2] - vector_2[0]*vector_1[2]);
    normal[2] = vector_1[0]*vector_2[1] - vector_2[0]*vector_1[1];
}
//Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y)
{
    GLfloat fraction = 0.5f;
    float arrayRight[3];
	switch (key)
	{
		case 27: //Escape key
			exit(0); //Exit the program
			break;
        case 'w': //move camera ahead
            eyex += Ref[0] * fraction;
            eyez += Ref[2] * fraction;
            break;
        case 's': //move camera back
            eyex -= Ref[0] *fraction;
            eyez -= Ref[2] *fraction;
            break;
        case 'd': //move camera right

            cross_product3D(arrayRight,Ref,up);
            eyex += arrayRight[0] *fraction;
            eyey += arrayRight[1] *fraction;
            eyez += arrayRight[2] *fraction;
            break;
        case 'a': //move camera left

            cross_product3D(arrayRight,Ref,up);
            eyex -= arrayRight[0] *fraction;
            eyey -= arrayRight[1] *fraction;
            eyez -= arrayRight[2] *fraction;
            break;
        default:
            break;
	}
}
void special_key(int key, int x, int y)//up down right left
{

    switch (key)
    {
        case GLUT_KEY_UP://rotate camera up

            _cameraUpAngle += 0.05f;
            Ref[1] = sin(_cameraUpAngle);//y
            break;
        case GLUT_KEY_DOWN://rotate camera down
            _cameraUpAngle -= 0.05f;
            Ref[1] = sin(_cameraUpAngle);
            break;
        case GLUT_KEY_RIGHT://rotate camera right
            _cameraAngle += 0.05f;
            Ref[0] = sin(_cameraAngle);//x
            Ref[2] = -cos(_cameraAngle);//z

            break;
        case GLUT_KEY_LEFT://rotate camera left
            _cameraAngle -= 0.05f;
            Ref[0] = sin(_cameraAngle);//x
            Ref[2] = -cos(_cameraAngle);//z

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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //clear the background color
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_CULL_FACE); //Enable back face culling
	glCullFace(GL_BACK);
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_NORMALIZE); //Automatically normalize normals

	glShadeModel(GL_SMOOTH); //Enable smooth shading
	Image* image = loadBMP("Skies0338_1_thumbhuge.bmp");
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

void drawRoom()
{//draw Room
    GLfloat roomScale_x = 100;
    GLfloat roomScale_y = 50;
    GLfloat roomScale_z = 100;

    glTranslatef(-roomScale_x/2, -10 , -roomScale_z);
    glScalef(roomScale_x, roomScale_y, roomScale_z);

    //---------------------texture mapping--------------------------
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //-------------------------------------------------
    glBegin(GL_QUADS);
        //front
        glNormal3f(0,0,-1);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0,1,1);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1,1,1);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1,0,1);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0,0,1);
        //back
        glNormal3f(0,0,1);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1,1,0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0,1,0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0,0,0);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1,0,0);

        //top
        glNormal3f(0,-1,0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0,1,1);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0,1,0);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1,1,0);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1,1,1);

        //bottom
        glNormal3f(0,1,0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0,0,1);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1,0,1);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1,0,0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0,0,0);

        //right
        glNormal3f(-1,0,0);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1,1,1);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1,1,0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1,0,0);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1,0,1);

        //left
        glNormal3f(1,0,0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0,0,1);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(0,0,0);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(0,1,0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0,1,1);
    glEnd();
    glDisable(GL_TEXTURE_2D);

}
//Draws the 3D scene
void setLight_0()
{//set Light_0
    GLfloat lightPos0[] = {20.0f, -4.0f, -8.0f, 1.0f};

    GLfloat light0_specular[] = {0.6f, 0.6f, 0.6f, 1.0f}; //White Specular light
    GLfloat light0_diffuse[] = {0.5f, 0.35f, 0.2f, 1.0f}; //red diffuse light

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);


    GLfloat shininess = 50;
    GLfloat materialColor[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat materialSpecular[] = {1.0f, 0.97f, 0.34f, 1.0f};//light yellow
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess); //The shininess parameter
}
void setLight_1()
{//set Light_1

    GLfloat light1_specular[] = {0.5f, 0.5f, 0.8f, 1.0f}; //blue Specular light
    GLfloat light1_diffuse[] = {0.25f, 0.23f, 0.71f, 2.0f}; //blue diffuse light
	GLfloat lightPos1[] = {-1, 0, -2 ,0.0};


	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);


    glEnable(GL_LIGHT1);
}
void drawObject_1()
{

    float array[vertex.size()][3];
	ConvertArray(array,vertex.size(),vertex);

	float array_vertex_nor[vertex_normal.size()][3];
	ConvertArray(array_vertex_nor,vertex_normal.size(),vertex_normal);
    glColor4f(1,1,1,0.6);

    glTranslatef(0, -10, -50.0f);

    glScalef(3,3,3);

    glRotatef(-90,1,0,0);

    for(int i=0;i< (int)polygon.size(); i++)
    {
        glBegin(GL_POLYGON);
            glNormal3fv(array_vertex_nor[polygon[i][1]-1]);

            glVertex3fv(array[polygon[i][1]-1]);

            glNormal3fv(array_vertex_nor[polygon[i][2]-1]);
            glVertex3fv(array[polygon[i][2]-1]);
            glNormal3fv(array_vertex_nor[polygon[i][3]-1]);
            glVertex3fv(array[polygon[i][3]-1]);
        glEnd();

    }
}
void drawObject_2()
{

    float array[vertex_2.size()][3];
	ConvertArray(array,vertex_2.size(),vertex_2);

	float array_vertex_nor_2[vertex_normal_2.size()][3];
	ConvertArray(array_vertex_nor_2,vertex_normal_2.size(),vertex_normal_2);


    glColor4f(1,1,1,1);
    glTranslatef(10, -10, -40.0f);
    glScalef(3,3,3);
    glRotatef(-90,1,0,0);

    for(int i=0;i< (int)polygon_2.size(); i++)
    {

        glBegin(GL_POLYGON);
            glNormal3fv(array_vertex_nor_2[polygon_2[i][1]-1]);

            glVertex3fv(array[polygon_2[i][1]-1]);

            glNormal3fv(array_vertex_nor_2[polygon_2[i][2]-1]);
            glVertex3fv(array[polygon_2[i][2]-1]);
            glNormal3fv(array_vertex_nor_2[polygon_2[i][3]-1]);
            glVertex3fv(array[polygon_2[i][3]-1]);
        glEnd();

    }
}
void drawObject_3()
{

    float array[vertex_3.size()][3];
	ConvertArray(array,vertex_3.size(),vertex_3);

	float array_vertex_nor_3[vertex_normal_3.size()][3];
	ConvertArray(array_vertex_nor_3,vertex_normal_3.size(),vertex_normal_3);

    glTranslatef(-10, -10, -30.0f);
    glScalef(3,3,3);
    glRotatef(-90,1,0,0);

    for(int i=0;i< (int)polygon_3.size(); i++)
    {
        glBegin(GL_POLYGON);
            glNormal3fv(array_vertex_nor_3[polygon_3[i][1]-1]);

            glVertex3fv(array[polygon_3[i][1]-1]);

            glNormal3fv(array_vertex_nor_3[polygon_3[i][2]-1]);
            glVertex3fv(array[polygon_3[i][2]-1]);
            glNormal3fv(array_vertex_nor_3[polygon_3[i][3]-1]);
            glVertex3fv(array[polygon_3[i][3]-1]);
        glEnd();

    }
}

void drawScene()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective

    gluLookAt(eyex, eyey, eyez,
              eyex + Ref[0], eyey + Ref[1], eyez + Ref[2],
              up[0], up[1], up[2]);
    //light
    glPushMatrix();
    setLight_0();
    setLight_1();
    glPopMatrix();

    //draw Geometry
    //draw room
    glPushMatrix();
    drawRoom();
    glPopMatrix();

    //draw object 1
    glPushMatrix();
    drawObject_1();
    glPopMatrix();
    //draw object 2
    glPushMatrix();
    drawObject_2();
    glPopMatrix();

    //draw object 3
    glPushMatrix();
    drawObject_3();
    glPopMatrix();

	glutSwapBuffers(); //Send the 3D scene to the screen
}
void update(int value)
{
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}

void loadGeometry()
{
    //--------------object 1---------
    loadModel model_1;
    model_1.setfileName("knight.d");
    model_1.LoadData();

	vertex = model_1.vec_vertex;//get vertex data
    polygon = model_1.vec_polygon;//get polygon data
    vertex_normal = model_1.vertex_normal;//get vertex normal data
    //--------------object 2---------
    loadModel model_2;
    model_2.setfileName("queen.d");
    model_2.LoadData();

	vertex_2 = model_2.vec_vertex;
    polygon_2 = model_2.vec_polygon;
    vertex_normal_2 = model_2.vertex_normal;
    //--------------object 3---------
    loadModel model_3;
    model_3.setfileName("king.d");
    model_3.LoadData();

	vertex_3 = model_3.vec_vertex;
	polygon_3 = model_3.vec_polygon;
    vertex_normal_3 = model_3.vertex_normal;

}
int main(int argc, char** argv)
{

    ///*************************************************
    loadGeometry();

    ///*************************************************

	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800 , 600); //Set the window size

	//Create the window
	glutCreateWindow("Project 4 - Illumination Model & Texture Mapping");
	initRendering(); //Initialize rendering

	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);

	glutReshapeFunc(handleResize);
	//-------------------------------------------
    glutTimerFunc(25, update, 0); //Add a timer
    glutSpecialFunc(special_key);//up down
    glutKeyboardFunc(handleKeypress);

	glutMainLoop();
	return 0; //This line is never reached
}
