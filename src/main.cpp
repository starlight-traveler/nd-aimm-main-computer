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
#include <pybind11/embed.h>
#include <dexode/EventBus.hpp>
#include <csignal>

// Userland headers
#include "version.h"
#include "logger.h"
#include "pipeline.h"
#include "udp.h"
#include "peripherals.h"
#include "inference.hpp"
#include "serializer_factory.h"
#include "threading.h"
#include "example_generated.h"
#include "thread_safe_queue.hpp"
#include "thread_safe_queue_network.hpp"
#include "rns_sender.h"
#include "rns_manager.h"
#include "ring_buffer.h"
#include "initializer.h"
#include "emergency.h"
#include "events.h"
#include "emergency_listener.h"

// Test headers
#include "fb_check_t.h"
#include "peripherals_t.h"
#include "a_star_check_t.h"

#include "gui.h"
#include "r_tree.h"

// Set namespaces, typedefs, and usings

typedef RTree<int *, double, 3> MyTree;

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

  // ----- Must Happen Before Runtime ----- //

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
  quill::Logger *logger = initialize_logger();

  auto eventBus = std::make_shared<EventBus>();
  auto listener = std::make_shared<EmergencyListener>(logger, eventBus);

  LOG_DEBUG(logger, "Build date: {}", BUILD_DATE);
  LOG_DEBUG(logger, "Project version: {}", PROJECT_VERSION);

  // ----- Non-Threaded ----- //

  primary_initialization();

  // FIXME: RNS sender
  // rns_sender_manager(logger);
  py::gil_scoped_release release;

  /**
   * @brief Start all tests to make sure functionality exits atomically
   *
   * All tests should run to make sure no threading issues or post-compilation
   * issues occur.
   */
  testing_flatbuffer::flatBufferGeneralTest(logger);
  testing_peripherals::peripheralTest(logger);
  testing_a_star::a_star_runner(logger);

  /**
   * @brief Start all necessary threading
   *
   * This section will start all the necessary threads, seperated in grouping
   */

  // ----- Threading Structures ----- //

  // Threading
  ThreadSafeQueue<cv::Mat> displayQueue;

  // RNS Sender
  ThreadSafeQueueNetwork<std::tuple<std::string, std::string, std::string>> dataNetwork;


  // ----- Camera Threads----- //
  std::thread camera_thread([&]()
                            { threaded(logger, 5, 3, orchestrationThreadLRCamera, logger, std::ref(displayQueue)); }); 
  // TODO: Camera thread #1
  // TODO: Camera thread #2
  // TODO: Camera thread #3
  // TODO: Camera thread #4
  // TODO: Camera thread #5

  // ----- Worker ----- //
  // TODO: Spatial Analysis thread
  // TODO: Spatial Actor thread
  // TODO: MavLink Command Forwarder

  // ----- Misc. Threads ----- //

  // std::thread log_thread(log_increment, std::ref(x), logger);

  // FIXME: std::thread network_sender([&]()
  //                           { rns_sender_manager(dataNetwork); });

  std::thread events([&]()
                    { threaded(logger, 5, 3, event_processor, std::ref(eventBus)); });

  // LOG_INFO(logger, "Events thread on!");                  

     std::thread emergency([&listener, &eventBus]() {
        // Here, the listener simply exists and does its job in the background
        while (listener) {  // Check if listener is still alive
            eventBus->process();
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work and prevent tight loop
        }
    });


  LOG_INFO(logger, "Emergency thread on!");

        eventBus->postpone(event::EmergencyShutoff{});
	      eventBus->process();


  // TODO: Perhipheral manager thread
  // TODO: Heartbeat thread


  /**
   * @brief GUI Entrance
   * 
   * All GUI systems should be handled in this function, the GUI should not
   * be exposed anywhere else. If the GUI needs to be contexted, then it 
   * should be done in a threaded manner.
   */

  entrance(logger);

  /**
   * @brief Functions that will never return likely, if they do must be before
   * joins of things that will never finish
   *
   * @fn camera_thread will not return
   * @fn log_thread will not return
   * @fn network_sender will not return
   */

  // camera_thread.join();
  // log_thread.join();
  // network_sender.join();

    return 0;
}

/*
Examples of how to do stuff:

// ---- Pushing To Network ---- //

dataQueueNetwork.push(std::make_tuple(getRNSHexID("nd-aimm"), "/Users////.reticulum", "Hello, Reticulum!"));


*/
