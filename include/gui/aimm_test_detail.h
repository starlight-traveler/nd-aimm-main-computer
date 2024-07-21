#ifndef TEST_DETAIL_H
#define TEST_DETAIL_H

#include <string>
#include <mutex>
#include <imgui.h>

// Thread-safe Test Detail class
class TestDetail
{
private:
    std::string description;
    std::string result;
    ImVec4 color;
    bool running;
    mutable std::mutex mtx;

public:
    TestDetail(std::string desc, std::string res, ImVec4 col)
        : description(std::move(desc)), result(std::move(res)), color(col), running(false) {}

    void setResult(const std::string &res);
    void setColor(const ImVec4 &col);
    void setRunning(bool run);
    std::string getDescription() const;
    std::string getResult() const;
    ImVec4 getColor() const;
    bool isRunning() const;
    void display() const;
};

#endif // TEST_DETAIL_H
