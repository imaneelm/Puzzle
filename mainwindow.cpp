#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <stdlib.h>

#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->done1->setDisabled(true);
    QPixmap icon(":/Ressources/images/2-triangles.png");
    int w = ui->choix->width();
    int h = ui->choix->height();
    ui->choix->setPixmap(icon.scaled(w,h,Qt::KeepAspectRatio));
    ui->stackedWidget->insertWidget(2,setup_list());
    model = new piecesModel(puzzleWidget->pieceSize(), this);
    piecesList->setModel(model);

}

MainWindow::~MainWindow()
{
    delete ui;
}
/* choisir une autre image pour le puzzle*/
void MainWindow::on_actionOuvrir_triggered()
{
    //ouvrir une image (la boite de dialogue va s'ouvrir directement dans le dossier image de l'utilisateur)
    const QString directory =
        QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).value(0, QDir::homePath());

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                   directory,
                                                    tr("Images (*.png *.xpm *.jpg)"));
    if (!fileName.isEmpty())
        {
             loadImage(fileName);
        }

}
/* si l'utilisateur ne choisit aucune image il aura un warning sinon l'image va être découper*/
void MainWindow::loadImage(QString &fileName){

    QPixmap newImage;
    if (!newImage.load(fileName)) {
        QMessageBox::warning(this, tr("Open Image"),
                             tr("The image file could not be loaded."),
                             QMessageBox::Close);
        return;
    }
    puzzleImage = newImage;
    setupPuzzle();
}


/*slot du premier bouton done pour le choix du nombre de triangles*/
void MainWindow::on_done_clicked()
{
     nb_triangles= ui->nbtriangles->value();

     ui->stackedWidget->setCurrentIndex(1);

}
/* slot pour choisir l'image */
void MainWindow::on_upload_clicked()
{
    const QString directory =QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).value(0, QDir::homePath());

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                               directory,
                                                tr("Images (*.png *.xpm *.jpg)"));
    QPixmap newImage;
    bool valid =  newImage.load(fileName);
    if (!valid) {
        QMessageBox::warning(this, tr("Open Image"),
                             tr("The image file could not be loaded."),
                             QMessageBox::Close);
        return;
    }
    else {
          newImage = newImage.scaledToWidth(ui->chosenImage->width(),Qt::SmoothTransformation);
          puzzleImage = newImage;
          ui->chosenImage->setPixmap(newImage);
          ui->done1->setDisabled(false);

    }
    setupPuzzle();
}
/* slot pour passer à l'interface du puzzle */
void MainWindow::on_done1_clicked()
{
     ui->stackedWidget->setCurrentIndex(2);
}


/*slot pour revenir en arrière */
void MainWindow::on_pushButton_clicked()
{
   ui->stackedWidget->setCurrentIndex(0);
}

/*Cette fonction est responspable de la mise en place de la liste des pièce: elle fait appel à addPieces */
void MainWindow::setupPuzzle()
{
    int size = qMin(puzzleImage.width(), puzzleImage.height());
    puzzleImage = puzzleImage.copy((puzzleImage.width() - size) / 2,
        (puzzleImage.height() - size) / 2, puzzleImage.width(), puzzleImage.width()).scaled(puzzleWidget->imageSize(),
            puzzleWidget->imageSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    model->addPieces(puzzleImage,nb_triangles);
    puzzleWidget->clear();
}
/* l'interface graphique de la troisième page (le puzzle)*/
QFrame* MainWindow::setup_list(){

    QFrame *frame = new QFrame;
        QHBoxLayout *frameLayout = new QHBoxLayout(frame);

        puzzleWidget = new PuzzleWidget(400);

        piecesList = new QListView;
        piecesList->setDragEnabled(true);
        piecesList->setViewMode(QListView::IconMode);
        piecesList->setIconSize(QSize(puzzleWidget->pieceSize() - 20, puzzleWidget->pieceSize() - 20));
        piecesList->setGridSize(QSize(puzzleWidget->pieceSize(), puzzleWidget->pieceSize()));
        piecesList->setSpacing(10);
        piecesList->setMovement(QListView::Snap);
        piecesList->setAcceptDrops(true);
        piecesList->setDropIndicatorShown(true);

        piecesModel *model = new piecesModel(puzzleWidget->pieceSize(), this);
        piecesList->setModel(model);

        frameLayout->addWidget(piecesList);
        frameLayout->addWidget(puzzleWidget);
        return frame;

}

