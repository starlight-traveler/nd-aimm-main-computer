#ifndef TEST_MANAGER_H
#define TEST_MANAGER_H

#include "aimm_test_detail.h"
#include "initializer.h"
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <iostream>
#include <imgui.h>
#include "runner.h"

// Singleton class to manage TestDetail instances categorized by type
class TestManager
{
private:
    static TestManager *instance;
    std::map<std::string, std::vector<std::shared_ptr<TestDetail>>> categorizedTests;
    mutable std::mutex tests_mutex;

    // Private constructor
    TestManager() {}

public:
    static TestManager *getInstance()
    {
        static TestManager instance; // Guaranteed to be destroyed and instantiated on first use.
        return &instance;
    }

    void addTest(const std::string &category, const std::string &desc, const std::string &res, ImVec4 col)
    {
        std::lock_guard<std::mutex> lock(tests_mutex);
        categorizedTests[category].push_back(std::make_shared<TestDetail>(desc, res, col));
    }

    void updateTest(const std::string &category, const std::string &testString, int index, const std::string &result, const ImVec4 &color, bool running)
    {

        if (color.x == 0.5f && color.y == 0.5f && color.z == 0.5f && color.w == 1.0f)
        {
        }
        else if (color.x == 1.0f && color.y == 0.8f && color.z == 0.0f && color.w == 1.0f)
        {
            StatusLogger::getInstance()->incrementWarnings(category, testString);

            ImGuiToast toast(ImGuiToastType::Warning, 2000); // <-- content can also be passed here as above
            toast.setTitle("Warning: %s", &category);
            toast.setContent("System, %s, has thrown a warning and is not functioning properly.", &testString);
            ImGui::InsertNotification(toast);
        }
        else if (color.x == 1.0f && color.y == 0.0f && color.z == 0.0f && color.w == 1.0f)
        {
            StatusLogger::getInstance()->incrementErrors(category, testString);
        }
        else if (color.x == 0.0f && color.y == 1.0f && color.z == 0.0f && color.w == 1.0f)
        {
            StatusLogger::getInstance()->incrementPasses(category, testString);
        }

        std::lock_guard<std::mutex> lock(tests_mutex);
        if (index < categorizedTests[category].size())
        {
            categorizedTests[category][index]->setResult(result);
            categorizedTests[category][index]->setColor(color);
            categorizedTests[category][index]->setRunning(running);
        }
    }

    const std::vector<std::shared_ptr<TestDetail>> &getTests(const std::string &category) const
    {
        auto it = categorizedTests.find(category);
        if (it != categorizedTests.end())
        {
            return it->second;
        }
        static std::vector<std::shared_ptr<TestDetail>> empty;
        return empty; // Return an empty vector if the category does not exist
    }
};

#endif // TEST_MANAGER_H
