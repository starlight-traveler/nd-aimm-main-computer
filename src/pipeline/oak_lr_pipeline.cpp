#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <getopt.h>

#include "pipeline.h"
#include "quill/LogMacros.h"

static constexpr float stepSize = 0.05f;

static std::atomic<bool> newConfig{false};

// Flag setup for stereo depth calculations
bool extended_disparity = true; // Use extended disparity range (increases resolution of disparity map at the cost of performance)
bool subpixel = true;           // Enable sub-pixel accuracy in disparity map
bool lr_check = false;          // Enable left-right consistency check
bool enabledRectified = false;  // Enable rectified images (unused in this code)

auto topLeft = dai::Point2f(0.4, 0.4);
auto bottomRight = dai::Point2f(0.6, 0.6);

void orchestrationThreadLRCamera(quill::Logger *logger, ThreadSafeQueue<cv::Mat> &displayQueue)
{

    // Yolo model inferencing, if this fails what is the point in setting up anything else?
    Inference inf("models/yolov8m.onnx", cv::Size(640, 480), "classes.txt", false);

    // Setup pipeline and linker
    dai::Pipeline pipeline = setupPipeline(logger);

    // Initialize the device with the pipeline and set the USB speed
    dai::Device device(pipeline, dai::UsbSpeed::SUPER);

    // Check and display bootloader version if available
    if (device.getBootloaderVersion())
    {
        LOG_DEBUG(logger, "Bootloader version: {}", device.getBootloaderVersion()->toString());
    }

    LOG_DEBUG(logger, "Device name: {} Product name: {}", device.getDeviceName(), device.getProductName());

    auto depthQueue = device.getOutputQueue("depth", 8, false);
    auto spatialCalcQueue = device.getOutputQueue("spatialData", 8, false);
    auto spatialCalcConfigInQueue = device.getInputQueue("spatialCalcConfig");
    auto qRgb = device.getOutputQueue("rgb", 8, false);

    while(true)
    {
        processFrames(depthQueue, spatialCalcQueue, qRgb, spatialCalcConfigInQueue, inf, displayQueue, logger);
    }
}

dai::Pipeline setupPipeline(quill::Logger *logger)
{
    dai::Pipeline pipeline;

    LOG_TRACE_L3(logger, "Started the pipeline.");

    // Create and configure camera nodes
    auto leftCamera = setupCamera(pipeline, dai::CameraBoardSocket::CAM_A, "left");
    auto centerCamera = setupCamera(pipeline, dai::CameraBoardSocket::CAM_B, "center");
    auto rightCamera = setupCamera(pipeline, dai::CameraBoardSocket::CAM_C, "right");

    LOG_TRACE_L3(logger, "Finished setting up the cameras.");

    // Create output nodes to send the disparity maps to the host via USB
    auto xout_stereoDepth = pipeline.create<dai::node::XLinkOut>();
    auto xout_rawCamera = pipeline.create<dai::node::XLinkOut>();
    auto xin_SpatialCalcConfig = pipeline.create<dai::node::XLinkIn>();
    auto xout_SpatialData = pipeline.create<dai::node::XLinkOut>();

    try
    {
        xout_stereoDepth->setStreamName("depth");
    }
    catch (...)
    {
        LOG_ERROR(logger, "Error thrown at xStereoStream.");
    }

    try
    {
        xin_SpatialCalcConfig->setStreamName("spatialCalcConfig");
    }
    catch (...)
    {
        LOG_ERROR(logger, "Error thrown at spatialCalcConfigStream.");
    }

    try
    {
        xout_rawCamera->setStreamName("rgb");
    }
    catch (...)
    {
        LOG_ERROR(logger, "Error thrown at rgbStream.");
    }  

    try
    {
        xout_SpatialData->setStreamName("spatialData");
    }
    catch (...)
    {
        LOG_ERROR(logger, "Error thrown at spatialDataStream.");
    }

    LOG_TRACE_L3(logger, "Finished setting up the pipeline linkers.");

    // Setup the calculators and configs for spatial data
    auto stereoDepth = setupStereoDepthNode(pipeline);
    auto config = setupSpatialCalcConfig();
    auto spatialDataCalculator = setupSpatialCalculator(*config, pipeline, xout_stereoDepth, stereoDepth);

    LOG_TRACE_L3(logger, "Finished setting up the stereo dameon.");

    // Spatial Data Calculator Config Linker
    xin_SpatialCalcConfig->out.link(spatialDataCalculator->inputConfig);

    LOG_TRACE_L3(logger, "Finished setting the DataCalculatorConfig linker.");

    // These are the linker processes
    linkStereoDepthNodes(leftCamera, rightCamera, centerCamera, stereoDepth, xout_stereoDepth, xout_rawCamera, logger);

    LOG_TRACE_L3(logger, "Finished setting up the stereo nodes linker.");

    linkSpatialCalculationNodes(stereoDepth, spatialDataCalculator, xout_SpatialData, pipeline);

    LOG_TRACE_L3(logger, "Finished linking the spatial nodes.");

    // Do we need to return the pipeline? Is there a usecase
    return pipeline;
}

