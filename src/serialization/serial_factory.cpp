// serializer_factory.cpp
#include "serializer_factory.h"
#include "example_serializer.h" // Ensure this includes MonsterSerializer
#include <iostream>

// Implement GetSerializer
std::unique_ptr<SerialAbstract> SerializerFactory::GetSerializer(ObjectType type)
{
    switch (type)
    {
    case ObjectType::Monster:
        return std::make_unique<MonsterSerializer>();
    // Handle other cases...
    default:
        return nullptr;
    }
}

// Implement GetDeserializer
std::unique_ptr<DeserialAbstract> SerializerFactory::GetDeserializer(ObjectType type)
{
    switch (type)
    {
    case ObjectType::Monster:
        return std::make_unique<MonsterSerializer>();
    // Handle other cases...
    default:
        return nullptr;
    }
}

// Implement Deserialize method
std::unique_ptr<DeserializedObject> SerializerFactory::Deserialize(ObjectType type, const uint8_t *buffer)
{
    std::unique_ptr<DeserialAbstract> deserializer = GetDeserializer(type);
    if (!deserializer)
    {
        std::cerr << "No deserializer available for the given ObjectType." << std::endl;
        return nullptr;
    }

    std::unique_ptr<DeserializedObject> obj = deserializer->Deserialize(buffer);
    // No need to delete deserializer; unique_ptr handles it
    return obj;
}
