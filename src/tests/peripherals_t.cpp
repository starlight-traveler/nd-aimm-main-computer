#include "peripherals_t.h"

namespace testing_peripherals
{

    bool peripheralTest(quill::Logger *logger)
    {
        auto clients = buildPeripherals(logger);
        if (!testClientInitialization(clients, "eth0", logger))
        {
            return false;
        }

        auto builder = testing_flatbuffer::flatBufferTestBuilder(logger);

        UDPClient &eth0_client = clients.at("eth0");

        return testSendValue(eth0_client, builder, logger);
    }

    bool testClientInitialization(const std::map<std::string, UDPClient> &clients, const std::string &clientKey, quill::Logger *logger)
    {
        try
        {
            const UDPClient &client = clients.at(clientKey);
            LOG_TRACE_L3(logger, "Client initialization tested successfully for {}", clientKey);
            return true;
        }
        catch (const std::out_of_range &e)
        {
            LOG_ERROR(logger, "Client initialization failed for {} : {}", clientKey, e.what());
            return false;
        }
    }

    bool testSendValue(UDPClient &client, flatbuffers::FlatBufferBuilder &builder, quill::Logger *logger)
    {
        try
        {
            client.sendMessageAsyncSerialized(builder, logger);
            LOG_TRACE_L3(logger, "Message sent successfully.");
            return true;
        }
        catch (const std::exception &e)
        {
            LOG_ERROR(logger, "Failed to send message: {}", std::string(e.what()));
            return false;
        }
    }

}
