#ifndef FLIPWIDGET_H
#define FLIPWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

class FlipWidget : public QWidget
{
    Q_OBJECT

public:
    FlipWidget(QWidget *parent = 0);

private:
    QHBoxLayout *configHLayout;
    QHBoxLayout *textHLayout;
    QVBoxLayout *vLayout;
    QTextEdit *input;
    QTextEdit *output;
    QPushButton *translate;
    QComboBox *sourceLang;
    QComboBox *outputLang;
    QLabel *fromLabel;
    QLabel *toLabel;


    void initUI();
};


#endif // FLIPWIDGET_H
