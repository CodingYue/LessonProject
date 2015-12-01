
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
int main(int, char** argv)
{
	// Load the image
	Mat src = imread(argv[1]);
	// Check if everything was fine
	if (!src.data)
		return -1;

	imshow("Source Image", src);
	return 0;
}
