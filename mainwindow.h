#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListView>
#include <QMainWindow>
#include <QFrame>
#include "puzzlewidget.h"
#include "piecesmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadImage(QString &path);
    void setupPuzzle();
    QFrame* setup_list();

private slots:
    void on_actionOuvrir_triggered();
    void on_done_clicked();

    void on_upload_clicked();

    void on_done1_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap puzzleImage;
    int nb_triangles;
    PuzzleWidget *puzzleWidget;
    piecesModel *model;
    QListView *piecesList;

};
#endif // MAINWINDOW_H
