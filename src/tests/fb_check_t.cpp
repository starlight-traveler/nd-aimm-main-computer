#include "fb_check_t.h"
#include "aimm_test_manager.h"

using namespace MyGame::Sample;

namespace testing_flatbuffer
{

    bool flatBufferGeneralTest(quill::Logger *logger)
    {
        flatbuffers::FlatBufferBuilder builder = flatBufferTestBuilder(logger);
        auto monster = GetMonster(builder.GetBufferPointer());

        if (!monster)
        {
            LOG_TRACE_L1(logger, "Failed to get monster from buffer.");
            TestManager::getInstance()->updateTest("flatbuffers", "Flatbuffer General Test", 0, "Failed to retrieve from buffer", ImVec4(1.0, 0.0, 0.0, 1.0), false);
            return false;
        }

        try
        {
            flatBufferExampleCheck(monster, logger);
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

        SerialAbstract *serializer = SerializerFactory::GetSerializer(ObjectType::Monster);
        if (serializer)
        {
            serializer->Serialize(builder);
            LOG_TRACE_L1(logger, "Successfully serialized monster object.");
            delete serializer; // Clean up to prevent memory leak
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

} // namespace testing_flatbuffer
