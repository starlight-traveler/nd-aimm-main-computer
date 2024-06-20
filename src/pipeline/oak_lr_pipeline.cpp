#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <getopt.h>

#include "pipeline.h"
#include "quill/LogMacros.h"
#include "inference.h"

static constexpr float stepSize = 0.05f;

static std::atomic<bool> newConfig{false};

int setup_pipeline(quill::Logger* logger) {

    Inference inf("models/yolov8m.onnx", cv::Size(300, 300), "classes.txt", false);

    // Flag setup for stereo depth calculations
    bool extended_disparity = true; // Use extended disparity range (increases resolution of disparity map at the cost of performance)
    bool subpixel = true;           // Enable sub-pixel accuracy in disparity map
    bool lr_check = false;           // Enable left-right consistency check
    bool enabledRectified = false;  // Enable rectified images (unused in this code)
    bool newConfig = false;         // Flag to indicate a new configuration (unused in this code)

    auto topLeft = dai::Point2f(0.4, 0.4);
    auto bottomRight = dai::Point2f(0.6, 0.6);

    // Create a pipeline object that coordinates the flow of data between nodes
    dai::Pipeline pipeline;

    // Create camera nodes for capturing images from left, center, and right cameras
    auto left = pipeline.create<dai::node::ColorCamera>();
    auto center = pipeline.create<dai::node::ColorCamera>();
    auto right = pipeline.create<dai::node::ColorCamera>();

    // Create stereo depth nodes for calculating depth maps from pairs of cameras
    auto LR_depth = pipeline.create<dai::node::StereoDepth>();

    // Stereo Calculator Pipeline
    auto spatialDataCalculator = pipeline.create<dai::node::SpatialLocationCalculator>();

    // Create output nodes to send the disparity maps to the host via USB
    auto xout_LR = pipeline.create<dai::node::XLinkOut>();
    auto xoutRgb = pipeline.create<dai::node::XLinkOut>();

    center->setPreviewSize(300, 300);
    center->setBoardSocket(dai::CameraBoardSocket::CAM_B);
    center->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
    center->setInterleaved(false);
    center->setColorOrder(dai::ColorCameraProperties::ColorOrder::RGB);

    // Create an input node for receiving spatial location configuration from the host (unused in this code)
    auto xinSpatialCalcConfig = pipeline.create<dai::node::XLinkIn>();
    auto xoutSpatialData = pipeline.create<dai::node::XLinkOut>();

    // Set the output stream names for linking with external applications or other parts of the application
    xout_LR->setStreamName("depth");
    xinSpatialCalcConfig->setStreamName("spatialCalcConfig");
    xoutRgb->setStreamName("rgb");
    xoutSpatialData->setStreamName("spatialData");

    // Configure the resolution and camera settings for each ColorCamera node
    left->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
    left->setCamera("left");
    left->setIspScale(2, 3);

    center->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
    center->setCamera("center");
    center->setIspScale(2, 3);

    right->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
    right->setCamera("right");
    right->setIspScale(2, 3);

    // Lambda function to configure stereo depth settings consistently across different nodes
    auto setupStereoDepth = [&](std::shared_ptr<dai::node::StereoDepth> node)
    {
        node->setDefaultProfilePreset(dai::node::StereoDepth::PresetMode::HIGH_DENSITY);
        node->initialConfig.setMedianFilter(dai::MedianFilter::MEDIAN_OFF);
        node->setLeftRightCheck(lr_check);
        node->setExtendedDisparity(extended_disparity);
        node->setSubpixel(subpixel);
    };

    setupStereoDepth(LR_depth);

    left->isp.link(LR_depth->left);
    right->isp.link(LR_depth->right);
    LR_depth->disparity.link(xout_LR->input);
    center->preview.link(xoutRgb->input);

    // Config for spatial location calculator
    auto config = dai::SpatialLocationCalculatorConfigData();
    config.depthThresholds.lowerThreshold = 100;
    config.depthThresholds.upperThreshold = 10000;
    auto calculationAlgorithm = dai::SpatialLocationCalculatorAlgorithm::MEDIAN;
    config.roi = dai::Rect(topLeft, bottomRight);

    spatialDataCalculator->inputConfig.setWaitForMessage(false);
    spatialDataCalculator->initialConfig.addROI(config);

    spatialDataCalculator->passthroughDepth.link(xout_LR->input);
    LR_depth->depth.link(spatialDataCalculator->inputDepth);

    spatialDataCalculator->out.link(xoutSpatialData->input);
    xinSpatialCalcConfig->out.link(spatialDataCalculator->inputConfig);

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

    auto color = cv::Scalar(255, 255, 255);

        // Main loop to process and display incoming frames
        while (true) {

            auto inDepthPtr = depthQueue->tryGet<dai::ImgFrame>();
            auto inRgb = qRgb->get<dai::ImgFrame>();

            if (inDepthPtr)
            {
                cv::Mat depthFrame = inDepthPtr->getFrame(); // depthFrame values are in millimeters

                cv::Mat depthFrameColor;

                cv::normalize(depthFrame, depthFrameColor, 255, 0, cv::NORM_INF, CV_8UC1);
                cv::equalizeHist(depthFrameColor, depthFrameColor);
                cv::applyColorMap(depthFrameColor, depthFrameColor, cv::COLORMAP_HOT);

                auto spatialDataPtr = spatialCalcQueue->tryGet<dai::SpatialLocationCalculatorData>();

                if (spatialDataPtr)
                {
                    auto spatialData = spatialDataPtr->getSpatialLocations();

                    for (auto depthData : spatialData)
                    {
                        std::cout << "Hello World 2" << std::endl;
                        auto roi = depthData.config.roi;
                        roi = roi.denormalize(depthFrameColor.cols, depthFrameColor.rows);
                        auto xmin = (int)roi.topLeft().x;
                        auto ymin = (int)roi.topLeft().y;
                        auto xmax = (int)roi.bottomRight().x;
                        auto ymax = (int)roi.bottomRight().y;

                        auto depthMin = depthData.depthMin;
                        auto depthMax = depthData.depthMax;

                        cv::rectangle(depthFrameColor, cv::Rect(cv::Point(xmin, ymin), cv::Point(xmax, ymax)), color, cv::FONT_HERSHEY_SIMPLEX);
                        std::stringstream depthX;
                        depthX << "X: " << (int)depthData.spatialCoordinates.x << " mm";
                        cv::putText(depthFrameColor, depthX.str(), cv::Point(xmin + 10, ymin + 20), cv::FONT_HERSHEY_TRIPLEX, 0.5, color);
                        std::stringstream depthY;
                        depthY << "Y: " << (int)depthData.spatialCoordinates.y << " mm";
                        cv::putText(depthFrameColor, depthY.str(), cv::Point(xmin + 10, ymin + 35), cv::FONT_HERSHEY_TRIPLEX, 0.5, color);
                        std::stringstream depthZ;
                        depthZ << "Z: " << (int)depthData.spatialCoordinates.z << " mm";
                        cv::putText(depthFrameColor, depthZ.str(), cv::Point(xmin + 10, ymin + 50), cv::FONT_HERSHEY_TRIPLEX, 0.5, color);
                    }

                    // Show the frame
                    cv::Mat frame = inRgb->getCvFrame();

                    std::vector<Detection> output = inf.runInference(frame);

                    int detections = output.size();

                    LOG_TRACE_L1(logger, "The number of detections is {}!", detections);

                    for (int i = 0; i < detections; ++i)
                    {
                        Detection detection = output[i];

                        cv::Rect box = detection.box;
                        cv::Scalar color = detection.color;

                        // Detection box
                        cv::rectangle(frame, box, color, 2);

                        // Detection box text
                        std::string classString = detection.className + ' ' + std::to_string(detection.confidence).substr(0, 4);
                        cv::Size textSize = cv::getTextSize(classString, cv::FONT_HERSHEY_DUPLEX, 1, 2, 0);
                        cv::Rect textBox(box.x, box.y - 40, textSize.width + 10, textSize.height + 20);

                        cv::rectangle(frame, textBox, color, cv::FILLED);
                        cv::putText(frame, classString, cv::Point(box.x + 5, box.y - 10), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2, 0);
                    }

                    // This is only for preview purposes
                    float scale = 0.8;
                    cv::resize(frame, frame, cv::Size(frame.cols * scale, frame.rows * scale));
                    cv::imshow("Inference", frame);

                    cv::imshow("depth", depthFrameColor);

                    int key = cv::waitKey(1);
                    switch (key)
                    {
                    case 'q':
                        return 0;
                    case 'w':
                        if (topLeft.y - stepSize >= 0)
                        {
                            topLeft.y -= stepSize;
                            bottomRight.y -= stepSize;
                            newConfig = true;
                        }
                        break;
                    case 'a':
                        if (topLeft.x - stepSize >= 0)
                        {
                            topLeft.x -= stepSize;
                            bottomRight.x -= stepSize;
                            newConfig = true;
                        }
                        break;
                    case 's':
                        if (bottomRight.y + stepSize <= 1)
                        {
                            topLeft.y += stepSize;
                            bottomRight.y += stepSize;
                            newConfig = true;
                        }
                        break;
                    case 'd':
                        if (bottomRight.x + stepSize <= 1)
                        {
                            topLeft.x += stepSize;
                            bottomRight.x += stepSize;
                            newConfig = true;
                        }
                        break;
                    case '1':
                        calculationAlgorithm = dai::SpatialLocationCalculatorAlgorithm::MEAN;
                        newConfig = true;
                        std::cout << "Switching calculation algorithm to MEAN!" << std::endl;
                        break;
                    case '2':
                        calculationAlgorithm = dai::SpatialLocationCalculatorAlgorithm::MIN;
                        newConfig = true;
                        std::cout << "Switching calculation algorithm to MIN!" << std::endl;
                        break;
                    case '3':
                        calculationAlgorithm = dai::SpatialLocationCalculatorAlgorithm::MAX;
                        newConfig = true;
                        std::cout << "Switching calculation algorithm to MAX!" << std::endl;
                        break;
                    case '4':
                        calculationAlgorithm = dai::SpatialLocationCalculatorAlgorithm::MODE;
                        newConfig = true;
                        std::cout << "Switching calculation algorithm to MODE!" << std::endl;
                        break;
                    case '5':
                        calculationAlgorithm = dai::SpatialLocationCalculatorAlgorithm::MEDIAN;
                        newConfig = true;
                        std::cout << "Switching calculation algorithm to MEDIAN!" << std::endl;
                        break;
                    default:
                        break;
                    }

                    if (newConfig)
                    {
                        config.roi = dai::Rect(topLeft, bottomRight);
                        config.calculationAlgorithm = calculationAlgorithm;
                        dai::SpatialLocationCalculatorConfig cfg;
                        cfg.addROI(config);
                        spatialCalcConfigInQueue->send(cfg);
                        newConfig = false;
                    }
                } else {

                    LOG_ERROR(logger, "No spatial data received. Check pipeline configuration and ensure depth data is being sent.");
                } 
        }
        else
        {
            LOG_ERROR(logger, "Depth frame not available at this time.");
        }
        }
    return 0;
}