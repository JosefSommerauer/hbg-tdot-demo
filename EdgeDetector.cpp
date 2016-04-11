#include "EdgeDetector.h"
#include "color_mapper.h"

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


EdgeDetector::EdgeDetector(int calc_res_w, int calc_res_h, int display_res_w, int display_res_h) :
		mCalcResW(calc_res_w), mCalcResH(calc_res_h), 
		mDisplayResW(display_res_w), mDisplayResH(display_res_h) 
{
	gauss = cv::cuda::createGaussianFilter(edges.type(), edges.type(), 
			cv::Size(filter_size, filter_size), 2.5, 2.5);

	canny = cv::cuda::createCannyEdgeDetector(30,3);
}

cv::Mat & EdgeDetector::doit(cv::Mat & frame, bool gpu) {

   t0 = (double) cv::getTickCount();

	// uplaod
	t_start = (double) cv::getTickCount();
	d_edges.upload(frame);
	t_upload = ((double) cv::getTickCount() - t_start) / cv::getTickFrequency();

	// calc
	t_start = (double) cv::getTickCount();
	cv::cuda::cvtColor(d_edges, d_edges, cv::COLOR_BGR2GRAY);
	gauss->apply(d_edges, d_edges);
	canny->detect(d_edges, d_edges);
	
	// downlaod
   t_start = (double) cv::getTickCount();
	d_edges.download(edges);
   t_download = ((double) cv::getTickCount() - t_start) / cv::getTickFrequency();

	res = cv::Mat::zeros(edges.rows, edges.cols, CV_8UC3);

	cv::findContours(edges, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_NONE, cv::Point(0,0) );
	cv::cvtColor(edges, edges, cv::COLOR_GRAY2BGR);
	for (unsigned int i=0; i<contours.size(); i++) {
		 //if (hierarchy[i][3] >= 0)   //has parent, inner (hole) contour of a closed edge (looks good)
		color = pfc::get_rgb_smooth<pfc::RGB_3_t>(i, contours.size());
		cv::drawContours(res, contours, i, cv::Scalar(color.red, color.green, color.blue), -1, 8);
	}

	cv::resize(res, res, cv::Size(mDisplayResW, mDisplayResH), 0, 0, cv::INTER_NEAREST);
   t_calc = ((double) cv::getTickCount() - t_start) / cv::getTickFrequency();

	// overlay info
   t_total    = ((double) cv::getTickCount() - t0) / cv::getTickFrequency();
	
	return res;
}
