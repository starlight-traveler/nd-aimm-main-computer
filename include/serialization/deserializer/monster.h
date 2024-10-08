// monster.h
#ifndef MONSTER_H
#define MONSTER_H

#include "deserializer_object.h"
#include <string>
#include <vector>
#include <iostream>

namespace Internal
{

    struct Vec3
    {
        float x;
        float y;
        float z;
    };

    struct Weapon
    {
        std::string name;
        int16_t damage;
    };

    enum class Color : uint8_t
    {
        Red = 0,
        Green,
        Blue
    };

    class Monster : public DeserializedObject
    {
    public:
        Vec3 pos;
        int16_t mana;
        int16_t hp;
        std::string name;
        std::vector<uint8_t> inventory;
        Color color;
        std::vector<Weapon> weapons;
        Weapon equipped_weapon;

        void PrintInfo() const override
        {
            std::cout << "Monster's Name: " << name << "\nHP: " << hp
                      << "\nMana: " << mana << "\nPosition: (" << pos.x
                      << ", " << pos.y << ", " << pos.z << ")\n";

            std::cout << "Inventory: ";
            for (auto item : inventory)
            {
                std::cout << static_cast<int>(item) << " ";
            }
            std::cout << "\n";

            std::cout << "Weapons:\n";
            for (const auto &weapon : weapons)
            {
                std::cout << "  Name: " << weapon.name << ", Damage: " << weapon.damage << "\n";
            }

            std::cout << "Equipped Weapon: " << equipped_weapon.name
                      << ", Damage: " << equipped_weapon.damage << "\n";
        }
    };

} // namespace Internal

#endif // MONSTER_H
