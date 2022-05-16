#include "Widget.h"
#include "modular.h"
#include <QPushButton>
#include <QLabel>
#include <fstream>
#include <vector>
#include <QTime>
#include <QApplication>
#include <QDebug>
#include <QTextEdit>
#include <QFileDialog>

#include <iostream>
#include <sstream>
#include <cstring>
#include <fstream>
#include <windows.h>
void sleep(unsigned int ads)
{
    QTime reachtime = QTime::currentTime().addMSecs(ads);
    while(QTime::currentTime()<reachtime)
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
}

Widget::Widget(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("Tomasulo Simulator");
    this->setFixedSize(1900,1000);


    this->clock.setParent(this);
    this->clock.move(20,20);
    this->clock.setText("clock:0  ");
    this->clock.show();

    QPushButton* btn = new QPushButton;
    btn->setParent(this);
    btn->setText("步进");
    btn->move(200,900);
    QFont font("华文行楷",20,10,0);
    btn->setFont(font);
    btn->show();

    btn->setStyleSheet("QPushButton{background-color:white;}\
                        QPushButton:hover{background-color:rgba(160,160,160);}\
                        QPushButton:pressed{background-color:rgba(100,100,100);}");
    connect(btn,&QPushButton::clicked,this,&Widget::onClick);


    QPushButton* loadb = new QPushButton;
    loadb->setParent(this);
    loadb->setText("载入");
    loadb->move(400,900);
    loadb->setFont(font);
    loadb->show();

    loadb->setStyleSheet("QPushButton{background-color:white;}\
                                QPushButton:hover{background-color:rgba(160,160,160);}\
                                QPushButton:pressed{background-color:rgba(100,100,100);}");
    connect(loadb,&QPushButton::clicked,this,&Widget::load);


    QPushButton* run = new QPushButton;
    run->setParent(this);
    run->setText("运行");
    run->move(600,900);
    run->setFont(font);
    run->show();

    run->setStyleSheet("QPushButton{background-color:white;}\
                                        QPushButton:hover{background-color:rgba(160,160,160);}\
                                        QPushButton:pressed{background-color:rgba(100,100,100);}");
    connect(run,&QPushButton::clicked,this,&Widget::run);


    QPushButton* reset = new QPushButton;
    reset->setParent(this);
    reset->setText("清除");
    reset->move(800,900);
    reset->setFont(font);
    reset->show();

    reset->setStyleSheet("QPushButton{background-color:white;}\
                                                QPushButton:hover{background-color:rgba(160,160,160);}\
                                                QPushButton:pressed{background-color:rgba(100,100,100);}");
    connect(reset,&QPushButton::clicked,this,&Widget::clear);




    QPushButton* stop = new QPushButton;
    stop->setParent(this);
    stop->setText("暂停");
    stop->move(1000,900);
    stop->setFont(font);
    stop->show();

    stop->setStyleSheet("QPushButton{background-color:white;}\
                                                        QPushButton:hover{background-color:rgba(160,160,160);}\
                                                        QPushButton:pressed{background-color:rgba(100,100,100);}");
    connect(stop,&QPushButton::clicked,this,&Widget::stopr);


    QPushButton* save = new QPushButton;
    save->setParent(this);
    save->setText("保存");
    save->move(1200,900);
    save->setFont(font);
    save->show();

    save->setStyleSheet("QPushButton{background-color:white;}\
                                                                QPushButton:hover{background-color:rgba(160,160,160);}\
                                                                QPushButton:pressed{background-color:rgba(100,100,100);}");
    connect(save,&QPushButton::clicked,this,&Widget::save);


    QPushButton* repa = new QPushButton;
    repa->setParent(this);
    repa->setText("修改参数");
    repa->move(1400,900);
    repa->setFont(font);
    repa->show();

    repa->setStyleSheet("QPushButton{background-color:white;}\
                                                                        QPushButton:hover{background-color:rgba(160,160,160);}\
                                                                        QPushButton:pressed{background-color:rgba(100,100,100);}");
    connect(repa,&QPushButton::clicked,this,&Widget::repar);



    QFont fontt("华文行楷",15,5,0);
    QLabel *Register=new QLabel;
    Register->setParent(this);
    Register->move(xreg+40,yreg-30);
    Register->setText("寄存器");
    Register->setFont(fontt);
    Register->show();

    QLabel *Res=new QLabel;
    Res->setParent(this);
    Res->move(xrest+200,yrest-30);
    Res->setText("保留站");
    Res->setFont(fontt);
    Res->show();


    QLabel *timetable=new QLabel;
    timetable->setParent(this);
    timetable->move(xinst+100,yinst-30);
    timetable->setText("指令时间表");
    timetable->setFont(fontt);
    timetable->show();


    QLabel *mp=new QLabel;
    mp->setParent(this);
    mp->move(1400,25);
    mp->setText("各模块个数");
    mp->setFont(fontt);
    mp->show();

    QLabel *tp=new QLabel;
    tp->setParent(this);
    tp->move(1600,25);
    tp->setText("指令需要周期");
    tp->setFont(fontt);
    tp->show();

    QLabel *tips=new QLabel;
    tips->setParent(this);
    tips->move(1400,400);
    tips->setText("1.修改好参数需点击修改参数生效\n2.载入错误格式的指令文件将退出\n3.使用过多的部件是允许的，但是超出显示长度的部\n分将不予显示\n4.保存的文件用记事本打开可能产生错位，建议使用\nVScode打开");
    tips->setFont(fontt);
    tips->show();

    QLabel *sign=new QLabel;
    sign->setParent(this);
    sign->move(1750,960);
    sign->setText("Designed by @Xu_Shiro\nCopyright infringement.");
    sign->show();


    int x=xreg,y=yreg,t=treg;
    for(int i=0;i<this->sim.mnum[3];i++)
    {
        QLabel * riglab=new QLabel;
        riglab->setParent(this);
        QString f="R";
        QString n=QString::number(i);
        f.append(n).append(":");
        riglab->setText(f);
        riglab->move(x,y);
        riglab->show();
        this->rigsname.push_back(riglab);
        y+=t;
    }

    int x1=xreg+regnn,y1=yreg;
    for(int i=0;i<this->sim.mnum[3];i++)
    {
        QLabel * riglab=new QLabel;
        riglab->setParent(this);
        riglab->resize(30,10);
        QString f;
        if(this->sim.RegS[i].q==-1)
        {
            f=QString::number(this->sim.RegS[i].value.lv);
        }
        else {
            f=QString::fromStdString(this->sim.resname(this->sim.RegS[i].q));
        }
        riglab->setText(f);
        riglab->move(x1,y1);
        riglab->show();
        this->rigsnum.push_back(riglab);
        y1+=t;
    }


    x+=80;y=yreg;
    for(int i=0;i<this->sim.mnum[4];i++)
    {
        QLabel * riglab=new QLabel;
        riglab->setParent(this);
        QString f="F";
        QString n=QString::number(i);
        f.append(n).append(":");
        riglab->setText(f);
        riglab->move(x,y);
        riglab->show();
        this->rigsname.push_back(riglab);
        y+=t;
    }

    x1+=80;y1=yreg;
    for(int i=0;i<this->sim.mnum[4];i++)
    {
        QLabel * riglab=new QLabel;
        riglab->setParent(this);
        riglab->resize(30,10);
        QString f;
        if(this->sim.RegS[i+this->sim.mnum[3]].q==-1)
        {
            f=QString::number(this->sim.RegS[i+this->sim.mnum[3]].value.lv);
        }
        else {
            f=QString::fromStdString(this->sim.resname(this->sim.RegS[i+this->sim.mnum[3]].q));
        }
        riglab->setText(f);
        riglab->move(x1,y1);
        riglab->show();
        this->rigsnum.push_back(riglab);
        y1+=t;
    }

    QLabel *tttitle=new QLabel;
    tttitle->setParent(this);
    tttitle->setText("inruction         issue       execute        write");
    tttitle->move(xinst,yinst);
    tttitle->show();

    QLabel *restitle=new QLabel;
    restitle->setParent(this);
    restitle->setText("name     Busy      t     OP     Vj          Vk          Qj     Qk    A");
    restitle->move(xrest,yrest);
    restitle->show();

    int x2=xrest,y2=yrest+fromt;
    for(int i=0;i<this->sim.rssize;i++)
    {
        string name=this->sim.resname(i);
        QLabel* resn = new QLabel;
        resn->setParent(this);
        resn->setText(QString::fromStdString(name));
        resn->move(x2,y2);
        y2+=tlineres;
        resn->show();
        this->resname.push_back(resn);
    }

    int x3=xrest+55,y3=yrest+fromt;
    for(ReservationStation res:this->sim.RS)
    {
        string s;
        QLabel *resc=new QLabel;
        s+="                                                                                                                                        ";
        resc->setParent(this);
        resc->setText(QString::fromStdString(s));
        resc->move(x3,y3);
        y3+=tlineres;
        resc->show();
        this->rescon.push_back(resc);
    }


    QLabel* mnum=new QLabel[5];
    int mnumi=0;int numx=1400,numy=50,numt=50;
    string m[]={"LD","ADD","MUL","FReg","RReg"};
    string runtime[]={"LD","ADD","SUB","MUL","DIV"};
    while(mnumi<5)
    {
        mnum[mnumi].setParent(this);
        mnum[mnumi].move(numx,numy);
        mnum[mnumi].setText(QString::fromStdString(m[mnumi]));
        mnum[mnumi].show();
        numy+=numt;
        mnumi++;
    }

    QLabel* tinum=new QLabel[5];
    mnumi=0;numx=1600;numy=50;
    while(mnumi<5)
    {
        tinum[mnumi].setParent(this);
        tinum[mnumi].move(numx,numy);
        tinum[mnumi].setText(QString::fromStdString(runtime[mnumi]));
        tinum[mnumi].show();
        numy+=numt;
        mnumi++;
    }


    mnumi=0;numx=1450;numy=50;
    while(mnumi<5)
    {
        QTextEdit* tex=new QTextEdit;
        tex->setParent(this);
        tex->resize(50,20);
        tex->setText(QString::number(this->sim.mnum[mnumi]));
        tex->move(numx,numy);
        tex->show();
        numy+=numt;
        this->mnums.push_back(tex);
        mnumi++;
    }

    mnumi=0;numx=1650;numy=50;
    while(mnumi<5)
    {
        QTextEdit* tex=new QTextEdit;
        tex->setParent(this);
        tex->resize(50,20);
        tex->setText(QString::number(this->sim.time[mnumi]));
        tex->move(numx,numy);
        tex->show();
        numy+=numt;
        this->mtime.push_back(tex);
        mnumi++;
    }











}


