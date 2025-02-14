#include <string>
#include <vector>

#include "chessai/computer.hpp"
#include "parser.hpp"
#include "tools.hpp"
#include "writter.hpp"

class UCI : public Parser {
 public:
  UCI() {
    settings.feedback.reset(dynamic_cast<IFeedBack *>(this), [](auto ptr) {});
  }

  void id() {
    Writter::idName("chess-app");
    Writter::idAuthor("adidvar");
  }

  void uciOk() {
    Writter::uciOk();
  }

  void readyOk() {
    Writter::readyOk();
  }

  virtual void debug(bool enable) override {
    setDebugMode(enable);
  }

  void uci() override {
    id();
    uciOk();
  }

  void stop() override {
    if (getReadyFlag() == false) computer.Abort();
  }

  void quit() override {
    exit(0);
  }

  void isReady() override {
    readyOk();
  }

  void uciNewGame() override {
    stop();
  }

  void setOption(const std::string &name, const std::string &value) override {}

  void position(const BitBoard &fen) override {
    settings.board = fen;
  }

  void go() override {
    if (getReadyFlag() == true) {
      computer.Start(settings);
      resetReadyFlag();
    }
  }

  void goPerft(int depth) override {
    perft(settings.board, depth);
  }

  virtual void goMate(int depth) override {
    sendInfo("mate search");
  }

  virtual void goTimeControl(int wsec, int bsec, int winc, int binc) override {
    sendInfo("time controll: auto");
  };
  virtual void goTime(int msec) override {
    sendInfo("time controll: time");
  };
  virtual void goDepth(int depth) override {
    sendInfo("depth controll: enabled");
  };
  virtual void goNodes(long nodes) override {
    sendInfo("nodes controll: enabled");
  };
  virtual void goMoves(std::vector<Turn> turns) override {
    sendInfo("nodes moves: enabled");
  };
  virtual void goInfinite() override {
    sendInfo("time controll: infinite");
  };
  virtual void goMovesToGo(int moves) override {};

 private:
  Computer computer;
  SearchSettings settings;
};
