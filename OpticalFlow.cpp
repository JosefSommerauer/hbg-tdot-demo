#include "OpticalFlow.h"
#include "color_mapper.h"

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/cudaobjdetect.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/cudaarithm.hpp"
#include "opencv2/cudawarping.hpp"
#include "opencv2/cudafeatures2d.hpp"
#include "opencv2/cudafilters.hpp"
#include "opencv2/cudaoptflow.hpp"
#include "opencv2/cudabgsegm.hpp"


OpticalFlow::OpticalFlow(int calc_res_w, int calc_res_h, int display_res_w, int display_res_h) :
		mCalcResW(calc_res_w), mCalcResH(calc_res_h), 
		mDisplayResW(display_res_w), mDisplayResH(display_res_h) 
{
	mFarneback = cv::cuda::FarnebackOpticalFlow::create(1, 0.5, false, 13, 1, 7, 1.1, 0);
}

cv::Mat & OpticalFlow::doit(cv::Mat & frame, bool gpu) {
	double calc_start,   dl_start,   ul_start,   vis_start;
	double calc_time_ms, dl_time_ms, ul_time_ms, vis_time_ms, total_time_ms;

	// upload
	ul_start = (double) cv::getTickCount();
	
	if(!next_gpu.empty()) {
		prvs_gpu = next_gpu.clone(); 
	} else {
		//prvs_gpu.upload(cv::Mat::zeros(frame.rows / s, frame.cols / s, CV_8U));
		prvs_gpu.upload(cv::Mat::zeros(mCalcResH, mCalcResW, CV_8U));
	}
	
	next_gpu_o.upload(frame);

	//cv::cuda::resize(next_gpu_o, next_gpu_c, cv::Size(frame.size().width / s, frame.size().height / s));
	cv::cuda::resize(next_gpu_o, next_gpu_c, cv::Size(mCalcResW, mCalcResH));
	cv::cuda::cvtColor(next_gpu_c, next_gpu, CV_BGR2GRAY);

	ul_time_ms = ((double) cv::getTickCount() - ul_start) / cv::getTickFrequency() * 1000;

	// calc 
	calc_start = (double) cv::getTickCount();
	mFarneback->calc(prvs_gpu, next_gpu, d_flow);
	cv::resize(frame, result, cv::Size(mDisplayResW, mDisplayResH), 0, 0, cv::INTER_LINEAR); 
	calc_time_ms = ((double) cv::getTickCount() - calc_start) / cv::getTickFrequency() * 1000;

	// download
	dl_start = (double) cv::getTickCount();
	d_flow.download(h_flow);
	dl_time_ms = ((double) cv::getTickCount() - dl_start) / cv::getTickFrequency() * 1000;

	// visualize
	vis_start = (double) cv::getTickCount();
	drawOptFlowMap_gpu(h_flow, result, 5); 
	//cv::flip(result, result, 1); 
	vis_time_ms = ((double) cv::getTickCount() - vis_start) / cv::getTickFrequency() * 1000;

	return result;
}

void OpticalFlow::drawOptFlowMap_gpu(const cv::Mat& flow_xy, cv::Mat& cflowmap, int step) {
	double const l_threshold = 0;
	double scale_x = (cflowmap.cols / flow_xy.cols);
	double scale_y = (cflowmap.rows / flow_xy.rows);
	double dy, dx, l;
	int phi;

	pfc::RGB_3_t color;

	for (int y = 0; y < flow_xy.rows; y += step) {
		for (int x = 0; x < flow_xy.cols; x += step)	{  
			cv::Point2f fxy;  
			fxy.x = cvRound(flow_xy.at< cv::Vec2f >(y, x)[0] + x);  
			fxy.y = cvRound(flow_xy.at< cv::Vec2f >(y, x)[1] + y);

			dx = x - fxy.x;
			dy = y - fxy.y;

			l = std::sqrt(dx*dx + dy*dy);
			phi = (atan(dy/dx) * 255 / PI);

			color = pfc::get_rgb_smooth<pfc::RGB_3_t>(phi, 255);

			if ((l > l_threshold)) {  
				cv::line(cflowmap, cv::Point(x*scale_x, y*scale_y), 
							cv::Point(fxy.x*scale_x, fxy.y*scale_y), cv::Scalar(color.red, color.green, color.blue), 2);  
				cv::circle(cflowmap, cv::Point(fxy.x*scale_x, fxy.y*scale_y), 
							3, cv::Scalar(color.red, color.green, color.blue), 2);
			}
		}
	}  
}
