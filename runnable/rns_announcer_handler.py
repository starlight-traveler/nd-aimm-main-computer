import argparse
import random
import RNS
import time
import sys

APP_NAME = "aimm"

sys.path.insert(0, '../build')

import quill_logger

# Global variable to store the latest hex ID
last_received_hex_id = None

# Singleton instance of Reticulum
reticulum_instance = None

class AIMMAnnounceHandler:
    def __init__(self, aspect_filter=None, additional_data=None):
        self.aspect_filter = aspect_filter
        self.additional_data = additional_data

    def received_announce(self, destination_hash, announced_identity, app_data):
        global last_received_hex_id
        # Store the hex ID
        last_received_hex_id = RNS.prettyhexrep(destination_hash)
        quill_logger.log_message("Received an announce from " + last_received_hex_id, quill_logger.LogLevel.Info)
        
        if app_data:
            quill_logger.log_message("The announce contained the following app data: " + app_data.decode("utf-8"), quill_logger.LogLevel.Info)

def receive(configpath, data):
    reticulum = RNS.Reticulum(configpath)
    identity = RNS.Identity().from_file("/Users/rpaillet/Identities/new_identity")

    destination = RNS.Destination(
        identity, RNS.Destination.IN, RNS.Destination.SINGLE, APP_NAME, "announcer", "idverify")
    
    destination_checker = RNS.Destination(
        identity, RNS.Destination.IN, RNS.Destination.SINGLE, APP_NAME, "responder", "idverify")

    destination.set_proof_strategy(RNS.Destination.PROVE_ALL)
    destination_checker.set_proof_strategy(RNS.Destination.PROVE_ALL) 
    
    announce_handler = AIMMAnnounceHandler(aspect_filter="aimm.announcer.idverify", additional_data=data)
    RNS.Transport.register_announce_handler(announce_handler)
    
    announce_handler_responder = AIMMAnnounceHandler(aspect_filter="aimm.responder.idverify", additional_data=data)
    RNS.Transport.register_announce_handler(announce_handler_responder)
    
    idverify = "nd-aimm"
    destination.announce(app_data=idverify.encode("utf-8"))
    
    timeout = 10  # Set the timeout to 10 seconds
    elapsed_time = 0  # Counter to keep track of elapsed time

    while True:
        if last_received_hex_id:
            return last_received_hex_id
        elif elapsed_time >= timeout:
            return None  # Return None if 10 seconds have passed without receiving an ID
        else:
            time.sleep(1)  # Wait for 1 second
            elapsed_time += 1  # Increment the counter
