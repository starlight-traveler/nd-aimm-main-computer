#ifndef MONSTER_SERIALIZER_H
#define MONSTER_SERIALIZER_H

#include "serial_abstract.h"
#include "monster_generated.h" // Include generated FlatBuffer schema header

class MonsterSerializer : public SerialAbstract
{
public:
    void Serialize(flatbuffers::FlatBufferBuilder &builder) override;
};

#endif // MONSTER_SERIALIZER_H
