#ifndef SERIALIZER_FACTORY_H
#define SERIALIZER_FACTORY_H

#include "serial_abstract.h"

enum class ObjectType
{
    Monster,
    Motor,
    Other
}; // Define object types as needed

class SerializerFactory
{
public:
    static SerialAbstract *GetSerializer(ObjectType type);
};

#endif // SERIALIZER_FACTORY_H
