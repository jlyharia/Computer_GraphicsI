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


        loadModel();
        //constructor and set up default parameter

        void LoadData();
        //load geometric data from file to program

        virtual ~loadModel();

        void setfileName (string);
        //set geometric file name

    private:
        char *fileName;



    protected:

};

#endif // LOADMODEL_H
