#include "aiwidget.h"

#include <QDebug>
#include <alphabeta.hpp>
#include <statistics.hpp>

#include "evaluate.hpp"
#include "ui_aiwidget.h"

AIWidget::AIWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::AIWidget), a(Evaluate()), b(Evaluate()) {
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

    std::vector<std::pair<Evaluate, Turn>> marks;

    /*
    Match match;
    match.SetBoard(board);
    Computer computer;
    computer.Start();
    computer.Wait();

    computer.LoadMarks(marks);

    std::sort(marks.begin(),marks.end());

    for(auto &pair : marks)
       ui->turns_list->addItem(QString::fromStdString(pair.second.ToChessFormat()+'
    '+pair.first.ToString()));
*/
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


void AIWidget::on_a_sliderMoved(int position)
{
     a = Evaluate(position);
     ui->a_text->setText(QString::number(position));
}


void AIWidget::on_b_sliderMoved(int position)
{
     b = Evaluate(position);
     ui->b_text->setText(QString::number(position));
}

