// deserialized_object.h
#ifndef DESERIALIZED_OBJECT_H
#define DESERIALIZED_OBJECT_H

class DeserializedObject
{
public:
    virtual void PrintInfo() const = 0; // Pure virtual function
    virtual ~DeserializedObject() {}
};

#endif // DESERIALIZED_OBJECT_H
