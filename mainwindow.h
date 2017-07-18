#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QMap>
#include <QMultiMap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QMap<QString, float>        map;
    QMultiMap<QString, QString> map2;
    QMap<QString, float>        map3;

    QVector<QStringList> DivisionIntoWords(QVector<QString>vec);
    void CompareKeyWithAllWords(QFile *file);
    void ReadKeyWords(QFile *file);
    void KeySentences(QFile *file);
    void SimmetricRef(QFile *file);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