Widget::~Widget()
{

}

void Widget::onClick()
{
    this->sim.step();
    this->refreshnum();
    return;

}

void Widget::refreshnum()
{
    for(int i=0;i<this->sim.mnum[3]+this->sim.mnum[4];i++)
    {
        QString f;
        if(this->sim.RegS[i].q==-1)
        {
            if(i<this->sim.mnum[3])f=QString::number(this->sim.RegS[i].value.lv);
            else f=QString::number(this->sim.RegS[i].value.fv);
        }
        else {
            f=QString::fromStdString(this->sim.resname(this->sim.RegS[i].q));
        }
        this->rigsnum[i]->setText(f);
    }

    vector<QLabel*>::iterator labit;
    labit=this->ins.begin();
    vector<InstructionStation>::iterator it;
    it=this->sim.IS.begin();
    while(it!=this->sim.IS.end())
    {
        QString f;
        string s;
        s+=it->ins.name;
        for(int i=0;i<6-it->ins.name.size();i++)s+=" ";
        s+=it->ins.me;
        for(int i=0;i<16-it->ins.me.size();i++)s+=" ";
        s+=to_string(it->ins.t[0]);
        for(int i=0;i<10-to_string(it->ins.t[0]).size();i++)s+=" ";
        s+=to_string(it->ins.t[1]);
        for(int i=0;i<3-to_string(it->ins.t[1]).size();i++)s+=" ";
        s+="-";
        for(int i=0;i<2;i++)s+=" ";
        s+=to_string(it->ins.t[2]);
        for(int i=0;i<11-to_string(it->ins.t[2]).size();i++)s+=" ";
        s+=to_string(it->ins.t[3]);
        for(int i=0;i<11;i++)s+=" ";

        f=QString::fromStdString(s);
        (*labit)->setText(f);
        it++;
        labit++;
    }

    string s="clock:";
    s+=to_string(this->sim.t);
    this->clock.setText(QString::fromStdString(s));

    labit=this->rescon.begin();
    for(ReservationStation res:this->sim.RS)
    {
        string s;
        if(res.busy)
        {

            s+="yes";
            for(int i=0;i<7;i++)s+=" ";
            s+=to_string(res.lasttime);
            for(int i=0;i<6-to_string(res.lasttime).size();i++)s+=" ";
            s+=res.op;
            for(int i=0;i<7-res.op.size();i++)s+=" ";
            if(res.op=="L.D")
            {
                s+=to_string(res.value[0].lv);
                for(int i=0;i<37-to_string(res.value[0].lv).size();i++)s+=" ";
                s+=to_string(res.value[1].lv);
                for(int i=0;i<5;i++)s+=" ";
            }
            else
            {
                   if(res.rname[0]==-1)
                   {
                       s+=to_string(res.value[0].fv);
                       for(int i=0;i<12-to_string(res.value[0].fv).size();i++)s+=" ";
                   }
                   else {
                       for(int i=0;i<12;i++)s+=" ";
                   }


                   if(res.rname[1]==-1)
                   {
                       s+=to_string(res.value[1].fv);
                       for(int i=0;i<12-to_string(res.value[1].fv).size();i++)s+=" ";
                   }
                   else {
                       for(int i=0;i<12;i++)s+=" ";
                   }


                   if(res.rname[0]!=-1)
                   {
                       s+=this->sim.resname(res.rname[0]);
                       for(int i=0;i<7-this->sim.resname(res.rname[0]).size();i++)s+=" ";
                   }
                   else {
                       for(int i=0;i<7;i++)s+=" ";
                   }

                   if(res.rname[1]!=-1)
                   {
                       s+=this->sim.resname(res.rname[1]);
                       for(int i=0;i<12-this->sim.resname(res.rname[1]).size();i++)s+=" ";
                   }
                   else {
                       for(int i=0;i<12;i++)s+=" ";
                   }



            }

        }
        else {
            s+="                                                                     ";
        }

        (*labit)->setText(QString::fromStdString(s));
        labit++;
    }


    return;

}

