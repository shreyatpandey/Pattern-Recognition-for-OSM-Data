#include "opencv2/opencv.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    cv::Mat src = cv::imread(argv[1],0);
    double angle = atoi(argv[3]);
    // get rotation matrix for rotating the image around its center
    cv::Point2f center(src.cols/2.0, src.rows/2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    // determine bounding rectangle
    cv::Rect bbox = cv::RotatedRect(center,src.size(), angle).boundingRect();
    // adjust transformation matrix
    rot.at<double>(0,2) += bbox.width/2.0 - center.x;
    rot.at<double>(1,2) += bbox.height/2.0 - center.y;

    cv::Mat dst;
    cv::warpAffine(src, dst, rot, bbox.size());
    cv::Mat dest;
    cv::cvtColor(dst,dest,cv::COLOR_GRAY2BGR);
    cv::imwrite(argv[2], dest);

    //cv::imshow("Display",dst);
    //std::cout<<" New rotated image done! "<<std::endl;

    return 0;
}

