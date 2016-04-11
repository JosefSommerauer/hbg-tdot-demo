#ifndef OPTICALFLOW_H
#define OPTICALFLOW_H

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/cudaobjdetect.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/cudaarithm.hpp"
#include "opencv2/cudafeatures2d.hpp"
#include "opencv2/cudafilters.hpp"
#include "opencv2/cudaoptflow.hpp"

#include "color_mapper.h"

#define PI 3.14159265

class OpticalFlow {
public:
	OpticalFlow(int calc_res_w, int calc_res_h, int display_res_w, int display_res_h);

	cv::Mat & doit(cv::Mat & frame, bool gpu = true);
private:
	void drawOptFlowMap_gpu(const cv::Mat& flow_xy, cv::Mat& cflowmap, int step);

	int mCalcResW; //384; //320;
	int mCalcResH; //240; //200;
	int mDisplayResW;
	int mDisplayResH;

	const int s = 4; 

	cv::Ptr<cv::cuda::FarnebackOpticalFlow> mFarneback;

	// pointers to the GpuMats are used to allow fast swapping of last and new images
	/*
	cv::cuda::GpuMat mGpuImg1;
	cv::cuda::GpuMat mGpuImg2;
	cv::cuda::GpuMat *mNowGpuImg, *mLastGpuImg;
	cv::Mat GetImg;
	*/

	cv::cuda::GpuMat prvs_gpu, next_gpu, flow_x_gpu, flow_y_gpu, flow_xy_gpu;  
	cv::cuda::GpuMat prvs_gpu_o, next_gpu_o;  
	cv::cuda::GpuMat prvs_gpu_c, next_gpu_c;  

	cv::Mat h_flow, result;
	cv::cuda::GpuMat d_flow;  
};

#endif // OPTICALFLOW_H
