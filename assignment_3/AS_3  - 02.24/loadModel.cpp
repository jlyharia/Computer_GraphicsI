/** Tasks in LoadModel.cpp
    LoadModel.cpp contains all routine regarding vertices transformation ,
    and back face eliminate.
    Vertices transformations compute vertices from local coordinate to
    perspective coordinate and translate to normalized format.
    Back face culling process in view space and skip invisible vertices enter
    perspective transformation.
**/
#include "loadModel.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <math.h>
#include <iomanip>
using namespace std;


loadModel::loadModel()
{   //set up default parameter
    setfileName("house.d"); //input file name

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
}

void loadModel::setfileName(string name)
{   //set geometric file name
    fileName = new char[name.length() + 1];
    strcpy(fileName, name.c_str());
}


loadModel::~loadModel()
{
    //deconstructor
}
