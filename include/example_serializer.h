#ifndef EXAMPLE_SERIALIZER_H
#define EXAMPLE_SERIALIZER_H

#include "serial_abstract.h"
#include "example_generated.h" // Include generated FlatBuffer schema header

class MonsterSerializer : public SerialAbstract
{
public:
    void Serialize(flatbuffers::FlatBufferBuilder &builder) override;
};

#endif // EXAMPLE_SERIALIZER_H
