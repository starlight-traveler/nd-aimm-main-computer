#include "fb_check_t.h"
#include "aimm_test_manager.h"
#include "monster.h"

using namespace MyGame::Sample;

namespace testing_flatbuffer
{

    bool flatBufferGeneralTest(quill::Logger *logger)
    {
        flatbuffers::FlatBufferBuilder builder = flatBufferTestBuilder(logger);

        // Deserialize the buffer
        std::unique_ptr<DeserializedObject> deserialized_object = SerializerFactory::Deserialize(ObjectType::Monster, builder.GetBufferPointer());

        if (!deserialized_object)
        {
            LOG_TRACE_L1(logger, "Failed to deserialize Monster object.");
            TestManager::getInstance()->updateTest("flatbuffers", "Flatbuffer General Test", 0, "Failed to deserialize Monster object", ImVec4(1.0, 0.0, 0.0, 1.0), false);
            return false;
        }

        // Attempt to cast the deserialized object to Internal::Monster
        Internal::Monster *monster = dynamic_cast<Internal::Monster *>(deserialized_object.get());

        if (!monster)
        {
            LOG_TRACE_L1(logger, "Failed to cast deserialized object to Internal::Monster.");
            TestManager::getInstance()->updateTest("flatbuffers", "Flatbuffer General Test", 0, "Failed to cast deserialized object", ImVec4(1.0, 0.0, 0.0, 1.0), false);
            return false;
        }

        try
        {
            flatBufferDeserializedCheck(monster, logger);
            LOG_TRACE_L1(logger, "Monster data verification succeeded.");
            TestManager::getInstance()->updateTest("flatbuffers", "Flatbuffer General Test", 0, "Flatbuffer verified.", ImVec4(0.0, 1.0, 0.0, 1.0), false);
            return true;
        }
        catch (const std::exception &e)
        {
            LOG_TRACE_L1(logger, "Exception during monster data verification: {}", e.what());
            TestManager::getInstance()->updateTest("flatbuffers", "Flatbuffer General Test", 0, e.what(), ImVec4(1.0, 0.0, 0.0, 1.0), false);
            return false;
        }
    }

    flatbuffers::FlatBufferBuilder flatBufferTestBuilder(quill::Logger *logger)
    {
        flatbuffers::FlatBufferBuilder builder;

        std::unique_ptr<SerialAbstract> serializer = SerializerFactory::GetSerializer(ObjectType::Monster);
        if (serializer)
        {
            serializer->Serialize(builder);
            LOG_TRACE_L1(logger, "Successfully serialized monster object.");
            // No need to delete; unique_ptr handles it
        }
        else
        {
            LOG_TRACE_L1(logger, "Failed to create serializer.");
        }

        return builder;
    }

    void flatBufferExampleCheck(const MyGame::Sample::Monster *monster, quill::Logger *logger)
    {
        assert(monster->hp() == 80);
        assert(monster->mana() == 150); // default
        assert(monster->name()->str() == "MyMonster");

        auto pos = monster->pos();
        assert(pos && pos->z() == 3.0f);

        auto inv = monster->inventory();
        assert(inv && inv->Get(9) == 9);

        std::string expected_weapon_names[] = {"Sword", "Axe"};
        short expected_weapon_damages[] = {3, 5};
        auto weps = monster->weapons();
        assert(weps);
        for (unsigned int i = 0; i < weps->size(); i++)
        {
            assert(weps->Get(i)->name()->str() == expected_weapon_names[i]);
            assert(weps->Get(i)->damage() == expected_weapon_damages[i]);
        }

        assert(monster->equipped_type() == Equipment_Weapon);
        auto equipped = static_cast<const Weapon *>(monster->equipped());
        assert(equipped && equipped->name()->str() == "Axe" && equipped->damage() == 5);

        LOG_TRACE_L1(logger, "The FlatBuffer was successfully created and verified!\n");
    }

    void flatBufferDeserializedCheck(const Internal::Monster *monster, quill::Logger *logger)
    {
        assert(monster->hp == 80);
        assert(monster->mana == 150); // default
        assert(monster->name == "MyMonster");

        assert(monster->pos.z == 3.0f);

        assert(monster->inventory.size() == 10);
        assert(monster->inventory[9] == 9);

        std::string expected_weapon_names[] = {"Sword", "Axe"};
        short expected_weapon_damages[] = {3, 5};

        assert(monster->weapons.size() == 2);
        for (unsigned int i = 0; i < monster->weapons.size(); i++)
        {
            assert(monster->weapons[i].name == expected_weapon_names[i]);
            assert(monster->weapons[i].damage == expected_weapon_damages[i]);
        }

        assert(monster->equipped_weapon.name == "Axe");
        assert(monster->equipped_weapon.damage == 5);

        LOG_TRACE_L1(logger, "The deserialized Monster was successfully verified!");
    }

} // namespace testing_flatbuffer