std::shared_ptr<dai::node::ColorCamera> setupCamera(dai::Pipeline &pipeline, dai::CameraBoardSocket socket, const std::string &name)
{

    if (name != "center") {
        auto camera = pipeline.create<dai::node::ColorCamera>();
        camera->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
        camera->setIspScale(2, 3);
        camera->setCamera(name);
        return camera;
    } else {
        auto camera = pipeline.create<dai::node::ColorCamera>();
        camera->setPreviewSize(640, 320);
        camera->setInterleaved(false);
        camera->setBoardSocket(socket);
        camera->setColorOrder(dai::ColorCameraProperties::ColorOrder::RGB);
        camera->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
        camera->setIspScale(2, 3);
        camera->setCamera(name);
        return camera;
    }
}

std::shared_ptr<dai::node::StereoDepth> setupStereoDepthNode(dai::Pipeline &pipeline)
{
    auto stereoDepth = pipeline.create<dai::node::StereoDepth>();
    stereoDepth->setDefaultProfilePreset(dai::node::StereoDepth::PresetMode::HIGH_DENSITY);
    stereoDepth->initialConfig.setMedianFilter(dai::MedianFilter::MEDIAN_OFF);
    stereoDepth->setLeftRightCheck(lr_check);
    stereoDepth->setExtendedDisparity(extended_disparity);
    stereoDepth->setSubpixel(subpixel);

    return stereoDepth;
}

std::shared_ptr<dai::SpatialLocationCalculatorConfigData> setupSpatialCalcConfig()
{
    auto config = std::make_shared<dai::SpatialLocationCalculatorConfigData>();

    config->depthThresholds.lowerThreshold = 100;
    config->depthThresholds.upperThreshold = 10000;
    // Assuming topLeft and bottomRight are defined before or you need to define/pass them
    // e.g., dai::Point2f topLeft(0.0f, 0.0f);
    //       dai::Point2f bottomRight(1.0f, 1.0f);
    config->roi = dai::Rect(topLeft, bottomRight);

    return config;
}

void linkStereoDepthNodes(std::shared_ptr<dai::node::ColorCamera> left, std::shared_ptr<dai::node::ColorCamera> right, std::shared_ptr<dai::node::ColorCamera> center, std::shared_ptr<dai::node::StereoDepth> depthNode,
                          std::shared_ptr<dai::node::XLinkOut> xoutStereo, std::shared_ptr<dai::node::XLinkOut> xoutCamera, quill::Logger *logger)
{

    // Create linker
    left->isp.link(depthNode->left);
    right->isp.link(depthNode->right);
    depthNode->disparity.link(xoutStereo->input);
    center->preview.link(xoutCamera->input);

    LOG_TRACE_L3(logger, "Successfully set the stream names.");
    
}

std::shared_ptr<dai::node::SpatialLocationCalculator> setupSpatialCalculator(dai::SpatialLocationCalculatorConfigData config, dai::Pipeline &pipeline,
                                                                             std::shared_ptr<dai::node::XLinkOut> xoutStereo,
                                                                             std::shared_ptr<dai::node::StereoDepth> depthNode)
{
    auto spatialDataCalculator = pipeline.create<dai::node::SpatialLocationCalculator>();

    spatialDataCalculator->inputConfig.setWaitForMessage(false);
    spatialDataCalculator->initialConfig.addROI(config);
    spatialDataCalculator->passthroughDepth.link(xoutStereo->input);

    return spatialDataCalculator;
}

void linkSpatialCalculationNodes(std::shared_ptr<dai::node::StereoDepth> depthNode, std::shared_ptr<dai::node::SpatialLocationCalculator> spatialCalculator, std::shared_ptr<dai::node::XLinkOut> xoutSpatialData, dai::Pipeline &pipeline)
{
    depthNode->depth.link(spatialCalculator->inputDepth);
    spatialCalculator->out.link(xoutSpatialData->input);
}

