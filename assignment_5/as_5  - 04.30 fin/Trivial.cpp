#include "Trivial.h"
#include "imageloader.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include "textfile.h"
#define PI 3.1415927
using namespace std;
void cross_product3D(float *normal, float vector_1[3], float vector_2[3])
{//compute cross product
    normal[0] = vector_1[1]*vector_2[2] - vector_2[1]*vector_1[2];
    normal[1] = -(vector_1[0]*vector_2[2] - vector_2[0]*vector_1[2]);
    normal[2] = vector_1[0]*vector_2[1] - vector_2[0]*vector_1[1];
}

void ConvertArray(float array[][3] ,int num_vertex ,
                      vector< vector<float> > vertex )
{
    for(int i=0;i<num_vertex;i++)
        for(int j = 0; j<3; j++)
            array[i][j] = vertex[i][j];
}
int printOglError(char *file, int line)
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;
    //glGetError ¡X return error information
    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}


void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    //The status of the compile steps can be queried in OpenGL 2.0
    //with the following function:
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
    //glGetShaderiv  ßX Returns a parameter from a shader object
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        //glGetShaderInfoLog  ßX Returns the information log for a shader object
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    //The status of the link step can be queried in OpenGL 2.0
    //with the following function:
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
    //glGetProgramiv  ßX Returns a parameter from a program object
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        //glGetProgramInfoLog  ßX Returns the information log for a program object
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

//----------glut function------------------
//Called when the window is resized
void handleResize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0, (double)w / (double)h, 1.0, 200.0);

}

GLuint loadTexture(Image* image)
{
    GLuint textureId;
    glGenTextures(1, &textureId); //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexGeni(GL_S, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexGeni(GL_T, GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //----------------------------------
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
GLuint loadCubicTexture()
{
    Image* cube_image_back = loadBMP("brightday2_negative_z.bmp");
    Image* cube_image_front = loadBMP("brightday2_positive_z.bmp");
    Image* cube_image_top = loadBMP("brightday2_positive_y.bmp");
    Image* cube_image_bottom = loadBMP("brightday2_negative_y.bmp");
    Image* cube_image_left = loadBMP("brightday2_negative_x.bmp");
    Image* cube_image_right = loadBMP("brightday2_positive_x.bmp");
    GLuint cube_tex_id = NULL;

    glBindTexture(GL_TEXTURE_CUBE_MAP, cube_tex_id);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB,
                 cube_image_back->width, cube_image_back->height,
                    0, GL_RGB, GL_UNSIGNED_BYTE, cube_image_back->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB,
                 cube_image_front->width, cube_image_front->height,
                    0, GL_RGB, GL_UNSIGNED_BYTE, cube_image_front->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB,
                 cube_image_top->width, cube_image_top->height,
                    0, GL_RGB, GL_UNSIGNED_BYTE, cube_image_top->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB,
                 cube_image_bottom->width, cube_image_bottom->height,
                    0, GL_RGB, GL_UNSIGNED_BYTE, cube_image_bottom->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB,
                 cube_image_left->width, cube_image_left->height,
                    0, GL_RGB, GL_UNSIGNED_BYTE, cube_image_left->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB,
                 cube_image_right->width, cube_image_right->height,
                    0, GL_RGB, GL_UNSIGNED_BYTE, cube_image_right->pixels);
    // For each of the 6 textures, load the image. Face will be set to:
    // GL_TEXTURE_CUBE_MAP_POSITIVE_{X,Y,Z} and GL_TEXTURE_CUBE_MAP_NEGATIVE_{X,Y,Z}
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    delete cube_image_back;
    delete cube_image_front;
    delete cube_image_top;
    delete cube_image_bottom;
    delete cube_image_left;
    delete cube_image_right;
    return cube_tex_id;
}
GLuint CreateShaders(string file_name)
{

	char *vs = NULL,*fs = NULL;

	GLuint v = glCreateShader(GL_VERTEX_SHADER);  //shader step 1. create shader  - vertex shader
	GLuint f = glCreateShader(GL_FRAGMENT_SHADER);//shader step 1. create shader  - fragment shader

    string vert_shader = file_name + ".vert";
	string frag_shader = file_name + ".frag";

	vs = textFileRead((char *)vert_shader.c_str());
	fs = textFileRead((char *)frag_shader.c_str());
	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(v, 1, &vv,NULL);//shader step 2. shader source - vertex shader
	glShaderSource(f, 1, &ff,NULL);//shader step 2. shader source - fragment shader

	free(vs);free(fs);

	glCompileShader(v);//shader step 3. compile shader - vertex shader
	glCompileShader(f);//shader step 3. compile shader - fragment shader

    //---------Troubleshooting: The InfoLog - shader------------
	printShaderInfoLog(v);
	printShaderInfoLog(f);
    //----------------------------------------
	GLuint shader_program = glCreateProgram();//program step 1. create program
	glAttachShader(shader_program,v);//program step 2. attach shader - vertex shader
	glAttachShader(shader_program,f);//program step 2. attach shader - fragment shader
	glLinkProgram(shader_program);//program step 3. link program
	//---------Troubleshooting: The InfoLog - program------------
	printProgramInfoLog(shader_program);
    //------------------------------------------
//	glUseProgram(p);//program step 4. use program
    return shader_program;

}
