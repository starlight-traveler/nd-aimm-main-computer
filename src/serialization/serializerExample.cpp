#include <flatbuffers/flatbuffers.h>


#include "example_serializer.h"
#include "example_generated.h" // This should be the generated header file from FlatBuffers schema

using namespace MyGame::Sample;

void MonsterSerializer::Serialize(flatbuffers::FlatBufferBuilder &builder)
{
    // Example data for weapons
    auto weapon_one_name = builder.CreateString("Sword");
    short weapon_one_damage = 3;
    auto weapon_two_name = builder.CreateString("Axe");
    short weapon_two_damage = 5;

    // Create Weapons using the shortcut method assumed to be defined in your schema
    auto sword = CreateWeapon(builder, weapon_one_name, weapon_one_damage);
    auto axe = CreateWeapon(builder, weapon_two_name, weapon_two_damage);

    // Create a vector of weapons
    std::vector<flatbuffers::Offset<Weapon>> weapons_vector = {sword, axe};
    auto weapons = builder.CreateVector(weapons_vector);

    // Example data for Monster
    Vec3 position = Vec3(1.0f, 9.0f, 3.0f); // Assuming Vec3 is a struct from the schema
    auto name = builder.CreateString("MyMonster");

    // Assume monster also has an inventory as a vector of unsigned chars
    unsigned char inventory_data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto inventory = builder.CreateVector(inventory_data, 10);

    // Create the Monster object
    auto monster = CreateMonster(builder, &position, 150, 80, name, inventory, Color_Red, weapons, Equipment_Weapon, axe.Union());

    // Finish the buffer at the Monster object
    builder.Finish(monster);

    // Normally, you would here handle the serialized data, like storing or sending over network
}
