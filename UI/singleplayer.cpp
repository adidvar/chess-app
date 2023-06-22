#include "singleplayer.h"
#include "ui_singleplayer.h"

SinglePlayer::SinglePlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SinglePlayer),
    computer_(match,Color::kBlack)
{
    ui->setupUi(this);
    ui->board->SetMode(BoardWidget::kPlayerWhite);
    connect(ui->board,&BoardWidget::EnteredTurn,this,&SinglePlayer::TurnEntered);
}

SinglePlayer::~SinglePlayer()
{
    delete ui;
}

void SinglePlayer::TurnEntered(Turn inputturn)
{
    match.Push(inputturn);

    computer_.Start();
    computer_.Wait();
    std::vector<std::pair<Turn,MainAppraiser>> marks;
    computer_.LoadTurnsMarks(marks);

    auto max = std::max_element(marks.begin(),marks.end(),[](auto m1, auto m2){return m1.second < m2.second;});
    auto turn = max->first;

    match.Push(turn);
    ui->board->PushTurn(turn);
}
