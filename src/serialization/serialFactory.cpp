#include "serializer_factory.h"
#include "example_serializer.h"

SerialAbstract *SerializerFactory::GetSerializer(ObjectType type)
{
    switch (type)
    {
    case ObjectType::Monster:
        return new MonsterSerializer();
    case ObjectType::Motor:
    // Other cases
    default:
        return nullptr;
    }
}
