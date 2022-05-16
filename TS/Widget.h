#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <vector>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include "modular.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    int xreg=200;
    int yreg=80;
    int treg=20;
    int regnn=30;

    int xinst=400;
    int yinst=80;

    int fromt=40;
    int tlineres=20;
    int tlineins=30;

    int xrest=800;
    int yrest=80;

    bool stop=false;
    vector<QLabel*> rigsname;
    vector<QLabel*> rigsnum;
    vector<QLabel*> ins;
    vector<QLabel*> resname;
    vector<QLabel*> rescon;
    vector<QTextEdit*> mnums;
    vector<QTextEdit*> mtime;
    QLabel clock;
    simulator sim;
    Widget(QWidget *parent = 0);
    ~Widget();
    void onClick();
    void load();
    void refreshnum();
    void run();
    void clear();
    void stopr();
    void save();
    void repar();
};

#endif // WIDGET_H
