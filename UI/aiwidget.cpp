#include "aiwidget.h"
#include "position_rating.hpp"
#include "ui_aiwidget.h"
#include <QDebug>
#include <statistics.hpp>
#include <alphabeta.hpp>

AIWidget::AIWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AIWidget)
{
    ui->setupUi(this);
}

AIWidget::~AIWidget()
{
    delete ui;
}

void AIWidget::on_set_clicked()
{
     current = home = BitBoard(ui->fen->text().toStdString());
     setBoard(home);
}

void AIWidget::setBoard(BitBoard board)
{
    current = board;
    ui->widget->PushBoard(board);
    ui->turns_list->clear();

    std::vector<std::pair<MainAppraiser,std::string>> marks;

    Match match;
    match.SetBoard(board);
    Computer computer(match,color_);
    computer.Start();
    computer.Wait();

    Statistics stat;
    TransPositionTable table(100);
    AlphaBeta<MainAppraiser> appraiser(color_,stat,table);

    for(Turn turn : board.GenerateTurns(board.CurrentColor())){
        auto subboard = board;
        subboard.ExecuteTurn(turn);
        marks.push_back({appraiser.GetValue(subboard,0),turn.ToChessFormat()});
    }
    ui->score->setText(QString::fromStdString(appraiser.GetBestTurn(board,5).ToChessFormat()));

    std::sort(marks.begin(),marks.end());

    for(auto &pair : marks)
       ui->turns_list->addItem(QString::fromStdString(pair.second+' '+pair.first.ToString(5)));
}


void AIWidget::on_turns_list_itemClicked(QListWidgetItem *item)
{
    auto turn = Turn::FromChessFormat(item->text().split(" ")[0].toStdString());
    BitBoard board(current);
    board.ExecuteTurn(turn);
    setBoard(board);
}


void AIWidget::on_white_radio_clicked()
{
     color_ = Color::kWhite;
     setBoard(current);
}


void AIWidget::on_black_radio_clicked()
{
     color_ = Color::kBlack;
     setBoard(current);
}

