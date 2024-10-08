// example_serializer.cpp
#include "example_serializer.h"
#include "example_generated.h" // FlatBuffers generated code
#include <iostream>

void MonsterSerializer::Serialize(flatbuffers::FlatBufferBuilder &builder)
{
    // Example data for weapons
    auto weapon_one_name = builder.CreateString("Sword");
    short weapon_one_damage = 3;
    auto weapon_two_name = builder.CreateString("Axe");
    short weapon_two_damage = 5;

    // Create Weapons using FlatBuffers methods
    auto sword = MyGame::Sample::CreateWeapon(builder, weapon_one_name, weapon_one_damage);
    auto axe = MyGame::Sample::CreateWeapon(builder, weapon_two_name, weapon_two_damage);

    // Create a vector of weapons
    std::vector<flatbuffers::Offset<MyGame::Sample::Weapon>> weapons_vector = {sword, axe};
    auto weapons = builder.CreateVector(weapons_vector);

    // Example data for Monster
    MyGame::Sample::Vec3 position = MyGame::Sample::Vec3(1.0f, 9.0f, 3.0f); // Use FlatBuffers Vec3
    auto name = builder.CreateString("MyMonster");

    // Assume monster also has an inventory as a vector of unsigned chars
    unsigned char inventory_data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto inventory = builder.CreateVector(inventory_data, 10);

    // Create the Monster object
    auto monster = MyGame::Sample::CreateMonster(
        builder,
        &position,                        // Passed by pointer if required
        150,                              // mana
        80,                               // hp
        name,                             // name
        inventory,                        // inventory
        MyGame::Sample::Color_Red,        // color
        weapons,                          // weapons
        MyGame::Sample::Equipment_Weapon, // equipped_type
        axe.Union()                       // equipped
    );

    // Finish the buffer at the Monster object
    builder.Finish(monster);

    // Normally, you would handle the serialized data here (e.g., store or send over network)
}

std::unique_ptr<DeserializedObject> MonsterSerializer::Deserialize(const uint8_t *buffer)
{
    // Parse the buffer to get the FlatBuffer Monster object
    auto fb_monster = MyGame::Sample::GetMonster(buffer);
    if (!fb_monster)
    {
        std::cerr << "Failed to parse Monster from buffer." << std::endl;
        return nullptr;
    }

    // Create a new Monster instance from the Internal namespace
    auto monster = std::make_unique<Internal::Monster>();

    // Populate Monster fields from FlatBuffer
    monster->hp = fb_monster->hp();
    monster->mana = fb_monster->mana();
    monster->name = fb_monster->name() ? fb_monster->name()->str() : "";

    auto fb_pos = fb_monster->pos();
    if (fb_pos)
    {
        monster->pos.x = fb_pos->x();
        monster->pos.y = fb_pos->y();
        monster->pos.z = fb_pos->z();
    }

    // Copy inventory
    auto fb_inventory = fb_monster->inventory();
    if (fb_inventory)
    {
        monster->inventory.assign(fb_inventory->begin(), fb_inventory->end());
    }

    // Copy weapons
    auto fb_weapons = fb_monster->weapons();
    if (fb_weapons)
    {
        for (auto fb_weapon : *fb_weapons)
        {
            Internal::Weapon weapon;
            weapon.name = fb_weapon->name() ? fb_weapon->name()->str() : "";
            weapon.damage = fb_weapon->damage();
            monster->weapons.push_back(weapon);
        }
    }

    // Copy equipped weapon
    if (fb_monster->equipped_type() == MyGame::Sample::Equipment_Weapon)
    {
        auto fb_weapon = static_cast<const MyGame::Sample::Weapon *>(fb_monster->equipped());
        if (fb_weapon)
        {
            monster->equipped_weapon.name = fb_weapon->name() ? fb_weapon->name()->str() : "";
            monster->equipped_weapon.damage = fb_weapon->damage();
        }
    }

    // You can populate other fields as necessary...

    return monster; // Correctly returning std::unique_ptr<DeserializedObject>
}
