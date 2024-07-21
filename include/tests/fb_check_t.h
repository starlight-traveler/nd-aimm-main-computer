#include "flatbuffers/flatbuffers.h"
#include "logger.h"

#include "serializer_factory.h"
#include "example_generated.h"

namespace testing_flatbuffer
{
    bool flatBufferGeneralTest(quill::Logger *logger);
    flatbuffers::FlatBufferBuilder flatBufferTestBuilder(quill::Logger *logger);
    void flatBufferExampleCheck(const MyGame::Sample::Monster *monster, quill::Logger *logger);
}