void Widget::load(){
    if(this->ins.size()!=0)return;
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("txt(*.txt);;Allfile(*.*)"));
    //QString s=address.toPlainText();
    //string ads=s.toStdString();
    ifstream ops;
    ops.open(fileName.toStdString(),ios::in);
    this->sim.insload(ops);
    vector<InstructionStation>::iterator it;
    it=this->sim.IS.begin();
    int x=xinst,y=yinst+fromt;
    while(it!=this->sim.IS.end())
    {
        QLabel* inslab=new QLabel;
        inslab->setParent(this);
        QString f;
        string s;
        s+=it->ins.name;
        for(int i=0;i<6-it->ins.name.size();i++)s+=" ";
        s+=it->ins.me;
        for(int i=0;i<16-it->ins.me.size();i++)s+=" ";
        s+=to_string(it->ins.t[0]);
        for(int i=0;i<10-to_string(it->ins.t[0]).size();i++)s+=" ";
        s+=to_string(it->ins.t[1]);
        for(int i=0;i<3-to_string(it->ins.t[1]).size();i++)s+=" ";
        s+="-";
        for(int i=0;i<2;i++)s+=" ";
        s+=to_string(it->ins.t[2]);
        for(int i=0;i<11-to_string(it->ins.t[2]).size();i++)s+=" ";
        s+=to_string(it->ins.t[3]);
        for(int i=0;i<11;i++)s+=" ";

        f=QString::fromStdString(s);
        inslab->setText(f);
        inslab->move(x,y);
        inslab->show();
        y+=tlineins;
        this->ins.push_back(inslab);
        it++;
    }
}

