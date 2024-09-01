#ifndef LOG_FILE_MONITOR_H
#define LOG_FILE_MONITOR_H

#include "ring_buffer.h"
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

class LogFileMonitor
{
private:
    std::ifstream file;
    RingBuffer logBuffer;
    std::streampos lastPos;
    static LogFileMonitor *instance;
    static std::mutex mutex;

    // Private constructor
    LogFileMonitor(const std::string &filename) : logBuffer(100)
    {
        // std::cout << "Attempting to open file: " << filename << std::endl;
        file.open(filename, std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            // std::cerr << "Failed to open log file: " << filename << std::endl;
            throw std::runtime_error("Failed to open log file: " + filename);
        }
        lastPos = file.tellg();
        // std::cout << "File opened successfully. Initial position: " << lastPos << std::endl;
    }

public:
    // Delete copy constructor and assignment operator
    LogFileMonitor(const LogFileMonitor &) = delete;
    LogFileMonitor &operator=(const LogFileMonitor &) = delete;

    // Singleton access
    static LogFileMonitor *GetInstance(const std::string &filename)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (instance == nullptr)
        {
            // std::cout << "Creating new instance of LogFileMonitor." << std::endl;
            instance = new LogFileMonitor(filename);
        }
        else
        {
            // std::cout << "Instance already exists." << std::endl;
        }
        return instance;
    }

    void UpdateLogBuffer()
    {
        // std::cout << "Updating log buffer from file..." << std::endl;
        file.clear();                 // Clear any EOF or error flags
        file.seekg(0, std::ios::end); // Move to end of file to get current size
        std::streampos fileSize = file.tellg();
        if (lastPos != fileSize)
        {
            file.seekg(lastPos, std::ios::beg); // Seek back to the last known good position

            if (!file)
            {
                // std::cerr << "Error: Unable to seek in file. Last known position: " << lastPos << std::endl;
                return;
            }

            std::string line;
            int lineCount = 0;
            while (std::getline(file, line))
            {
                logBuffer.push(line);
                lineCount++;
            }

            if (lineCount > 0)
            {
                // std::cout << lineCount << " new lines read and added to the buffer." << std::endl;
            }
            else
            {
                // std::cout << "No new lines were read. File may not have been updated." << std::endl;
            }

            lastPos = file.tellg(); // Update last position after reading new lines
            if (lastPos == std::streampos(-1))
            {
                // std::cerr << "Failed to get a valid position after read. Resetting last position." << std::endl;
                file.clear();       // Clear any error states
                lastPos = fileSize; // Reset position to the current end if there's an issue
            }
            else
            {
                // std::cout << "Updated last position to: " << lastPos << std::endl;
            }
        }
        else
        {
            // std::cout << "No update in file size; no need to read new lines." << std::endl;
        }
    }

    // Method to get the next log entry
    std::string GetNextLogEntry()
    {
        if (!logBuffer.isEmpty())
        {
            std::string entry = logBuffer.pop();
            // std::cout << "Popping log entry from buffer: " << entry << std::endl;
            return entry;
        }
        return ""; // Return an empty string if the buffer is empty
    }

    // Destructor
    ~LogFileMonitor()
    {
        if (file.is_open())
        {
            // std::cout << "Closing file..." << std::endl;
            file.close();
        }
    }
};

// Static members initialization
LogFileMonitor *LogFileMonitor::instance = nullptr;
std::mutex LogFileMonitor::mutex;

#endif // LOG_FILE_MONITOR_H
