#include "aiwidget.h"
#include "position_rating.hpp"
#include "ui_aiwidget.h"
#include <QDebug>
#include <statistics.hpp>
#include <alphabeta.hpp>

AIWidget::AIWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AIWidget),
    a(MainAppraiser::Invalid()),
    b(MainAppraiser::Invalid())
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

    std::vector<std::pair<MainAppraiser,Turn>> marks;

    Match match;
    match.SetBoard(board);
    Computer computer(match,color_);
    computer.Start();
    computer.Wait();

    computer.LoadMarks(marks);

    std::sort(marks.begin(),marks.end());

    for(auto &pair : marks)
       ui->turns_list->addItem(QString::fromStdString(pair.second.ToChessFormat()+' '+pair.first.ToString()));
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
    a = MainAppraiser(position);
    ui->a_text->setText(QString::number(position));
}


void AIWidget::on_b_sliderMoved(int position)
{

    b = MainAppraiser(position);
    ui->b_text->setText(QString::number(position));
}

