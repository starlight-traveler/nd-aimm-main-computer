#include "aimm_test_detail.h"

// Setter for the result
void TestDetail::setResult(const std::string& res) {
    std::lock_guard<std::mutex> lock(mtx);
    result = res;
}

// Setter for the color
void TestDetail::setColor(const ImVec4& col) {
    std::lock_guard<std::mutex> lock(mtx);
    color = col;
}

// Setter for the running status
void TestDetail::setRunning(bool run) {
    std::lock_guard<std::mutex> lock(mtx);
    running = run;
}

// Getter for the description
std::string TestDetail::getDescription() const {
    std::lock_guard<std::mutex> lock(mtx);
    return description;
}

// Getter for the result
std::string TestDetail::getResult() const {
    std::lock_guard<std::mutex> lock(mtx);
    return result;
}

// Getter for the color
ImVec4 TestDetail::getColor() const {
    std::lock_guard<std::mutex> lock(mtx);
    return color;
}

// Getter for the running status
bool TestDetail::isRunning() const {
    std::lock_guard<std::mutex> lock(mtx);
    return running;
}

// Display function for ImGui rendering (example usage)
void TestDetail::display() const {
    std::lock_guard<std::mutex> lock(mtx);
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    ImGui::Button(result.c_str());
    if (running) {
        ImGui::SameLine();
        ImGui::Text("Running...");
    }
    ImGui::PopStyleColor();
}
