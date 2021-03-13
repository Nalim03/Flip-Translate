#ifndef FLIP_MODEL_H
#define FLIP_MODEL_H

#include <QObject>
#include <QString>

class FlipModel : public QObject {
    Q_OBJECT
public:
    void translate(QString sentece);

signals:
    void translated(QString translatedSentence);
};

#endif //FLIP_MODEL_H
