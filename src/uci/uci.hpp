#include <string>
#include <vector>

#include "chessai/computer.hpp"
#include "parser.hpp"
#include "writter.hpp"

class UCI : public Parser {
 public:
  UCI() {
    feedback.reset(new UCIFeedBack());
    settings.feedback = std::dynamic_pointer_cast<IFeedBack>(feedback);
    settings.depth = 7;
  }

  virtual void debug(bool enable) override { feedback->setDebugMode(enable); }

  void uci() override {
    id();
    uciOk();
  }

  void stop() override {
    if (feedback->getReadyFlag() == false) computer.Abort();
  }

  void quit() override {
    exit(0);
  }

  void isReady() override {
    readyOk();
  }

  void ponderHit() override {
  }

  void uciNewGame() override { stop(); }

  void setOption(const std::string& name, const std::string& value) override {
  }

  void position(const BitBoard& fen, const std::vector<Turn>& moves) override {
    BitBoard board(fen);
    for (auto turn : moves) {
      if (!turn.isValid()) throw std::runtime_error("Invalid turn");
      if (!board.testTurn(turn)) throw std::runtime_error("Invalid turn");

      board = board.executeTurn(board.getCurrentSide(), turn);
    }
    settings.board = board;
  }

  void go(const std::string& parameters) override {
    if (feedback->getReadyFlag() == true) {
      computer.Start(settings);
      feedback->resetReadyFlag();
    }
  }

 private:
  Computer computer;
  SearchSettings settings;
  std::shared_ptr<UCIFeedBack> feedback;

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

  void bestMove(const std::string& move, const std::string& ponder = "") {
    Writter::bestMove(move, ponder);
  }
};
