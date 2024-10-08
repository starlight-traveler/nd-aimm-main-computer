// serializer_factory.h
#ifndef SERIALIZER_FACTORY_H
#define SERIALIZER_FACTORY_H

#include "serial_abstract.h"
#include "deserializer_object.h"
#include <memory>

enum class ObjectType
{
    Monster,
    Motor,
    Other
    // Add other object types as needed
};

class SerializerFactory
{
public:
    static std::unique_ptr<SerialAbstract> GetSerializer(ObjectType type);
    static std::unique_ptr<DeserialAbstract> GetDeserializer(ObjectType type);

    // Method to deserialize and return the object
    static std::unique_ptr<DeserializedObject> Deserialize(ObjectType type, const uint8_t *buffer);
};

#endif // SERIALIZER_FACTORY_H
