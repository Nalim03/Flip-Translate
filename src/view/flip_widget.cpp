#include "flip_widget.h"

FlipWidget :: FlipWidget(QWidget *parent)
{
    initUI();
}

void FlipWidget :: initUI()
{
    textHLayout = new QHBoxLayout();
    configHLayout = new QHBoxLayout();
    vLayout = new QVBoxLayout();
    input = new QTextEdit();
    output = new QTextEdit();
    translate = new QPushButton();
    sourceLang = new QComboBox();
    outputLang = new QComboBox();
    fromLabel = new QLabel();
    toLabel = new QLabel();

    output->setReadOnly(true);
    translate->setText("Translate");
    fromLabel->setText("from:");
    toLabel->setText("to:");

    configHLayout->addWidget(translate);
    configHLayout->addWidget(fromLabel);
    configHLayout->addWidget(sourceLang);
    configHLayout->addWidget(toLabel);
    configHLayout->addWidget(outputLang);
    textHLayout->addWidget(input);
    textHLayout->addWidget(output);
    vLayout->addLayout(configHLayout);
    vLayout->addLayout(textHLayout);

    setLayout(vLayout);
    setWindowTitle("Flip Translate");
    resize(320, 240);

}
