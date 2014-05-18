/** Tasks in LoadModel.cpp
    Load file with .d extension
**/
#ifndef LOADMODEL_H
#define LOADMODEL_H
#include <string>
#include <vector>
using namespace std;
class loadModel
{
    public:
        vector< vector<float> > vec_vertex;
        //vector for original vertices data
        vector< vector<int> > vec_polygon;
        //vector for polygon order data
        vector< vector<float> > vertex_normal;

        loadModel();
        //constructor and set up default parameter

        void LoadData();
        //load geometric data from file to program

        virtual ~loadModel();

        void setfileName (string);
        //set geometric file name
        float vector_length(vector<float>);
        //compute vector length
        void cross_product3D(vector<float> &, vector<float>, vector<float>);
        //compute cross product
        void vector_subtraction(vector<float> &, vector<float>, vector<float>);
        //compute vector subtraction (vector_2 - vector_1)
        void Compute_Poly_Normal();
    private:
        char *fileName;



    protected:

};

#endif // LOADMODEL_H
