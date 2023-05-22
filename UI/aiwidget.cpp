#include "aiwidget.h"
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
    NoStatistics stat;
    current = board;
     ui->widget->PushBoard(board);
     ui->turns_list->clear();
     auto turns = board.GenerateTurns();
     std::vector<std::pair<MateAppraiser,std::string>> marks;
     for(auto turn : turns)
     {
         BitBoard copy(board);
         copy.ExecuteTurn(turn);
         marks.push_back({AlphaBeta<MateAppraiser,NoStatistics>::Evaluate(copy,color_,6,stat),turn.ToChessFormat()});
     }
     auto current = AlphaBeta<MateAppraiser,NoStatistics>::Evaluate(board,color_,7,stat);
     qDebug() << QString::fromStdString(current.ToString());
     std::sort(marks.begin(),marks.end());
     for(auto &pair : marks)
     {
        ui->turns_list->addItem(QString::fromStdString(pair.second+' '+pair.first.ToString()));
     }
     ui->score->setText(QString::fromStdString(current.ToString()));
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

