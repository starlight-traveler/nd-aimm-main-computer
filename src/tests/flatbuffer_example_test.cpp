#include "flatbuffers/flatbuffers.h"
#include "logger.h"

#include "serializer_factory.h"
#include "example_generated.h"

void flatBufferExampleCheck() {


    // Get and test some scalar types from the FlatBuffer.
    assert(monster->hp() == 80);
    assert(monster->mana() == 150); // default
    assert(monster->name()->str() == "MyMonster");

    // Get and test a field of the FlatBuffer's `struct`.
    auto pos = monster->pos();
    assert(pos);
    assert(pos->z() == 3.0f);
    (void)pos;

    // Get a test an element from the `inventory` FlatBuffer's `vector`.
    auto inv = monster->inventory();
    assert(inv);
    assert(inv->Get(9) == 9);
    (void)inv;

    // Get and test the `weapons` FlatBuffers's `vector`.
    std::string expected_weapon_names[] = {"Sword", "Axe"};
    short expected_weapon_damages[] = {3, 5};
    auto weps = monster->weapons();
    for (unsigned int i = 0; i < weps->size(); i++)
    {
        assert(weps->Get(i)->name()->str() == expected_weapon_names[i]);
        assert(weps->Get(i)->damage() == expected_weapon_damages[i]);
    }
    (void)expected_weapon_names;
    (void)expected_weapon_damages;

    // Get and test the `Equipment` union (`equipped` field).
    assert(monster->equipped_type() == Equipment_Weapon);
    auto equipped = static_cast<const Weapon *>(monster->equipped());
    assert(equipped->name()->str() == "Axe");
    assert(equipped->damage() == 5);
    (void)equipped;

    printf("The FlatBuffer was successfully created and verified!\n");
}