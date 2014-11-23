#ifndef BXACTORITEM_H
#define BXACTORITEM_H

#include "BxAttributes.h"

#include <QGraphicsItem>
#include <QtWidgets>
#include <QVector>

class BxNodeActor : public QGraphicsEllipseItem
{
public:
    BxNodeActor(QGraphicsItem *parent = 0);
    BxNodeActor(const QJsonObject &, QGraphicsItem *parent = 0);
    ~BxNodeActor();


    void initialise();
    QWidget* getControls();
    void writeToJson(QJsonObject &);
    void readFromJson(const QJsonObject &);
    void addAttribute(BxBaseAttribute*);

signals:
    
public slots:
//    void setValue(int in) { testValue = in; }

private:
//    BxIntAttribute* mTestValue;
    QVector<BxBaseAttribute*> mAttributes;
    QString mID;
    
};

#endif // BXACTORITEM_H