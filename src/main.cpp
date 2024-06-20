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

Use it accordingly.

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
#include "flatbuffers/flatbuffers.h"


#include "version.h"
#include "logger.h"
#include "pipeline.h"
#include "udp.h"
#include "peripherals.h"
#include "inference.h"
#include "serializer_factory.h"
#include "monster_generated.h"

using namespace MyGame::Sample;

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

    // Get and test some scalar types from the FlatBuffer.
    assert(monster->hp() == 80);
    assert(monster->mana() == 150); // default
    assert(monster->name()->str() == "MyMonster");

    // Get and test a field of the FlatBuffer's `struct`.
    auto pos = monster->pos();
    assert(pos);
    assert(pos->z() == 3.0f);
    (void)pos;

    // Get a test an element from the `inventory` FlatBuffer's `vector`.
    auto inv = monster->inventory();
    assert(inv);
    assert(inv->Get(9) == 9);
    (void)inv;

    // Get and test the `weapons` FlatBuffers's `vector`.
    std::string expected_weapon_names[] = {"Sword", "Axe"};
    short expected_weapon_damages[] = {3, 5};
    auto weps = monster->weapons();
    for (unsigned int i = 0; i < weps->size(); i++)
    {
      assert(weps->Get(i)->name()->str() == expected_weapon_names[i]);
      assert(weps->Get(i)->damage() == expected_weapon_damages[i]);
    }
    (void)expected_weapon_names;
    (void)expected_weapon_damages;

    // Get and test the `Equipment` union (`equipped` field).
    assert(monster->equipped_type() == Equipment_Weapon);
    auto equipped = static_cast<const Weapon *>(monster->equipped());
    assert(equipped->name()->str() == "Axe");
    assert(equipped->damage() == 5);
    (void)equipped;

    printf("The FlatBuffer was successfully created and verified!\n");

  // ---------------

  /**
   * @brief Build all system peripheral addresses to contact
   * 
   * Initalizes a range of system IPs that connect to outside devices for
   * specific system control. Returns a map of devices.
   */ 
  std::map<std::string, UDPClient> clients = buildPeripherals(logger);

  UDPClient &eth0_client = clients.at("eth0");
  eth0_client.sendMessageAsync("Hello, eth0!", 0, 0, logger);


  /**
   * @brief Start the camera pipeline
   *
   * Calls the setup_pipeline function to configure and start the camera
   * pipeline process. This function is expected to handle all aspects of the
   * camera setup including stream configuration.
   */
  setup_pipeline(logger);

  return 0;
}
