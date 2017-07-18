#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QVector>
#include <QRegExp>
#include <QMap>
#include <QMultiMap>
#include <windows.h>
#include <QTextCodec>
#include <algorithm>
#include <QMessageBox>



QVector<QStringList> MainWindow::DivisionIntoWords(QVector<QString>vec)
{
    int j = vec.size();
    QVector<QStringList>list(j);

    for (int i = 0;i<vec.size();i++)
    {
        list[i] = vec[i].split(QRegExp("[\\s+«»,;!?:^–*<>()/\]"), QString::SkipEmptyParts);
    }

    for (int i = 0;i<list.size();i++)
    {
        if (list[i].isEmpty())
            list.erase(list.begin()+i);
    }
    return list;
}


void MainWindow::CompareKeyWithAllWords(QFile *file)
{
    QString                 str;
    QVector<QString>        vec;
    QVector<QStringList>    lst;

    file->open(QIODevice::ReadOnly);
    for(;!file->atEnd();)
    {
        str = file->readLine();

        str.remove('\n');
        str.remove('\r');

        for(int i = 0;i<str.length();i++)
        {
            if(str[i]=='=')
                str.replace(i,1,' ');
        }

        vec.push_back(str);

    }
    file->close();

    lst = DivisionIntoWords(vec);

    for (int i = 0;i<lst.length();i++)
        map2.insert(lst[i][1],lst[i][0]);

    for (auto it = map2.begin();it!=map2.end();it++)
    {
        if (map.contains(it.key()))
        {
            QList<QString> values = map2.values(it.key());
            for (int i = 0; i < values.size(); ++i)
            {
                map3[map2.value(it.key())] = map[it.key()];
            }
        }
    }

}

void MainWindow::ReadKeyWords(QFile *file)
{
    QString                 str;
    QVector<QString>        vec;
    QVector<QStringList>    lst;

    file->open(QIODevice::ReadOnly);
    for(;!file->atEnd();)
    {
        str = file->readLine();

        str.remove('\n');
        str.remove('\r');

        for(int i = 0;i<str.length();i++)
        {
            if(str[i]=='=')
                str.replace(i,1,' ');
        }

        vec.push_back(str);

    }
    file->close();

        lst = DivisionIntoWords(vec);

        for (int i = 0;i < lst.length();i++)
        {
            map[lst[i][0]] = lst[i][1].toFloat();
        }
}


void MainWindow::KeySentences(QFile *file)
{
    QString             str,str2;
    QVector<QString>    vec;
    QVector<QStringList>list;

    file->open(QIODevice::ReadOnly);
    for(;!file->atEnd();)
    {
       str = file->readLine();

       for (int i =0 ;i < str.size();i++)
       {
            if(str[i] == '.' || str[i] == '?' || str[i] =='!' || str[i] == '\n'|| str[i]=='\r')
            {
                if (!str2.isEmpty())
                {
                   vec.push_back(str2);
                   str2.clear();
                }
            }
            else
            {
                str2+=str[i];
            }
        }
    }
    file->close();

    list = DivisionIntoWords(vec);

    int k = list.size();

    QVector<float>          sum(k);
    QVector<QVector<float>> weight(k);
    QVector<QStringList>    words(k);
    QVector<float>          weightKoef(k);
    QVector<float>          weightKoefBuf(k);

    for (int i = 0;i<list.size();i++)
         for (int j = 0;j<list[i].size();j++)
            if (map3.contains(list[i][j]))
            {
                words[i].push_back(list[i][j]);
                weight[i].push_back(map3[list[i][j]]);
            }


    for (int i = 0;i<weight.size();i++)
    {
        for (int j = 0;j < weight[i].size();j++)
        {
            sum[i]+=weight[i][j];
        }
    }

    for (int i = 0;i<list.size();i++)
    {
       weightKoef[i] = sum[i]/list[i].size();
    }

    int kolvo;
    float t;

    float procent = ((float)vec.size())/100*30;

    kolvo = procent;

    weightKoefBuf = weightKoef;

    for (int i = 0;i<weightKoef.size();i++)
    {
        for ( int j = i+1; j < weightKoef.size();j++)
        {
            if (weightKoef[i] > weightKoef[j])
            {
                    t = weightKoef[i];
                    weightKoef[i] = weightKoef[j];
                    weightKoef[j] = t;
            }
        }
    }


    float VesOut = weightKoef[weightKoef.size() - kolvo];


    for (int i = 0; i < vec.size(); i++)
    {
        if(weightKoefBuf[i] >= VesOut && list[i].size() > 10)
        {
            ui->textEdit->append(vec[i]);
        }
    }
}

