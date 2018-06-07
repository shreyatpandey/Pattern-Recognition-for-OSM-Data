#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <fstream>
#include <iostream>

#define PI 3.141592
using namespace cv;
using namespace std;

double CovertToDegree(double radian)
{
 const double C_rad =180.0/M_PI;
 return radian*C_rad;

}


int main(int argc, char** argv)
{
    if(argc < 3){
        cout << argv[0] << " expects an input file";
        cout << " on the command line and the filename output" << endl;
        return 1;
    }

    //Output file to dump the values.

    ofstream out_file;


    // Declare the output variables

    Mat dst, cdst, cdstP;
   // const char* default_file = "../data/sudoku.png";
    //const char* filename = argc >=2 ? argv[1] : default_file;
    // Loads an image
    Mat src = imread(argv[1], IMREAD_GRAYSCALE );
    cout <<"Image referenced with : "<<argv[1]<<endl;
    // Check if image is loaded fine
    if(src.empty()){
        printf(" Error opening image\n");
        //printf(" Program Arguments: [image_name -- default %s] \n", default_file);
        return -1;
    }
    // Edge detection
    Canny(src, dst, 50, 200, 3);
    // Copy edges to the images that will display the results in BGR
    cvtColor(dst, cdst, COLOR_GRAY2BGR);
    cdstP = cdst.clone();
    // Standard Hough Line Transform
    vector<Vec2f> lines; // will hold the results of the detection
    HoughLines(dst, lines, 1, CV_PI/180, 140, 0, 0 ); // runs the actual detection
    // Draw the lines
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( cdst, pt1, pt2, Scalar(0,0,255), 3, LINE_AA);
    }

    // Probabilistic Line Transform
    vector<Vec4i> linesP; // will hold the results of the detection
    HoughLinesP(dst, linesP, 1, CV_PI/180, 50, 50, 10 ); // runs the actual detection
    // Draw the lines and ouput the lines to a file
   
    out_file.open(argv[2]);//
     out_file<<linesP.size()<<endl;
    out_file<<std::endl;
    out_file <<"__R__"<<" "<<"_Theta_"<<std::endl;
    
    //myfile.close();
    for( size_t i = 0; i < linesP.size(); i++ )
    {
        Vec4i l = linesP[i];
        line( cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
        double m = (l[3]-l[1])/((double)(l[2]-l[0]));
        double c = (double)l[3] - (m*l[2]);
        
        
        
        double r = std::abs(c/(sqrt((m*m)+1.00)));
        double angle = CovertToDegree(atan(m));
        double distance ;
        if(std::abs(angle) ==180 ||std::abs(angle) == 0 )
        {
            distance = (double)l[3];
            /*if(l[3]==l[1])
                cout <<"They are perpendicular...."<<endl;
            if(l[2]==l[4])
                cout <<"They are perpendicular.."<<endl;*/
        }
        else if(std::abs(angle) == 90)
        {
            distance =(double)l[2];
            /*if(l[2]==l[4])
                cout <<"They are perpendicular.."<<endl;
            if(l[3]==l[1])
                cout <<"They are perpendicular...."<<endl;*/
        }
        else 
        {
            distance = abs(r*cos(angle*PI/180));
        }
        out_file <<distance<<"  "<<CovertToDegree(atan(m))<<std::endl;
        
    }
    cout<<"Number of detected lines: "<<linesP.size()<<endl;
    out_file.close();
    
    
    //cout<<"Number of detected lines: "<<linesP.size()<<endl;
    // Show results

    
    imshow("Source", src);
    imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
    imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP); 

    // Wait and Exit

   waitKey();
    return 0;
}
