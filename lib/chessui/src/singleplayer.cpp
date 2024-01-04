#include "singleplayer.h"

#include <iostream>

#include "ui_singleplayer.h"

SinglePlayer::SinglePlayer(QWidget *parent)
    : QMainWindow(parent), computer_(Color::kBlack), ui(new Ui::SinglePlayer) {
  ui->setupUi(this);
  ui->board->SetMode(BoardWidget::kPlayerWhite);
  connect(ui->board, &BoardWidget::EnteredTurn, this,
          &SinglePlayer::TurnEntered);
  // computer_.Start();
}

SinglePlayer::~SinglePlayer() { delete ui; }

void SinglePlayer::ComputerEntered() {
  /*
  computer_.Stop();

  auto turn = computer_.GetTurn();
  auto pv = computer_.GetDepth();
  std::cout << pv << std::endl;

  if (!turn.Valid()) return;

  match.Push(turn);
  ui->board->PushTurn(turn);

*/
  // computer_.Start();
}

void SinglePlayer::TurnEntered(Turn inputturn) {
  if (match.GetBoard().CurrentColor() != Color::kWhite) return;

  match.Push(inputturn);
  computer_.SetBoard(match.GetBoard());

  // auto turn = computer_.Work();

  // match.Push(turn);
  // ui->board->PushTurn(turn);

  // computer_.Stop();

  // QTimer::singleShot(6000, this, &SinglePlayer::ComputerEntered);
}
