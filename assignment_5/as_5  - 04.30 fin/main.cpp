/**********************************************************************
    Spring 2012
    CS 4554 Computer Graphics I
    Project 5 : GLSL Shaders8
    Student Name : YiHung Lee
    Gw ID        : G42957795
    Implement :
        1. phong shading
        2. cube mapping
        3. combine phong shading with texture
        4. animation using sin function
        Shaders used
            1. Phong shader
            2. Cube Mapping shader
            3. Texture and Phong combination shader
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
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include "loadModel.h"
#include "imageloader.h"
#include "Trivial.h"

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
GLuint cube_tex_id; //The id of the Cubic texture
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
//----------------object 4----------
vector< vector<float> > vertex_4;
vector< vector<float> > vertex_normal_4;
vector< vector<int> > polygon_4;
///----------------------------------

GLfloat light_dir[3] = {0,0,0};
GLuint shader_phong,shader_texture,shader_phongTexture,
        shader_cubeMapping,shader_EnvMapping;
GLuint roomTexTop, roomTexDown, roomTexFront, roomTexBack,
        roomTexRight, roomTexLeft;
double sinShift = 0;

//Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y)
{
    GLfloat speed = 0.5f;
    float arrayRight[3];
	switch (key)
	{
		case 27: //Escape key
			exit(0); //Exit the program
			break;
        case 'w': //move camera ahead
            eyex += Ref[0] * speed;
            eyez += Ref[2] * speed;
            break;
        case 's': //move camera back
            eyex -= Ref[0] *speed;
            eyez -= Ref[2] *speed;
            break;
        case 'd': //move camera right
            cross_product3D(arrayRight,Ref,up);
            eyex += arrayRight[0] *speed;
//            eyey += arrayRight[1] *speed;
            eyez += arrayRight[2] *speed;
            break;
        case 'a': //move camera left

            cross_product3D(arrayRight,Ref,up);
            eyex -= arrayRight[0] *speed;
//            eyey -= arrayRight[1] *speed;
            eyez -= arrayRight[2] *speed;
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
    glEnable(GL_TEXTURE_CUBE_MAP);
    glEnable(GL_TEXTURE_2D);
}
void loadRoomTexture()
{
    Image* image1 = loadBMP("brightday2_positive_y.bmp");
    roomTexTop = loadTexture(image1);
    Image* image2 = loadBMP("brightday2_negative_y.bmp");
    roomTexDown = loadTexture(image2);
    Image* image3 = loadBMP("brightday2_positive_z.bmp");
    roomTexFront = loadTexture(image3);
    Image* image4 = loadBMP("brightday2_negative_z.bmp");
    roomTexBack = loadTexture(image4);
    Image* image5 = loadBMP("brightday2_positive_x.bmp");
    roomTexRight = loadTexture(image5);
    Image* image6 = loadBMP("brightday2_negative_x.bmp");
    roomTexLeft = loadTexture(image6);

    delete image1;
    delete image2;
    delete image3;
    delete image4;
    delete image5;
    delete image6;
}

void drawRoom()
{//draw Room
    glPushMatrix();
    GLfloat roomScale_x = 100;
    GLfloat roomScale_y = 50;
    GLfloat roomScale_z = 100;

    glTranslatef(-roomScale_x/2, -10 , -roomScale_z);
    glScalef(roomScale_x, roomScale_y, roomScale_z);

    //---------------------texture mapping--------------------------
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //-----------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, roomTexBack );
    glBegin(GL_QUADS);
        //back
        glNormal3f(0,0,-1);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(0,1,1);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1,1,1);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1,0,1);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(0,0,1);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, roomTexFront );
    glBegin(GL_QUADS);
        //front
        glNormal3f(0,0,1);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1,1,0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0,1,0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0,0,0);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1,0,0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, roomTexTop );
    glBegin(GL_QUADS);
        //top
        glNormal3f(0,-1,0);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0,1,1);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0,1,0);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1,1,0);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1,1,1);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, roomTexDown );
    glBegin(GL_QUADS);
        //down
        glNormal3f(0,1,0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0,0,1);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1,0,1);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1,0,0);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0,0,0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, roomTexRight );
    glBegin(GL_QUADS);
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
    glEnd();
    glBindTexture(GL_TEXTURE_2D, roomTexLeft );
    glBegin(GL_QUADS);
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


    glPopMatrix();
}

void setLight_0()
{//set Light_0
    GLfloat lightPos0[] = {0, 15.5, -84.5, 1.0f};
    GLfloat light0_specular[] = {0.6f, 0.6f, 0.6f, 1.0f}; //White Specular light
    GLfloat light0_diffuse[] = {0.6f, 0.6f, 0.6f, 1.0f}; //red diffuse light

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);


    GLfloat shininess = 50;
    GLfloat material_diffuse[] = {1,1,1, 1.0f};
    GLfloat materialSpecular[] = {1.0f, 0.97f, 0.34f, 1.0f};//light yellow

	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess); //The shininess parameter
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);

}
void drawKnight(float array[][3],float array_vertex_nor[][3],float pos[3],float angle[4])
{
    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(pos[0],pos[1], pos[2]);
    glScalef(3,3,3);
    glRotatef(-90,1,0,0);
    glRotatef(angle[0],angle[1],angle[2],angle[3]);
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
    glPopMatrix();
}
void drawBishop(float array[][3],float array_vertex_nor[][3],float pos[3])
{
    glPushMatrix();
    glTranslatef(pos[0],pos[1], pos[2]);

    glScalef(2,2,2);
    glRotatef(-90,1,0,0);
    glRotatef(180,0,0,1);

    for(int i=0;i< (int)polygon_4.size(); i++)
    {
        glBegin(GL_POLYGON);
            glNormal3fv(array_vertex_nor[polygon_4[i][1]-1]);

            glVertex3fv(array[polygon_4[i][1]-1]);

            glNormal3fv(array_vertex_nor[polygon_4[i][2]-1]);
            glVertex3fv(array[polygon_4[i][2]-1]);
            glNormal3fv(array_vertex_nor[polygon_4[i][3]-1]);
            glVertex3fv(array[polygon_4[i][3]-1]);
        glEnd();

    }
    glPopMatrix();
}
void drawObject_1()//knight
{

    float array[vertex.size()][3];
	ConvertArray(array,vertex.size(),vertex);

	float array_vertex_nor[vertex_normal.size()][3];
	ConvertArray(array_vertex_nor,vertex_normal.size(),vertex_normal);
    float pos1[3] = {-26, -10, -75.0};
    float angle1[4] = {0,0,0,0};
    for(int i= 0; i<5; i++)
    {
        drawKnight(array,array_vertex_nor,pos1,angle1);
        pos1[2] +=10;
    }
    float pos2[3] = {26, -10, -75.0};
    float angle2[4] = {180,0,0,1};
    for(int i= 0; i<5; i++)
    {
        drawKnight(array,array_vertex_nor,pos2,angle2);
        pos2[2] +=10;
    }

}
void drawObject_2()//queen
{

    float array[vertex_2.size()][3];
	ConvertArray(array,vertex_2.size(),vertex_2);

	float array_vertex_nor_2[vertex_normal_2.size()][3];
	ConvertArray(array_vertex_nor_2,vertex_normal_2.size(),vertex_normal_2);

    glPushMatrix();
    glColor4f(1,1,1,1);
    glTranslatef(6.5, -10, -93.0f);
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
    glPopMatrix();
}
void drawObject_3()//king
{
    glPushMatrix();
    float array[vertex_3.size()][3];
	ConvertArray(array,vertex_3.size(),vertex_3);

	float array_vertex_nor_3[vertex_normal_3.size()][3];
	ConvertArray(array_vertex_nor_3,vertex_normal_3.size(),vertex_normal_3);

    glTranslatef(-6.5, -10, -93.0f);

    glScalef(3,3,3);
    glRotatef(-90,1,0,0);
    glRotatef(-90,0,0,1);
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
    glPopMatrix();
}
void drawObject_4()//bishop
{
    float array[vertex_4.size()][3];
	ConvertArray(array,vertex_4.size(),vertex_4);

	float array_vertex_nor[vertex_normal_4.size()][3];
	ConvertArray(array_vertex_nor,vertex_normal_4.size(),vertex_normal_4);

    float pos1[3] = {-17, -10, -89.0};
    drawBishop(array,array_vertex_nor,pos1);

    float pos2[3] = {17, -10, -89.0};
    drawBishop(array,array_vertex_nor,pos2);
}


void drawSphere()
{
    glPushMatrix();
    glTranslatef(0, -2.5 + sin(sinShift), -85.5);
    glScalef(2.5,2.5,2.5);

    //-----------cube mapping--------

    // Render the cube-mapped object:
    glUseProgram(shader_cubeMapping);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cube_tex_id);

    GLint cube_tex_loc = glGetUniformLocation(shader_cubeMapping, "cube_tex");
    glUniform1i(cube_tex_loc, 0);
    GLint cube_camera_loc = glGetUniformLocation(shader_cubeMapping, "camera_pos");
    glUniform3f(cube_camera_loc, eyex, eyey, eyez);
    //-----------------------
    glutSolidSphere(2,50,20);

    glPopMatrix();
    glDisable(GL_TEXTURE_CUBE_MAP);

}

void drawScene()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective

    gluLookAt(eyex, eyey, eyez,
              eyex + Ref[0], eyey + Ref[1], eyez + Ref[2],
              up[0], up[1], up[2]);
    // Get the location of the texture, and set the texture ID to 0

    //light
    setLight_0();

    //draw Geometry
    //draw room
    glUseProgram(shader_phongTexture);
    drawRoom();

    glUseProgram(shader_phong);
    //draw object 1
    drawObject_1();//knight
    //draw object 2
    drawObject_2();//queen
    //draw object 3
    drawObject_3();//king
    //draw object 4
    drawObject_4();//bishop

    //draw sphere
    drawSphere();


	glutSwapBuffers(); //Send the 3D scene to the screen
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
    //--------------object 4---------
    loadModel model_4;
    model_4.setfileName("bishop.d");
    model_4.LoadData();

	vertex_4 = model_4.vec_vertex;
	polygon_4 = model_4.vec_polygon;
    vertex_normal_4 = model_4.vertex_normal;

}
void update(int value)
{
    sinShift += 0.1;
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}
int main(int argc, char** argv)
{
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800 , 600); //Set the window size

	//Create the window
	glutCreateWindow("Project 5 - GLSL Shaders");
	initRendering(); //Initialize rendering

	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutReshapeFunc(handleResize);
	//-------------------------------------------
    glutTimerFunc(25, update, 0); //Add a timer
    glutSpecialFunc(special_key);//up down
    glutKeyboardFunc(handleKeypress);
    ///load geometry-------------------
    loadGeometry();
    ///load room texture
    loadRoomTexture();
    //-----------glew--------------

    glewInit();
    shader_phong = CreateShaders("phong");
    shader_phongTexture = CreateShaders("phongTexture");
    shader_cubeMapping = CreateShaders("CubeMapping");
    cube_tex_id = loadCubicTexture();
	//--------------------------

	glutMainLoop();
	return 0; //This line is never reached
}
