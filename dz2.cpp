#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

//Global Variables
int alpha = 1, beta = 0;
int alpha_MAX = 3; //Contrast limit
int beta_MAX = 100; //Brightness limit

/// Matrices to store images
Mat image;
Mat new_image;

void Hist(Mat im)
{
	Mat src, dst;

	/// Load image
	src = im;

	/// Separate the image in 3 places ( B, G and R )

	Mat bgr_planes[3];
	//vector<Mat> bgr_planes;
	split(src, bgr_planes);

	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat b_hist, g_hist, r_hist;

	/// Compute the histograms:
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	// Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	/// Draw for each channel
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}

	/// Display
	namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
	imshow("calcHist Demo", histImage);
}

void on_Trackbar(int, void* = NULL)
{
	image.convertTo(new_image, -1, alpha, beta);
	imshow("image", new_image); //Update the image
	Hist(new_image);
}

int main(int argc, char** argv)
{
	/// Read image ( same size, same type )
	image = imread("image.jpg");
	Mat new_image = Mat::zeros(image.size(), image.type());

	if (!image.data) { printf("Error loading image \n"); return -1; }

	namedWindow("image", CV_WINDOW_AUTOSIZE);

	//Create 'Contrast' & 'Brightness' trackbars
	createTrackbar("Contrast", "image", &alpha, alpha_MAX, on_Trackbar);
	createTrackbar("Brightness", "image", &beta, beta_MAX, on_Trackbar);

	on_Trackbar(0); //Callback Function
	waitKey(0);

	return 0;
}



