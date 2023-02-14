#ifndef MATCHVISUAL_H
#define MATCHVISUAL_H

#include <QObject>
#include <QWidget>
#include <QVector>

class MatchVisual : public QWidget
{
    Q_OBJECT
    QVector<float> data;
public:
    explicit MatchVisual(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;

    void upload(QVector<float> data);
signals:

};

#endif // MATCHVISUAL_H
