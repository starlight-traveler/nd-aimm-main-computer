#ifndef PIPELINE_SETUP_H
#define PIPELINE_SETUP_H

#include "quill/Logger.h"
#include "depthai/depthai.hpp"
#include "inference.hpp"
#include "thread_safe_queue.hpp"

extern int setup_pipeline(quill::Logger *logger);
void orchestrationThreadLRCamera(quill::Logger *logger, ThreadSafeQueue<cv::Mat> &displayQueue);
dai::Pipeline setupPipeline(quill::Logger *logger);
std::shared_ptr<dai::node::ColorCamera> setupCamera(dai::Pipeline &pipeline, dai::CameraBoardSocket socket, const std::string &name);
std::shared_ptr<dai::node::StereoDepth> setupStereoDepthNode(dai::Pipeline &pipeline);
std::shared_ptr<dai::SpatialLocationCalculatorConfigData>  setupSpatialCalcConfig();
void linkStereoDepthNodes(std::shared_ptr<dai::node::ColorCamera> left, std::shared_ptr<dai::node::ColorCamera> right, std::shared_ptr<dai::node::ColorCamera> center, std::shared_ptr<dai::node::StereoDepth> depthNode,
                          std::shared_ptr<dai::node::XLinkOut> xoutStereo, std::shared_ptr<dai::node::XLinkOut> xoutCamera, quill::Logger *logger);
std::shared_ptr<dai::node::SpatialLocationCalculator> setupSpatialCalculator(dai::SpatialLocationCalculatorConfigData config, dai::Pipeline &pipeline,
                                                                             std::shared_ptr<dai::node::XLinkOut> xoutStereo, std::shared_ptr<dai::node::StereoDepth> depthNode);
void linkSpatialCalculationNodes(std::shared_ptr<dai::node::StereoDepth> depthNode, std::shared_ptr<dai::node::SpatialLocationCalculator> spatialCalculator, std::shared_ptr<dai::node::XLinkOut> xoutSpatialData, dai::Pipeline &pipeline);
cv::Mat processDepthFrame(std::shared_ptr<dai::ImgFrame> inDepthPtr);
void annotateDepthFrame(cv::Mat &depthFrameColor, std::shared_ptr<dai::SpatialLocationCalculatorData> spatialDataPtr);
void processRgbFrame(cv::Mat &frame, std::shared_ptr<dai::ImgFrame> inRgb, Inference &inf, const cv::Mat &depthFrame, std::shared_ptr<dai::SpatialLocationCalculatorData> spatialDataPtr, quill::Logger *logger);
void updateSpatialCalcConfig(dai::SpatialLocationCalculatorConfigData &config, std::shared_ptr<dai::DataInputQueue> spatialCalcConfigInQueue);
void processFrames(std::shared_ptr<dai::DataOutputQueue> depthQueue, std::shared_ptr<dai::DataOutputQueue> spatialCalcQueue, std::shared_ptr<dai::DataOutputQueue> rgbQueue, std::shared_ptr<dai::DataInputQueue> spatialCalcConfigInQueue, Inference inf, ThreadSafeQueue<cv::Mat> &displayQueue, quill::Logger *logger);
float getDepthFromBox(const cv::Rect &box, const cv::Mat &depthFrame, const cv::Mat &frame, std::shared_ptr<dai::SpatialLocationCalculatorData> spatialDataPtr, quill::Logger *logger);

void caller();
#endif // PIPELINE_SETUP_H