void MainWindow::SimmetricRef(QFile *file)
{
    QString             str, str2;
    QVector<QString>    vec;
    QVector<QStringList>list,list2;

    file->open(QIODevice::ReadOnly);
    for (;!file->atEnd();)
    {
        str = file->readLine();
        for (int i = 0;i < str.size();i++)
        {
            if(str[i]=='.' || str[i] == '?' || str[i] == '!' || str[i] == '\r' || str[i] == '\n')
            {
                if (!str2.isEmpty())
                {
                    vec.push_back(str2);
                    str2.clear();
                }
            }
            else
            {
                str2+=str[i];
            }
        }
    }
    file->close();

    list = DivisionIntoWords(vec);

    list2 = list;

    int j = list2.size();
    QVector<int>ConNum(j,0);
    QVector<int>ConNumBuf(j,0);


    bool key = false;

    for (int i = 0;i < list2.size();i++)
    {
        for (int k = 0;k < list2[i].size();k++)
        {
           for (auto it = map2.begin(); it!=map2.end();it++)
            {
               key = false;
              QList<QString> values = map2.values(it.key());
                for (int m = 0; m < values.size(); m++)
                {
                    if (list2[i][k] == values[m])
                    {
                        list2[i][k] = it.key();
                        key = true;
                        break;
                    }
                }
                if (key)
                    break;
            }
        }
    }


    for (int i = 0;i < list2.size();i++)
    {
        for (int k = 0;k < list2[i].size();k++)
        {
            if (i != list2.size()-1)
            {
                for (int l = 0;l < list2[i+1].size();l++)
                {
                    if (list2[i][k] == list2[i+1][l])
                    {
                        ConNum[i]+=1;
                    }
                }
            }

            if(i != 0)
            {
                for (int l = 0;l < list2[i-1].size();l++)
                {
                    if (list2[i][k] == list2[i-1][l])
                    {
                        ConNum[i]+=1;
                    }
                }
            }
        }
    }

    int kolvo, t;

    float procent = ((float)vec.size())/100*30;

    kolvo = procent;

    ConNumBuf = ConNum;

    for (int i = 0;i<ConNum.size();i++)
    {
        for ( int j = i+1; j < ConNum.size();j++)
        {
            if (ConNum[i] > ConNum[j])
            {
                    t = ConNum[i];
                    ConNum[i] = ConNum[j];
                    ConNum[j] = t;
            }
        }
    }

    float VesOut = ConNum[ConNum.size() - kolvo];

    for (int i = 0; i < vec.size(); i++)
    {
        if(ConNumBuf[i] > VesOut+1)
        {
            ui->textEdit->append(vec[i]);
        }
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->lineEdit->setText(QFileDialog::getOpenFileName(this, tr("Open text"), "", tr("text Files (*.txt)")));
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->lineEdit_2->setText(QFileDialog::getOpenFileName(this, tr("Open text"), "", tr("text Files (*.txt)")));
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->lineEdit_3->setText(QFileDialog::getOpenFileName(this, tr("Open text"), "", tr("text Files (*.txt)")));
}

void MainWindow::on_pushButton_4_clicked()
{
    QFile kartochki         (ui->lineEdit->text());
    QFile keyWordsFile      (ui->lineEdit_2->text());
    QFile forms431          (ui->lineEdit_3->text());

    //checking that files are open

    if(!keyWordsFile.exists())
    {
        QMessageBox msgBox;
        msgBox.setText("Error : Can't open the first file");
        msgBox.exec();
        return;
    }

    if(!forms431.exists())
    {
        QMessageBox msgBox;
        msgBox.setText("Error : Can't open the second file");
        msgBox.exec();
        return;
    }

    if(!kartochki.exists())
    {
        QMessageBox msgBox;
        msgBox.setText("Error : Can't open the third file");
        msgBox.exec();
        return;
    }

    ReadKeyWords(&keyWordsFile);

    CompareKeyWithAllWords(&forms431);

    map.clear();
    map2.clear();
    map3.clear();


    ui->textEdit->clear();

    if(ui->radioButton->isChecked())
        KeySentences(&kartochki);
    else
        SimmetricRef(&kartochki);
}
