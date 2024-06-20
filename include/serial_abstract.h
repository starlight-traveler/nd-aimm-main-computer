#ifndef SERIALIZER_ABSTRACT_H
#define SERIALIZER_ABSTRACT_H

#include <flatbuffers/flatbuffers.h>

class SerialAbstract
{
public:
    virtual void Serialize(flatbuffers::FlatBufferBuilder &builder) = 0;
    virtual ~SerialAbstract() {}
};

#endif // BASE_SERIALIZER_H
