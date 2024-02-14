#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;

int main()
{

    Mat img = imread("..//cutCat.jpg");


    if (img.empty())
    {
        std::cout << "Could not read the image!" << std::endl;
        return 1;
    }

    namedWindow("Display Test", 0);
    resizeWindow("Display Test", 937, 1250);
    imshow("Display Test", img);

    int iKey = waitKey(0);

    return 0;
}