void Widget::run()
{
    this->stop=false;
    while(this->sim.step())
    {
        this->refreshnum();
        if(this->stop)break;
        sleep(1000);
    }
    this->refreshnum();
    return;
}

void Widget::clear()
{
    this->sim.simclear();
    vector<QLabel*>::iterator it;
    it=ins.begin();
    while(it!=ins.end())
    {
          QLabel* n=*it;
          delete n;
          it++;
    }
    this->ins.clear();
    this->refreshnum();
    this->stop=true;
    return;
}

void Widget::stopr()
{
    this->stop=true;
    return;
}

void Widget::save()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Open Config"),
            "",
            tr("Config Files (*.txt)"));

        if (!fileName.isNull())
        {
            ofstream file(fileName.toStdString());
            streambuf *x = cout.rdbuf( file.rdbuf( ) );
            this->sim.printsim();
            cout.rdbuf(x);
        }
            return;


}

void Widget::repar()
{
    if(this->ins.size()>0)this->clear();
    int i=0;
    while(i<5)
    {
        this->sim.time[i]=this->mtime[i]->toPlainText().toInt();
        i++;
    }
    i=0;
    while(i<5)
    {
        this->sim.mnum[i]=this->mnums[i]->toPlainText().toInt();
        i++;
    }

    this->sim.simclear();

    vector<QLabel*>::iterator labit;

        labit=this->rigsnum.begin();
        while(labit!=this->rigsnum.end())
        {
            QLabel* n=(*labit);
            delete n;
            labit++;
        }

        labit=this->rigsname.begin();
        while(labit!=this->rigsname.end())
        {
            QLabel* n=(*labit);
            delete n;
            labit++;
        }


        labit=this->resname.begin();
        while(labit!=this->resname.end())
        {
            QLabel* n=(*labit);
            delete n;
            labit++;
        }


        labit=this->rescon.begin();
        while(labit!=this->rescon.end())
        {
            QLabel* n=(*labit);
            delete n;
            labit++;
        }



    this->rigsnum.clear();
    this->rigsname.clear();
    this->resname.clear();
    this->rescon.clear();
    this->rigsnum.resize(0);
    this->rigsname.resize(0);
    this->resname.resize(0);
    this->rescon.resize(0);



        int x=xreg,y=yreg,t=treg;
            for(int i=0;i<this->sim.mnum[3];i++)
            {
                QLabel * riglab=new QLabel;
                riglab->setParent(this);
                QString f="R";
                QString n=QString::number(i);
                f.append(n).append(":");
                riglab->setText(f);
                riglab->move(x,y);
                riglab->show();
                this->rigsname.push_back(riglab);
                y+=t;
            }

            int x1=xreg+regnn,y1=yreg;
            for(int i=0;i<this->sim.mnum[3];i++)
            {
                QLabel * riglab=new QLabel;
                riglab->setParent(this);
                riglab->resize(30,10);
                QString f;
                if(this->sim.RegS[i].q==-1)
                {
                    f=QString::number(this->sim.RegS[i].value.lv);
                }
                else {
                    f=QString::fromStdString(this->sim.resname(this->sim.RegS[i].q));
                }
                riglab->setText(f);
                riglab->move(x1,y1);
                riglab->show();
                this->rigsnum.push_back(riglab);
                y1+=t;
            }


            x+=80;y=yreg;
            for(int i=0;i<this->sim.mnum[4];i++)
            {
                QLabel * riglab=new QLabel;
                riglab->setParent(this);
                QString f="F";
                QString n=QString::number(i);
                f.append(n).append(":");
                riglab->setText(f);
                riglab->move(x,y);
                riglab->show();
                this->rigsname.push_back(riglab);
                y+=t;
            }

            x1+=80;y1=yreg;
            for(int i=0;i<this->sim.mnum[4];i++)
            {
                QLabel * riglab=new QLabel;
                riglab->setParent(this);
                riglab->resize(30,10);
                QString f;
                if(this->sim.RegS[i+this->sim.mnum[3]].q==-1)
                {
                    f=QString::number(this->sim.RegS[i+this->sim.mnum[3]].value.lv);
                }
                else {
                    f=QString::fromStdString(this->sim.resname(this->sim.RegS[i+this->sim.mnum[3]].q));
                }
                riglab->setText(f);
                riglab->move(x1,y1);
                riglab->show();
                this->rigsnum.push_back(riglab);
                y1+=t;
            }


            int x2=xrest,y2=yrest+fromt,t2=tlineres;
            for(int i=0;i<this->sim.rssize;i++)
            {
                string name=this->sim.resname(i);
                QLabel* resn = new QLabel;
                resn->setParent(this);
                resn->setText(QString::fromStdString(name));
                resn->move(x2,y2);
                y2+=t2;
                resn->show();
                this->resname.push_back(resn);
            }

            int x3=xrest+55,y3=yrest+fromt,t3=tlineres;
            for(ReservationStation res:this->sim.RS)
            {
                string s;
                QLabel *resc=new QLabel;
                s+="                                                                                                                                        ";
                resc->setParent(this);
                resc->setText(QString::fromStdString(s));
                resc->move(x3,y3);
                y3+=t3;
                resc->show();
                this->rescon.push_back(resc);
            }


    return;
}
