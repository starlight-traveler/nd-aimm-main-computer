// serial_abstract.h
#ifndef SERIAL_ABSTRACT_H
#define SERIAL_ABSTRACT_H

#include <flatbuffers/flatbuffers.h>
#include "deserializer_object.h"
#include <memory>

class SerialAbstract
{
public:
    virtual void Serialize(flatbuffers::FlatBufferBuilder &builder) = 0;
    virtual ~SerialAbstract() {}
};

class DeserialAbstract
{
public:
    virtual std::unique_ptr<DeserializedObject> Deserialize(const uint8_t *buffer) = 0;
    virtual ~DeserialAbstract() {}
};

#endif // SERIAL_ABSTRACT_H
