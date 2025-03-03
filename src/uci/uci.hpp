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
    int remainingTime = (settings.board.side() == Color::White) ? wsec : bsec;
    int opponentTime = (settings.board.side() == Color::White) ? bsec : wsec;
    int increment = (settings.board.side() == Color::White) ? winc : binc;

    int timeDifference = remainingTime - opponentTime;

    int desiredTime = 4000 + (timeDifference / 3) + increment;

    // Встановлюємо час на хід з урахуванням додаткового часу і ліміту
    settings.time = std::min(remainingTime / 20 + increment,
                             std::max(desiredTime, remainingTime / 2));

    sendInfo("time control: auto");
    sendInfo(std::format(
        "time: {}",
        settings.time / 1000));  // Перетворюємо на секунди для виведення
  };
  virtual void goTime(int msec) override {
    settings.time = msec;
    sendInfo("time controll: time");
    sendInfo(std::format("time: {}", settings.time));
  };
  virtual void goDepth(int depth) override {
    settings.depth = depth;
    sendInfo("depth controll: enabled");
  };
  virtual void goNodes(long nodes) override {
    settings.nodes = nodes;
    sendInfo("nodes controll: enabled");
  };
  virtual void goMoves(std::vector<Turn> turns) override {
    settings.moves = turns;
    sendInfo("nodes moves: enabled");
  };
  virtual void goInfinite() override {
    settings.resetLimits();
    sendInfo("time controll: infinite");
  };
  virtual void goMovesToGo(int moves) override {};

 private:
  Computer computer;
  SearchSettings settings;
};
