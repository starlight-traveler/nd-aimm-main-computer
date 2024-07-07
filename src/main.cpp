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


// Built in headers
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


// Userland headers
#include "version.h"
#include "logger.h"
#include "pipeline.h"
#include "udp.h"
#include "peripherals.h"
#include "inference.h"
#include "serializer_factory.h"
#include "threading.h"
#include "example_generated.h"
#include "thread_safe_queue.h"

// Test headers
#include "fb_check_t.h"
#include "peripherals_t.h"

#include "gui.h"



/**
 * @brief Global variables should be kept to a minimum
 * 
 * Most variables which are global should be used for testing
 */

// Use atomic for synchronization across threads
// std::atomic<int> x(0);

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
  // quill::Logger *logger = initialize_logger();

  // LOG_DEBUG(logger, "Build date: {}", BUILD_DATE);
  // LOG_DEBUG(logger, "Project version: {}", PROJECT_VERSION);

  /**
   * @brief Start all necessary threading
   *
   * This section will start all the necessary threads, seperated in grouping
   */

  // // Neccessary threading queues
  // ThreadSafeQueue<cv::Mat> displayQueue;

  // // Camera threads
  // std::thread camera_thread(run_with_retry, logger, std::ref(displayQueue));
  // // Camera thread #1
  // // Camera thread #2
  // // Camera thread #3
  // // Camera thread #4
  // // Camera thread #5

  // // Worker threads
  // // Spatial Analysis thread
  // // Spatial Actor thread

  // // Misc. threads
  // std::thread log_thread(log_increment, std::ref(x), logger);
  // // Perhipheral Manager thread
  // // Emergency Shutoff Manager thread


  /**
   * @brief Start all tests if necessary, should not be done in production
   * 
   * All tests should be run before GUI start up
   */
  // testing_flatbuffer::flatBufferGeneralTest(logger);
  // assert(testing_peripherals::peripheralTest(logger));

  /**
   * @brief GUI Stuff
   * 
   */
  entrance();

  /**
   * @brief Functions that will never return likely, if they do must be before
   * joins of things that will never finish
   *
   * @fn camera_thread will not return
   * @fn log_thread will not return
   */

  // // Wait for the camera thread to finish, if it ever does
  // camera_thread.join();

  // // The log thread will run indefinitely unless you provide a mechanism to stop it
  // log_thread.join();

  return 0;

}