void processFrames(std::shared_ptr<dai::DataOutputQueue> depthQueue, std::shared_ptr<dai::DataOutputQueue> spatialCalcQueue,
                   std::shared_ptr<dai::DataOutputQueue> rgbQueue, std::shared_ptr<dai::DataInputQueue> spatialCalcConfigInQueue,
                   Inference inf, ThreadSafeQueue<cv::Mat> &displayQueue, quill::Logger *logger)
{
    auto inDepthPtr = depthQueue->tryGet<dai::ImgFrame>();
    auto inRgb = rgbQueue->tryGet<dai::ImgFrame>();

    if (inDepthPtr && inRgb)
    {
        cv::Mat depthFrameColor = processDepthFrame(inDepthPtr);
        auto spatialDataPtr = spatialCalcQueue->tryGet<dai::SpatialLocationCalculatorData>();

        if (spatialDataPtr)
        {
            // annotateDepthFrame(depthFrameColor, spatialDataPtr);
            cv::Mat frame = inRgb->getCvFrame();
            processRgbFrame(frame, inRgb, inf, depthFrameColor, spatialDataPtr, logger);

            float scale = 0.8;
            cv::resize(frame, frame, cv::Size(frame.cols * scale, frame.rows * scale));

            // LOG_TRACE_L2(logger, "Pushing frame into queue!");

            // Push frames to display in the main thread
            displayQueue.push(frame);

            if (newConfig)
            {
                dai::SpatialLocationCalculatorConfigData config;
                updateSpatialCalcConfig(config, spatialCalcConfigInQueue);
            }
        }
    }
}

cv::Mat processDepthFrame(std::shared_ptr<dai::ImgFrame> inDepthPtr)
{
    cv::Mat depthFrame = inDepthPtr->getFrame(); // depthFrame values are in millimeters

    // cv::Mat depthFrameColor;
    // cv::normalize(depthFrame, depthFrameColor, 255, 0, cv::NORM_INF, CV_8UC1);
    // cv::equalizeHist(depthFrameColor, depthFrameColor);
    // cv::applyColorMap(depthFrameColor, depthFrameColor, cv::COLORMAP_HOT);

    return depthFrame;
}

void annotateDepthFrame(cv::Mat &depthFrameColor, std::shared_ptr<dai::SpatialLocationCalculatorData> spatialDataPtr)
{
    auto spatialData = spatialDataPtr->getSpatialLocations();

    for (const auto &depthData : spatialData)
    {
        auto roi = depthData.config.roi;
        roi = roi.denormalize(depthFrameColor.cols, depthFrameColor.rows);
        auto xmin = static_cast<int>(roi.topLeft().x);
        auto ymin = static_cast<int>(roi.topLeft().y);
        auto xmax = static_cast<int>(roi.bottomRight().x);
        auto ymax = static_cast<int>(roi.bottomRight().y);

        auto depthMin = depthData.depthMin;
        auto depthMax = depthData.depthMax;

        cv::Scalar color = cv::Scalar(255, 0, 0); // Example color, you may need to define it appropriately
        cv::rectangle(depthFrameColor, cv::Rect(cv::Point(xmin, ymin), cv::Point(xmax, ymax)), color, cv::FONT_HERSHEY_SIMPLEX);

        std::stringstream depthX;
        depthX << "X: " << static_cast<int>(depthData.spatialCoordinates.x) << " mm";
        cv::putText(depthFrameColor, depthX.str(), cv::Point(xmin + 10, ymin + 20), cv::FONT_HERSHEY_TRIPLEX, 0.5, color);
        std::stringstream depthY;
        depthY << "Y: " << static_cast<int>(depthData.spatialCoordinates.y) << " mm";
        cv::putText(depthFrameColor, depthY.str(), cv::Point(xmin + 10, ymin + 35), cv::FONT_HERSHEY_TRIPLEX, 0.5, color);
        std::stringstream depthZ;
        depthZ << "Z: " << static_cast<int>(depthData.spatialCoordinates.z) << " mm";
        cv::putText(depthFrameColor, depthZ.str(), cv::Point(xmin + 10, ymin + 50), cv::FONT_HERSHEY_TRIPLEX, 0.5, color);
    }
}

void processRgbFrame(cv::Mat &frame, std::shared_ptr<dai::ImgFrame> inRgb, Inference &inf, const cv::Mat &depthFrame, std::shared_ptr<dai::SpatialLocationCalculatorData> spatialDataPtr, quill::Logger *logger)
{
    std::vector<Detection> output = inf.runInference(frame);
    int detections = output.size();
    LOG_TRACE_L1(logger, "The number of detections is {}!", detections);

    for (const auto &detection : output)
    {
        cv::Rect box = detection.box;
        cv::Scalar color = detection.color;

        // Draw detection box
        cv::rectangle(frame, box, color, 2);

        // Get depth for the bounding box
        float depth = getDepthFromBox(box, depthFrame, frame, spatialDataPtr, logger);

        // Detection box text
        std::string classString = detection.className + ' ' + std::to_string(detection.confidence).substr(0, 4) + " Depth: " + std::to_string(depth);
        cv::Size textSize = cv::getTextSize(classString, cv::FONT_HERSHEY_DUPLEX, 1, 2, 0);
        cv::Rect textBox(box.x, box.y - 40, textSize.width + 10, textSize.height + 20);

        cv::rectangle(frame, textBox, color, cv::FILLED);
        cv::putText(frame, classString, cv::Point(box.x + 5, box.y - 10), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2, 0);
    }
}

