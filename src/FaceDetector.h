#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/cudaobjdetect.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/cudaarithm.hpp"

#include "Hut.h"

#include "faces.h"

class FaceDetector {
public:
	FaceDetector(std::string cascadefile, int calc_res_w, int calc_res_h, int display_res_w, int display_res_h);

	cv::Mat & doit(cv::Mat & frame, bool gpu = true, int hat_offset = 0,  bool debug = false);
private:
	// add png head to camerastream
	void overlayImage(cv::Mat* src, cv::Mat* overlay, const cv::Point& location);

	int mCalcResW; //384; //320;
	int mCalcResH; //240; //200;
	int mDisplayResW;
	int mDisplayResH;
	double mScaleW;
	double mScaleH;

	cv::Size mMinObjectSize;
	cv::Size mMaxObjectSize;

	Faces mFaces;

	static const int number_of_hats = 7;
	//Hut hats[number_of_hats];

	std::vector<Hut> hats;
	
	/*
	double hats_scale_factor[number_of_hats];
	double hats_offset_x[number_of_hats];
	double hats_offset_y[number_of_hats];
	*/

	/*
	static const double hats_scale_factor[number_of_hats] = {2.1, 1.0,1.0,1.5,1.5,1.2,1.7};
	static const double hats_offset_x[number_of_hats] =     {0.5,0.52,0.5,0.5,0.5,0.5,0.5};
	static const double hats_offset_y[number_of_hats] =     {0.9,-0.1,0.8,0.7,0.6,0.8,0.7};
	*/

	// cv vetors
	cv::Mat frame, frame_out, frame_fliped, frame_lowres, frame_gray, tmp;
  	cv::cuda::GpuMat d_frame, d_faces;
	std::vector<cv::Rect> faces;

	// CascadeClassifier
	std::string mFrontalfaceFile;
	cv::Ptr<cv::cuda::CascadeClassifier> mFaceCascade_gpu;
	cv::CascadeClassifier mFaceCascade_cpu;
};

#endif // EDGEDETECTOR_H
