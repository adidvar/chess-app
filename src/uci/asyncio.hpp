#ifndef ASYNCIO_HPP
#define ASYNCIO_HPP

#include <atomic>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>
#include <sstream>
#include <string>
#include <thread>

#include "fen.hpp"

struct Command{
    std::string name;
    std::vector<std::string> parameters;
    std::string parameters_line;
};

class Asyncio
{
    Command ReadCommand() {
      const static std::vector<char> del = {' ', '\t'};

      Command command;

      std::string line;
      std::getline(std::cin, line);

      size_t index = 0;
      command.name = readUntillDelims(line, del, index);

      command.parameters_line = line.substr(index);

      while (index != line.size()) {
        command.parameters.push_back({});
        command.parameters.back() = readUntillDelims(line, del, index);
      }

      log_ << line;
      log_.flush();

      return command;
    }

    void CinThread(){
        while(!exit_flag_)
        {
            auto command = ReadCommand();
            if(exit_flag_)
                break;
            std::lock_guard guard(mutex_);
            commands_.push(command);
        }
    }

   public:
    Asyncio()
        : exit_flag_{0}, cin_thread_(std::bind(&Asyncio::CinThread, this)) {
        log_.open("log.txt", std::ios_base::app);
    }
    ~Asyncio() { exit(0); }

    std::optional<Command> GetCommandAsync(){
        std::lock_guard guard(mutex_);
        if(commands_.empty())
            return {};
        auto front = commands_.front();
        commands_.pop();
        return front;
    }

private:
    std::atomic<bool> exit_flag_;
    std::mutex mutex_;
    std::queue<Command> commands_;
    std::thread cin_thread_;
    std::ofstream log_;
};

#endif
