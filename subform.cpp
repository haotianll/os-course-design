﻿#include "subform.h"
#include "ui_subform.h"
#include <QKeyEvent>
#include <iostream>
using namespace std;

SubForm::SubForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SubForm)
{
    ui->setupUi(this);
    isEdit = false;

    ui->textEdit->setFocus();
    ui->textEdit->installEventFilter(this);

    ui->textEdit->setTextColor(QColor(255,0,255));
    ui->textEdit->append("$ ");
    ui->textEdit->setTextColor(QColor(0,0,0));

}

SubForm::~SubForm()
{
    delete ui;
}

//没有编辑文件时按下enter键时的操作
void SubForm::returnReaction()
{
    this->reactionFunction();
    if(!isEdit){
        ui->textEdit->setTextColor(QColor(255,0,255));
        ui->textEdit->append("\n$ ");
        ui->textEdit->setTextColor(QColor(0,0,0));
    }
}

void SubForm::reactionFunction()
{
    ml = ui->textEdit->toPlainText();
    QStringList mlist = ml.split("\n");
    ml = mlist[mlist.size()-1];
    //ml为按下回车前该行的内容
    getFun();
}

void SubForm::getFun()
{
    if(ml == "$ "){
        return;
    }
    QStringList mls = ml.split(" ");
    mlings.clear();
    for(int i = 1; i < mls.size(); i++){
        if(mls[i] != ""){
            mlings.append(mls[i]);
        }
    }
    if(mlings.at(mling) == cat || mlings.at(mling) == rm || mlings[mling] == vi){
        if(mlings.size() == 2){
            if(mlings[mling] == vi){
                isEdit = true;
                editFile();
            }
        }else{
            outputerror();
        }

    }else if(mlings[mling] == ls){
        if(mlings.size() == 2){
            QString t = mlings[1];
            if(!t.startsWith("/") || t.endsWith("/")){
                outputerror();
            }
            int tt = 0;
            QStringList temp = t.split("/");
            //temp中保存文件的路径，根据路径访问文件等
//            tosuper.append(mlings[0]);
            for(int i = 0; i < temp.size(); i++){
                if(temp[i] != ""){
//                    tosuper.append(temp[i]);
                    ui->textEdit->append(temp[i]);
                    tt = 0;
                }else{
                    tt++;
                    if(tt == 2){
                        outputerror();
                        break;
                    }
                }
            }
        }else{
            outputerror();
        }
    }else{
        outputerror();
    }
}

void SubForm::outputerror()
{
    ui->textEdit->setTextColor(QColor(255,0,0));
    ui->textEdit->append("input error!");
    ui->textEdit->setTextColor(QColor(0,0,0));
}

void SubForm::editFile()
{
    ui->textEdit->clear();
}

void SubForm::setName(QString string)
{
    name = string;
//    THIS = dir;
}

bool SubForm::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->textEdit){
        if (event->type() == QEvent::KeyPress) {
                QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
                if(!isEdit && keyEvent->key() == Qt::Key_Return){
                    returnReaction();
                    return true;
                }
                if(isEdit && keyEvent->key() == Qt::Key_F1){
                    //添加操作，当按下F1键时结束写文件
                    newfile = ui->textEdit->toPlainText();
                    ui->textEdit->clear();
                    ui->textEdit->setTextColor(QColor(255,0,255));
                    ui->textEdit->append("$ ");
                    ui->textEdit->setTextColor(QColor(0,0,0));
                    isEdit = false;
                    return true;
                }
        }
    }
    return QWidget::eventFilter(obj,event);
}
