#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
/*#include "preprocessing.h"*/

using namespace std;

#define nofFeatures 2
#define FLOAT_MAX 33554430

void printArray(double arr[],int n){

    for (int i = 0; i < n;i++){

        cout << arr[i]<<" ";
        if((i%10)==9)
            cout<<endl;
    }
}


int main(int argc, char *argv[])
{

/*********************************************************************
  This first stage is to process the information for TWO input files */

   if(argc < 3){
        cout << argv[0] << " expects two input files";
        cout << " on the command line to run tests" << endl;
        return 1;
    }

    // first file will be reference, second file will be "tested"
    ifstream fileref(argv[1]);
    ifstream filecom(argv[2]);

    string separator;
    int linesref = 0, linescom = 0;

    /*Count number of streets in file REFERENCE*/
    while (1){

        getline(fileref,separator);
        if (fileref.eof())  break;
        else linesref++;
    }

    fileref.close();
    cout<<"N of lines for REFERENCE text file: "<< --linesref<<endl;


    /*Count number of streets in file TEST*/
    while (1){

        getline(filecom,separator);
        if (filecom.eof())  break;
        else linescom++;
    }

    filecom.close();
    cout<<"N of lines for COMPARE text file: "<< --linescom<<endl;

    /*Store values into a matrix REFERENCE*/
    double matrixref[linesref][nofFeatures]; //definition of the matrix reference
    fileref.open(argv[1]);
    getline(fileref,separator);
    for(int row = 0; row < linesref; row++)
    {
        string line;
        getline(fileref, line);
        if ( !fileref.good() )
            break;

        stringstream iss(line);

        for (int col = 0; col <= nofFeatures; col++)
        {
            string val;
            getline(iss, val, ' ');
            if ( !iss.good() ){
                stringstream convertor(val);
                convertor >> matrixref[row][col];
                break;
            }

            stringstream convertor(val);
            convertor >> matrixref[row][col];
        }
    }


    //printing of the stored data array
    /*cout<<"Matrix for reference"<<endl;
    for (int i = 0; i < linesref;i++){
        for (int j = 0; j < nofFeatures;j++){
            cout << matrixref[i][j]<<"  ";
        }
        cout<<endl;
    }*/


    /*Store values into a matrix TEST*/
    double matrixcom[linescom][nofFeatures]; //definition of the matrix test
    filecom.open(argv[2]);
    getline(filecom,separator);
    for(int row = 0; row < linescom; row++)
    {
        string line;
        getline(filecom, line);
        if ( !filecom.good() )
            break;

        stringstream iss(line);

        for (int col = 0; col <= nofFeatures; col++)
        {
            string val;
            getline(iss, val, ' ');
            if ( !iss.good() ){
                stringstream convertor(val);
                convertor >> matrixcom[row][col];
                break;
            }

            stringstream convertor(val);
            convertor >> matrixcom[row][col];
        }
    }

    //printing of the stored data array
    /*cout<<"Matrix for Comparison"<<endl;
    for (int i = 0; i < linescom;i++){
        for (int j = 0; j < nofFeatures;j++){
            cout << matrixcom[i][j]<<"  ";
        }
        cout<<endl;
    } */


/********************************************************************************
  This stage is to run the Closest Neighbor algorithm to get the combined RMSE */

    /* For each point in the COMPARISON MATRIX we will FIRST find the CLOSEST
    NEIGHBOR,then SECOND we calculate the result and will store it in a matrix*/
    double matrixsol[linescom];
    double minimum = FLOAT_MAX;
    double distance;

    for (int i=0;i<linescom;i++){
        // every round we start assigning a comparison entry as PIVOT
        for(int j=0;j<linesref;j++){
            //compare each reference matrix entry with the PIVOT comparison entry
            double a0 = matrixcom[i][0];
            double a1 = matrixcom[i][1];
            double b0 = matrixref[j][0];
            double b1 = matrixref[j][1];

            distance = sqrt(pow(a0-b0,2)+pow(a1-b1,2));
            if (distance < minimum)  minimum = distance;
        }
        matrixsol[i] = minimum;// the minimum will be the neighbor
        minimum = FLOAT_MAX;
    }

    //printing of the stored solution array
    /*cout<<"Matrix of Solutions: "<<endl;
    for (int i = 0; i < linescom;i++){
            cout << matrixsol[i]<<"  ";
    }
    cout<<endl;*/

    //Combine the results from the matrix solution to generate the RMSE for the two input cities
    double sum = 0;
    for (int i = 0; i < linescom;i++){
            sum = sum + pow(matrixsol[i],2);
    }

    double RMSE = sqrt( (double) sum/linescom);
    ofstream output("rmse.txt");
    
    cout<<"The final RMSE is: "<< RMSE<<endl;
    output<<RMSE<<endl;

    return 0;
}
