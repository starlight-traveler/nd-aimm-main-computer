// example_serializer.h
#ifndef EXAMPLE_SERIALIZER_H
#define EXAMPLE_SERIALIZER_H

#include "serial_abstract.h"
#include "deserializer_object.h"
#include "monster.h" // Internal::Monster
#include <flatbuffers/flatbuffers.h>
#include <memory>

class MonsterSerializer : public SerialAbstract, public DeserialAbstract
{
public:
    void Serialize(flatbuffers::FlatBufferBuilder &builder) override;
    std::unique_ptr<DeserializedObject> Deserialize(const uint8_t *buffer) override; // Correct return type
};

#endif // EXAMPLE_SERIALIZER_H
