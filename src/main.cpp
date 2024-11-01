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
Date: Octiber 24th, 2024

*/

// Built In Headers (Including Library)
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
#include <pybind11/embed.h>
#include <dexode/EventBus.hpp>
#include <csignal>
#include "BS_thread_pool.hpp"

// Userland General Headers
#include "version.h"
#include "logger.h"
#include "pipeline.h"
#include "udp.h"
#include "peripherals.h"
#include "inference.hpp"
#include "serializer_factory.h"
#include "threading.h"
#include "rns_sender.h"
#include "initializer.h"
#include "emergency.h"
#include "emergency_listener.h"
#include "gui.h"

// Dameons
#include "mavlink_dameon.h"
#include "rns_manager.h"
#include "events.h"

// Specialized Data Structures
#include "r_tree.h"
#include "ring_buffer.h"
#include "thread_safe_queue.hpp"
#include "serial_data.h"

// Serialization Manual Schema
#include "monster.h" 

// Serialization Automated Schema
#include "example_generated.h" // Related to monster

// Test Headers
#include "fb_check_t.h"
#include "peripherals_t.h"
#include "a_star_check_t.h"
#include "thread_pool_t.h"

/**
 * @brief Set namespaces, typedefs, and usings
 * 
 */

namespace py = pybind11;

using EventBus = dexode::EventBus;
using Listener = dexode::EventBus::Listener;

/**
 * @brief Setup Signal Handlers
 * 
 * @param signum 
 */

void signalHandler(int signum)
{
  std::cout << "Interrupt signal (" << signum << ") received.\n";

  exit(signum);
}

/**
 * @brief Global variables should be kept to a minimum
 * 
 * Most variables which are global should be used for testing
 */

// Use atomic for synchronization across threads


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
  // ----- Pre-execution Runtime Initialization Phase ----- //

  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);
  std::signal(SIGSEGV, signalHandler);

  // Python Handler
  setenv("PYTHONPATH", "../runnable/", 1);
  py::scoped_interpreter guard{};

  /**
   * @brief Initialize the system
   *
   * Initializes the system logger to capture and log system events. Initializes
   * the signletons that need to be fired once.
   */

  // Initialize logger
  quill::Logger *logger = initialize_logger();

  // Initialize Events
  auto eventBus = std::make_shared<EventBus>();
  auto listener = std::make_shared<EmergencyListener>(logger, eventBus);

  // What is the current build?
  LOG_DEBUG(logger, "Build date: {}", BUILD_DATE);
  LOG_DEBUG(logger, "Project version: {}", PROJECT_VERSION);

#ifdef RUN_TESTS

  /**
   * @brief Start all tests to make sure functionality exits atomically
   *
   * All tests should run to make sure no threading issues or post-compilation
   * issues occur.
   */

  LOG_INFO(logger, "Running tests...");

  bool fb_test = testing_flatbuffer::flatBufferGeneralTest(logger);
  bool peripherals_test = testing_peripherals::peripheralTest(logger);
  bool thread_pool_test = testing_threadpool::threadpool_test(logger);
  testing_a_star::a_star_runner(logger);

  if (fb_test && peripherals_test)
  {
    LOG_INFO(logger, "All tests passed successfully.");
    return 0; // Exit after successful tests
  }
  else
  {
    LOG_ERROR(logger, "Some tests failed.");
    return 1; // Exit with error code
  }

#else

  // ----- Single-Threaded Execution Context ----- //

  primary_initialization();

  py::gil_scoped_release release;

  /**
   * @brief Start all necessary threading
   *
   * This section will start all the necessary threads, separated in grouping
   */

  // ----- Multi-Threaded Execution Structures ----- //

  // Queing structures
  ThreadSafeQueue<cv::Mat> displayQueue;
  ThreadSafeQueue<std::pair<std::string, FlatBufferData>> dataQueueSend;

  // ----- Multi-Threaded Execution Context ----- //

  // TODO: Imaging Thread Pool
  // BS::thread_pool pool;

  // TODO: Camera Threads (~6 must spawn)
  // std::thread camera_results([&]()
  //                           { threaded(logger, 5, 3, orchestrationThreadLRCamera, logger, std::ref(displayQueue)); });

  // TODO: Spatial Analysis Threads
  // std::thread spatial_analysis([&]()
  //                           { threaded(logger, 5, 3, orchestrationThreadLRCamera, logger, std::ref(displayQueue)); });

  // std::thread spatial_decision([&]()
  //                           { threaded(logger, 5, 3, orchestrationThreadLRCamera, logger, std::ref(displayQueue)); });

  // TODO: Mavlink
  std::thread mavlink([&]()
                            { threaded(logger, 5, 3, mavlink_dameon, logger); });

  // TODO: Networking
  std::thread network_dameon([&]()
                             { threaded(logger, 5, 3, rnsd_daemon, logger); });

  std::thread network_sender([&]()
                             { threaded(logger, 5, 3, rns_sender_manager, std::ref(dataQueueSend), logger); });

  // std::thread network_reciever([&]()
  //                            { threaded(logger, 5, 3, rns_reciever_manager, logger); });

  // Events
  std::thread events([&]()
                    { threaded(logger, 5, 3, event_processor, std::ref(eventBus)); });

  // Emergency
  std::thread emergency([&]()
                    { threaded(logger, 5, 3, emergency_instance, logger, std::ref(listener)); });

  // TODO: Peripherals
  // std::thread peripheral([&]()
  //                   { threaded(logger, 5, 3, emergency_instance, logger, std::ref(listener)); });

  // TODO: Heartbeat
  // std::thread heartbeat([&]()
  //                   { threaded(logger, 5, 3, heartbeat, logger, eventBus); });


  /**
   * @brief GUI Entrance
   *
   * All GUI systems should be handled in this function, the GUI should not
   * be exposed anywhere else. If the GUI needs to be contexted, then it
   * should be done in a threaded manner.
   */

  // GUI
  gui(logger);

  /**
   * @brief Keep the main therad alive indefintely, though it will never get
   * to this point.
   */

  while (true)
  {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

#endif

  return 0;
}