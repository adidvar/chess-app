#include <asyncio.hpp>
#include <iostream>
#include <sstream>
#include <turn.hpp>
#include <vector>

#include "computer.hpp"
#include "fen.hpp"
#include "match.hpp"

void uciok() { std::cout << "uciok" << std::endl; };
// to controller
void id() {
  std::cout << "id name Chess5" << std::endl;
  std::cout << "id author adidvar@gmail.com" << std::endl;
  uciok();
};

void readyok() { std::cout << "readyok" << std::endl; };

void bestmove(Turn turn) {
  std::cout << "bestmove " << turn.ToChessFormat() << std::endl;
};

/*
void copyprotection(bool ok){
    std::cout << "copyprotection " <<
             (ok ? (const char*)"ok" : (const char*)"error")
          << std::endl;
};
void registration(bool ok){
    std::cout << "registration " <<
             (ok ? (const char*)"ok" : (const char*)"error")
          << std::endl;
};
void info(){

};
void option(){
};
*/

struct UCIState {
  Computer computer;

  UCIState() {}

  bool working = false;

  // from controller
  void uci() { id(); }
  void stop() { ; }
  void quit() { exit(0); }
  void isready() { readyok(); }
  void ponderhit() { ; }
  void ucinewgame() {}

  void debug(Command command) { ; }
  void setoption(Command command) { ; }
  void registered(Command command) { ; }

  void position(Command command) {
    if (!working) {
      Match match;
      match.LoadFromUCIString(command.parameters_line);
      computer.SetBoard(match.GetBoard());
    }
  }
  void go(Command command) {
    if (!working) {
      computer.Start();
      working = true;
    }
  }
  void exec() {
    if (working && computer.IsReady()) {
      bestmove(computer.Get());
      computer.Abort();
      working = false;
    }
    std::cout.flush();
  }
};

int main(){
    Asyncio io;
    UCIState state;

    for (;;) {
      auto command_opt = io.GetCommandAsync();
      if (command_opt.has_value()) {
        auto command = command_opt.value();

        if (command.name == "uci")
          state.uci();
        else if (command.name == "debug")
          state.debug(command);
        else if (command.name == "isready")
          state.isready();
        else if (command.name == "setoption")
          state.setoption(command);
        else if (command.name == "registered")
          state.registered(command);
        else if (command.name == "ucinewgame")
          state.ucinewgame();
        else if (command.name == "position")
          state.position(command);
        else if (command.name == "go")
          state.go(command);
        else if (command.name == "stop")
          state.stop();
        else if (command.name == "ponderhit")
          state.ponderhit();
        else if (command.name == "quit")
          state.quit();
      }
      state.exec();

      std::this_thread::sleep_for(std::chrono::milliseconds{20});
    }
}
