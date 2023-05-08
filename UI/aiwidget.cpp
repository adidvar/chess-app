#include "aiwidget.h"
#include "ui_aiwidget.h"
#include <QDebug>

AIWidget::AIWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AIWidget)
{
    ui->setupUi(this);
    setBoard({});
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
     auto turns = board.GenerateTurns();
     std::vector<std::pair<int,std::string>> marks;
     for(auto turn : turns)
     {
         BitBoard copy(board);
         copy.ExecuteTurn(turn);
         marks.push_back({computer.Evaluate(copy,Color::kWhite).Value(),turn.ToChessFormat()});
     }
     auto current = computer.Evaluate(board,Color::kWhite);
     qDebug() << current.Value();
     std::sort(marks.begin(),marks.end());
     for(auto &pair : marks)
     {
        ui->turns_list->addItem(QString::fromStdString(pair.second+' '+std::to_string(pair.first)));
     }
     ui->status->setValue(current.Value());
     ui->score->setText(QString::number(current.Value()));
}


void AIWidget::on_turns_list_itemClicked(QListWidgetItem *item)
{
    auto turn = Turn::FromChessFormat(item->text().split(" ")[0].toStdString());
    BitBoard board(current);
    board.ExecuteTurn(turn);
    setBoard(board);
}

