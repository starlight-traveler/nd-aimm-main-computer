from __future__ import annotations
import os
import sys
import time
import argparse
from datetime import datetime

sys.path.insert(0, '/Users/rpaillet/Documents/Projects/NDAIMM/nd-aimm/nd-aimm-main-computer/build')

import quill_logger

import RNS
from RNS.vendor import umsgpack

APP_NAME = "example_utilities"


##########################################################
#### Client Part #########################################
##########################################################

# A reference to the server link
server_link = None

# A reference to the buffer object, needed to share the
# object from the link connected callback to the client
# loop.
buffer = None

# This initialisation is executed when the users chooses
# to run as a client
def client(destination_hexhash, configpath, data):
    
    # We need a binary representation of the destination
    # hash that was entered on the command line
    try:
        dest_len = (RNS.Reticulum.TRUNCATED_HASHLENGTH//8)*2
        if len(destination_hexhash) != dest_len:
            quill_logger.log_message("Destination length is invalid, must be {hex} hexadecimal characters ({byte} bytes).", quill_logger.LogLevel.Error)
            raise ValueError(
                "Destination length is invalid, must be {hex} hexadecimal characters ({byte} bytes).".format(hex=dest_len, byte=dest_len//2)
            )
            
        destination_hash = bytes.fromhex(destination_hexhash)
    except:
        quill_logger.log_message("Invalid destination entered. Check your input!", quill_logger.LogLevel.Error)
        exit()

    # # We must first initialise Reticulum
    # reticulum = get_reticulum_instance(configpath)
    
    # Check if we know a path to the destination
    if not RNS.Transport.has_path(destination_hash):
        quill_logger.log_message("Destination is not yet known. Requesting path and waiting for announce to arrive...", quill_logger.LogLevel.Info)
        RNS.Transport.request_path(destination_hash)
        while not RNS.Transport.has_path(destination_hash):
            time.sleep(0.1)

    # Recall the server identity
    server_identity = RNS.Identity.recall(destination_hash)

    # Inform the user that we'll begin connecting
    quill_logger.log_message("Establishing link with server...", quill_logger.LogLevel.Info)

    # When the server identity is known, we set
    # up a destination
    server_destination = RNS.Destination(
        server_identity,
        RNS.Destination.OUT,
        RNS.Destination.SINGLE,
        APP_NAME,
        "bufferexample"
    )

    # And create a link
    link = RNS.Link(server_destination)

    # We'll also set up functions to inform the
    # user when the link is established or closed
    link.set_link_established_callback(link_established)
    link.set_link_closed_callback(link_closed)

    # Everything is set up, so let's enter a loop
    # for the user to interact with the example
    client_loop(data)

def client_loop(data):
    global server_link

    # Wait for the link to become active
    while not server_link:
        time.sleep(0.1)

    try:
        # Check if data is a string and encode it, otherwise assume it's already bytes
        if isinstance(data, str):
            data = data.encode("utf-8")

        buffer.write(data)
        # Flush the buffer to ensure the data is sent.
        buffer.flush()

        # After successfully sending data, you may choose to teardown the connection
        server_link.teardown()

    except Exception as e:
        quill_logger.log_message("Error while sending data over the link buffer: " + str(e), quill_logger.LogLevel.Error)
        server_link.teardown()

# This function is called when a link
# has been established with the server
def link_established(link):
    # We store a reference to the link
    # instance for later use
    global server_link, buffer
    server_link = link

    # Create buffer, see server_client_connected() for
    # more detail about setting up the buffer.
    channel = link.get_channel()
    buffer = RNS.Buffer.create_bidirectional_buffer(0, 0, channel, client_buffer_ready)

# When a link is closed, we'll inform the
# user, and exit the program
def link_closed(link):
    if link.teardown_reason == RNS.Link.TIMEOUT:
        quill_logger.log_message("The link timed out, exiting now", quill_logger.LogLevel.Info)
    elif link.teardown_reason == RNS.Link.DESTINATION_CLOSED:
        quill_logger.log_message("The link was closed by the server, exiting now", quill_logger.LogLevel.Info)
    else:
        quill_logger.log_message("Link closed, exiting now", quill_logger.LogLevel.Info)
    
    RNS.Reticulum.exit_handler()
    time.sleep(1.5)

# When the buffer has new data, read it and write it to the terminal.
def client_buffer_ready(ready_bytes: int):
    global buffer
    data = buffer.read(ready_bytes)
    quill_logger.log_message("Received data over the link buffer: " + data.decode("utf-8"), quill_logger.LogLevel.Info)
    print("> ", end=" ")
    sys.stdout.flush()