#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include "imageloader.h"
#include <string>
using namespace std;
void cross_product3D(float *normal, float vector_1[3], float vector_2[3]);
//compute cross product
void ConvertArray(float array[][3] ,int num_vertex ,vector< vector<float> > vertex );

int printOglError(char *file, int line);
void printShaderInfoLog(GLuint obj);
void printProgramInfoLog(GLuint obj);

//----------glut function----------------------

void handleResize(int w, int h);
GLuint loadTexture(Image* image);
GLuint loadCubicTexture();
//----------shaders----
GLuint CreateShaders(string file_name);