float getDepthFromBox(const cv::Rect &box, const cv::Mat &depthFrame, const cv::Mat &frame, std::shared_ptr<dai::SpatialLocationCalculatorData> spatialDataPtr, quill::Logger *logger)
{
    // Calculate scaling ratios
    float scaleX = static_cast<float>(depthFrame.cols) / frame.cols;
    float scaleY = static_cast<float>(depthFrame.rows) / frame.rows;

    // Translate bounding box to depth frame coordinates
    cv::Rect depthBox;
    depthBox.x = static_cast<int>(box.x * scaleX);
    depthBox.y = static_cast<int>(box.y * scaleY);
    depthBox.width = static_cast<int>(box.width * scaleX);
    depthBox.height = static_cast<int>(box.height * scaleY);

    // Ensure the box is within frame boundaries
    depthBox.x = std::max(depthBox.x, 0);
    depthBox.y = std::max(depthBox.y, 0);
    depthBox.width = std::min(depthBox.width, depthFrame.cols - depthBox.x);
    depthBox.height = std::min(depthBox.height, depthFrame.rows - depthBox.y);

    // Compute the median depth within the bounding box to avoid the effect of outliers
    std::vector<uint16_t> depths;
    depths.reserve(depthBox.width * depthBox.height);

    for (int y = depthBox.y; y < depthBox.y + depthBox.height; ++y) {
        for (int x = depthBox.x; x < depthBox.x + depthBox.width; ++x) {
            auto depthValue = depthFrame.at<uint16_t>(y, x);
            if (depthValue != 0) {  // Filter out invalid measurements
                depths.push_back(depthValue);
            }
        }
    }

    float medianDepth = 0;
    if (!depths.empty()) {
        std::nth_element(depths.begin(), depths.begin() + depths.size() / 2, depths.end());
        medianDepth = depths[depths.size() / 2];
    }

    LOG_INFO(logger, "Median Depth Value: {}", medianDepth);
    return medianDepth;  // Return the median depth in millimeters
}

// Average
// {
//     // Calculate scaling ratios
//     float scaleX = static_cast<float>(depthFrame.cols) / frame.cols;
//     float scaleY = static_cast<float>(depthFrame.rows) / frame.rows;

//     // Translate bounding box to depth frame coordinates
//     cv::Rect depthBox;
//     depthBox.x = static_cast<int>(box.x * scaleX);
//     depthBox.y = static_cast<int>(box.y * scaleY);
//     depthBox.width = static_cast<int>(box.width * scaleX);
//     depthBox.height = static_cast<int>(box.height * scaleY);

//     // Ensure depth box does not exceed the depth frame boundaries
//     depthBox.x = std::max(depthBox.x, 0);
//     depthBox.y = std::max(depthBox.y, 0);
//     depthBox.width = std::min(depthBox.width, depthFrame.cols - depthBox.x);
//     depthBox.height = std::min(depthBox.height, depthFrame.rows - depthBox.y);

//     LOG_INFO(logger, "Adjusted Depth Box - x: {}, y: {}, width: {}, height: {}", depthBox.x, depthBox.y, depthBox.width, depthBox.height);

//     // Compute the average depth within the bounding box
//     double sumDepth = 0.0;
//     int count = 0;

//     for (int y = depthBox.y; y < depthBox.y + depthBox.height; ++y) {
//         for (int x = depthBox.x; x < depthBox.x + depthBox.width; ++x) {
//             auto depthValue = depthFrame.at<uint16_t>(y, x); // Assuming original depth frame type is CV_16U
//             if (depthValue != 0) {  // Check for valid depth value
//                 sumDepth += depthValue;
//                 ++count;
//             }
//         }
//     }


//     float averageDepth = count > 0 ? static_cast<float>(sumDepth) / count : 0;  // Compute average or return 0 if no data
//     LOG_INFO(logger, "The average depth value is: {}", averageDepth);
//     return averageDepth; // Return the average depth in millimeters
// }

void updateSpatialCalcConfig(dai::SpatialLocationCalculatorConfigData &config, std::shared_ptr<dai::DataInputQueue> spatialCalcConfigInQueue)
{
    config.roi = dai::Rect(topLeft, bottomRight);
    config.calculationAlgorithm = dai::SpatialLocationCalculatorAlgorithm::MEDIAN;
    dai::SpatialLocationCalculatorConfig cfg;
    cfg.addROI(config);
    spatialCalcConfigInQueue->send(cfg);
    newConfig = false;
}
