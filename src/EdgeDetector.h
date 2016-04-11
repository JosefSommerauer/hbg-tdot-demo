#ifndef EDGEDETECTOR_H
#define EDGEDETECTOR_H

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

class EdgeDetector {
public:
	EdgeDetector(int calc_res_w, int calc_res_h, int display_res_w, int display_res_h);

	cv::Mat & doit(cv::Mat & frame, bool gpu = true);
private:
	int mCalcResW; //384; //320;
	int mCalcResH; //240; //200;
	int mDisplayResW;
	int mDisplayResH;

	static const int filter_size = 7;
	pfc::RGB_3_t color;

	double t0, t_start, t_upload, t_calc, t_download, t_total;

	cv::Mat edges, res;
	cv::cuda::GpuMat d_edges, d_edges_u, d_edges_f;

	cv::Ptr<cv::cuda::Filter> gauss;
	cv::Ptr<cv::cuda::CannyEdgeDetector> canny;

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
};

#endif // EDGEDETECTOR_H
