#ifndef ASYNCIO_HPP
#define ASYNCIO_HPP

#include <functional>
#include <iostream>

#include <string>
#include <queue>
#include <optional>
#include <sstream>
#include <functional>

#include <mutex>
#include <thread>

struct Command{
    std::string name;
    std::vector<std::string> parameters;
};

class Asyncio
{
    Command ReadCommand(){

        Command command;

        std::string line;
        std::getline(std::cin,line);

        std::stringstream ss(line);
        ss >> command.name;

        while(ss.good()){
            command.parameters.push_back({});
            ss >> command.parameters.back();
        }

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
    Asyncio():
        exit_flag_{0},
        cin_thread_(std::bind(&Asyncio::CinThread,this))
    {
    };
    ~Asyncio(){
        exit(0);
    };

    std::optional<Command> GetCommandAsync(){
        std::lock_guard guard(mutex_);
        if(commands_.empty())
            return {};
        auto front = commands_.front();
        commands_.pop();
        return front;
    };

private:
    std::atomic<bool> exit_flag_;
    std::mutex mutex_;
    std::queue<Command> commands_;
    std::thread cin_thread_;

};

#endif
