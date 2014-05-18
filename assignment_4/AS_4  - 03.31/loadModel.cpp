/** Tasks in LoadModel.cpp
    Load file with .d extension
**/
#include "loadModel.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <math.h>
using namespace std;

loadModel::loadModel()
{   //set up default parameter
    setfileName("knight.d"); //input file name

}


void loadModel::LoadData()
{   //load geometric data from file to program

    //initialize default parameter
    ifstream inGeometricData(fileName, ios::in);
    //load Geometric data *.d, constructor name - inGeometricData
    if(!inGeometricData)
    {
        cerr << "File could not be opened" <<endl;
        exit(1); //#include <cstdlib>
    }

    int num_vertex, num_polygon;
    // store 1)number of vertex and 2)number of polygon
    string first_word;
    //store first word - data

    string headline_string;
    char headline[256];
    inGeometricData.getline(headline, 256, '\n');
    inGeometricData.seekg(0L, ios::beg);//put cursor back to beginning
    headline_string = headline; //convert to string type
    if(!headline_string.find("data"))
    {
        inGeometricData >>first_word >> num_vertex >> num_polygon;
        //first line include word "data"
    }
    else
    {
        inGeometricData >> num_vertex >> num_polygon;
    }


    vector< vector<float> > vertex(num_vertex, vector<float>(4));
    //declare 2 dimensional vector to store vertices data
    for(int i = 0; i< num_vertex; i++)
    {
        //put vertex data to vector - vec_vertex
        inGeometricData >> vertex[i][0]
                        >> vertex[i][1]
                        >> vertex[i][2];
        vertex[i][3] = 1;
    }
    vec_vertex = vertex; // assign to public vector vec_vertex

    //declare 2 dimensional vector to store polygon data
    //dynamic array to fit different number of vertex index

    for(int i = 0; i< num_polygon; i++)
    {
        //put element to vector vec_polygon
        int element,index;
        inGeometricData>>index; //get number of vertex index
        vector<int> row;
        row.push_back(index);//put in a new vector
        for (int j = 0; j < index  ; j++)
        {
            inGeometricData>>element;
            row.push_back(element); // Add an element to the row
        }
        vec_polygon.push_back(row);
        // Add the vector to the main vector (vec_polygon)
    }
    inGeometricData.close(); // close file
    //end of load vertices from file
    Compute_Poly_Normal();
}

void loadModel::setfileName(string name)
{   //set geometric file name
    fileName = new char[name.length() + 1];
    strcpy(fileName, name.c_str());
}
void loadModel::Compute_Poly_Normal()
{
    vector< vector<float> > temp_vertex_normal(vec_vertex.size(),vector<float>(3));

    vector <float> poly_vector_1(3);
    vector <float> poly_vector_2(3);
    vector <float> poly_normal(3);
    for(int i=0; i<(int)vec_polygon.size(); i++)
    {//compute polygon normal
        vector_subtraction(poly_vector_1,
                           vec_vertex[vec_polygon[i][1]-1],
                           vec_vertex[vec_polygon[i][2]-1]);
        vector_subtraction(poly_vector_2,
                           vec_vertex[vec_polygon[i][1]-1],
                           vec_vertex[vec_polygon[i][3]-1]);

        //calculus cross product
        cross_product3D(poly_normal, poly_vector_1, poly_vector_2);


        for(int j=1; j<(int)vec_polygon[i][0]+1 ;j++)
        {//add polygon normal to vertex
            for (int p=0; p<3 ;p++ )
                temp_vertex_normal[vec_polygon[i][j]-1][p] += poly_normal[p];
        }

    }
    for(int i=0; i<(int)temp_vertex_normal.size() ;i++ )
    {
        float length = vector_length(temp_vertex_normal[i]);
        for (int j=0; j<3 ;j++ )
        {   //normalize the normal on vertex
            temp_vertex_normal[i][j] =
                temp_vertex_normal[i][j] / length;
        }
    }
    vertex_normal = temp_vertex_normal;
    //vertex normals as average of surrounding neibor polygon's normal
}
void loadModel::cross_product3D(vector<float> &normal,
                        vector<float>vector_1, vector<float>vector_2)
{//compute cross product
    normal[0] = vector_1[1]*vector_2[2] - vector_2[1]*vector_1[2];
    normal[1] = -(vector_1[0]*vector_2[2] - vector_2[0]*vector_1[2]);
    normal[2] = vector_1[0]*vector_2[1] - vector_2[0]*vector_1[1];
}
void loadModel::vector_subtraction(vector<float> &sub,
                   vector<float> vector_1, vector<float> vector_2)
{   //compute vector subtraction
    //vector_2 - vector_1
    for(int i = 0 ; i < 3 ; i++)
        sub[i] = vector_2[i] - vector_1[i];
}
float loadModel::vector_length(vector<float> vector_1)
{//compute vector length
    return pow(pow(vector_1[0],2) + pow(vector_1[1],2) +
               pow(vector_1[2],2),0.5);
}
loadModel::~loadModel()
{
    //deconstructor
}
