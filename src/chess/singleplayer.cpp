#include "singleplayer.h"

#include <iostream>

#include "ui_singleplayer.h"

SinglePlayer::SinglePlayer(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SinglePlayer) {
  ui->setupUi(this);
  ui->board->SetMode(BoardWidget::kPlayerWhite);
  connect(ui->board, &BoardWidget::EnteredTurn, this,
          &SinglePlayer::TurnEntered);
}

SinglePlayer::~SinglePlayer() { delete ui; }

void SinglePlayer::ComputerEntered() {
  computer_->Stop();

  auto turn = computer_->GetTurn();
  std::cout << turn.ToChessFormat() << std::endl;

  delete computer_;

  if (!turn.Valid()) return;

  match.Push(turn);
  ui->board->PushTurn(turn);
}

void SinglePlayer::TurnEntered(Turn inputturn) {
  if (match.GetBoard().CurrentColor() != Color::kWhite) return;

  match.Push(inputturn);

  table_.Clear();
  computer_ = new ThreadController(Color::kWhite, match.GetBoard(), &table_);

  computer_->Start();

  QTimer::singleShot(6000, this, &SinglePlayer::ComputerEntered);
}
