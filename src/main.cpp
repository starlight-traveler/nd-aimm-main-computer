/*
================================================================================
 ____  _____  ______              _       _____  ____    ____  ____    ____
|_   \|_   _||_   _ `.           / \     |_   _||_   \  /   _||_   \  /   _|
  |   \ | |    | | `. \ ______  / _ \      | |    |   \/   |    |   \/   |
  | |\ \| |    | |  | ||______|/ ___ \     | |    | |\  /| |    | |\  /| |
 _| |_\   |_  _| |_.' /      _/ /   \ \_  _| |_  _| |_\/_| |_  _| |_\/_| |_
|_____|\____||______.'      |____| |____||_____||_____||_____||_____||_____|
================================================================================
Authors: The Notre Dame Artifical Intelligence Team
Date: June 4th, 2024

*/

/*

Note: All functions must have applicable documentation for future use.

This utilizes a thread pool model, trunk based development model.

Use it well.

*/

#include <memory>
#include <opencv2/opencv.hpp>
#include "depthai/depthai.hpp"
#include <string>
#include <utility>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <atomic>
#include <chrono>
#include <thread>
#include <iostream>
#include "flatbuffers/flatbuffers.h"
#include "quill/LogMacros.h"

#include "version.h"
#include "logger.h"
#include "pipeline.h"
#include "udp.h"
#include "peripherals.h"
#include "inference.h"
#include "serializer_factory.h"
#include "example_generated.h"
#include "thread_safe_queue.h"

using namespace MyGame::Sample;

/**
 * @brief Function to increment and log values in a loop
 */
void log_increment(std::atomic<int> &x, quill::Logger *logger)
{
  while (true)
  {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int val = ++x;
    LOG_INFO(logger, "Number: {}", val);
  }
}

void run_with_retry(quill::Logger *logger, ThreadSafeQueue<cv::Mat> &displayQueue)
{
  while (true)
  {
    try
    {
      orchestrationThreadLRCamera(logger, displayQueue);
      break; // Exit loop if setup_pipeline succeeds
    }
    catch (...)
    {
      // LOG_ERROR(logger, "Caught runtime error: {}", e.what());
      LOG_INFO(logger, "Retrying in 5 seconds...");
      std::this_thread::sleep_for(std::chrono::seconds(5));
    }
  }
}

/**
 * 
 * @fn int main()
 * @brief Main function to enter the program, should be classified as main thread
 *
 * Initializes the system execution and should be considerd as primary
 * "runtime" thread.
 * @return Should never return any value, and run infintely.
 */
int main()
{


  /**
   * @brief Initialize the system logger
   *
   * Initializes the system logger to capture and log system events. Uses the
   * initialize_logger function to configure and obtain a logger instance.
   */
  quill::Logger *logger = initialize_logger();

  LOG_DEBUG(logger, "Build date: {}", BUILD_DATE);
  LOG_DEBUG(logger, "Project version: {}", PROJECT_VERSION);

    flatbuffers::FlatBufferBuilder builder;
    SerialAbstract* serializer = SerializerFactory::GetSerializer(ObjectType::Monster);

    LOG_TRACE_L1(logger, "Sucessfully serialized monster Object.");

    if (serializer) {
        serializer->Serialize(builder);
        // Send buffer over UDP, handle network communication, etc.
        delete serializer;
    }

    // Get access to the root:
    auto monster = GetMonster(builder.GetBufferPointer());

    /**
     * @brief Build all system peripheral addresses to contact
     *
     * Initalizes a range of system IPs that connect to outside devices for
     * specific system control. Returns a map of devices.
     */
    std::map<std::string, UDPClient> clients = buildPeripherals(logger);

    UDPClient &eth0_client = clients.at("eth0");
    eth0_client.sendMessageAsyncSerialized(builder, logger);

    ThreadSafeQueue<cv::Mat> displayQueue;

    /**
     * @brief Start the camera pipeline
     *
     * Calls the setup_pipeline function to configure and start the camera
     * pipeline process. This function is expected to handle all aspects of the
     * camera setup including stream configuration.
     */
    std::thread camera_thread(run_with_retry, logger, std::ref(displayQueue)); // Pass the address of logger

    // setup_pipeline(logger);

    // Use atomic for synchronization across threads
    std::atomic<int> x(0);

    // Start the logging and incrementing loop in a separate thread
    std::thread log_thread(log_increment, std::ref(x), logger);

    // Wait for the camera thread to finish, if it ever does
    camera_thread.detach();

    // The log thread will run indefinitely unless you provide a mechanism to stop it
    log_thread.join();

    // Main thread loop for display
    while (true)
    {
      cv::Mat frame;
      if (displayQueue.tryPop(frame))
      {
        cv::imshow("Display Window", frame);
        if (cv::waitKey(1) >= 0)
          break;
      } else {
        LOG_ERROR(logger, "Nothing to display.");
      }

      
    }

    return 0;
}