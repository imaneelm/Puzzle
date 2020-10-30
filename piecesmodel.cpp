#include "piecesmodel.h"
#include <QRandomGenerator>
#include <QBitmap>
#include <mainwindow.h>
#include <QDir>
#include <QMessageBox>
#include <QMimeData>

#include <QDebug>

piecesModel::piecesModel(int pieceSize, QObject *parent)
    : QAbstractListModel(parent),pieceSize(pieceSize)
{

}
QVariant piecesModel::data(const QModelIndex &index, int role ) const{


    if (!index.isValid())
        return QVariant();

    if (role == Qt::DecorationRole)
        return QIcon(pixmaps.value(index.row()).scaled(pieceSize, pieceSize,
                         Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else if (role == Qt::UserRole)
        return pixmaps.value(index.row());
    else if (role == Qt::UserRole + 1)
        return locations.value(index.row());

    return QVariant();

}
Qt::ItemFlags piecesModel::flags(const QModelIndex &index) const {

    if (index.isValid())
        return (QAbstractListModel::flags(index)|Qt::ItemIsDragEnabled);

    return Qt::ItemIsDropEnabled;
}
bool piecesModel::removeRows(int row, int count, const QModelIndex &parent) {

    if (parent.isValid())
        return false;

    if (row >= pixmaps.size() || row + count <= 0)
        return false;

    int beginRow = qMax(0, row);
    int endRow = qMin(row + count - 1, pixmaps.size() - 1);

    beginRemoveRows(parent, beginRow, endRow);

    while (beginRow <= endRow) {
        pixmaps.removeAt(beginRow);
        locations.removeAt(beginRow);
        ++beginRow;
    }

    endRemoveRows();
    return true;
}

bool piecesModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                               int row, int column, const QModelIndex &parent) {

    if (!data->hasFormat("image/x-puzzle-piece"))
        return false;

    if (action == Qt::IgnoreAction)
        return true;

    if (column > 0)
        return false;

    int endRow;

    if (!parent.isValid()) {
        if (row < 0)
            endRow = pixmaps.size();
        else
            endRow = qMin(row, pixmaps.size());
    } else {
        endRow = parent.row();
    }

    QByteArray encodedData = data->data("image/x-puzzle-piece");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    while (!stream.atEnd()) {
        QPixmap pixmap;
        QPoint location;
        stream >> pixmap >> location;

        beginInsertRows(QModelIndex(), endRow, endRow);
        pixmaps.insert(endRow, pixmap);
        locations.insert(endRow, location);
        endInsertRows();

        ++endRow;
    }

    return true;


}
QMimeData* piecesModel::mimeData(const QModelIndexList &indexes) const {
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    for (const QModelIndex &index : indexes) {
        if (index.isValid()) {
            QPixmap pixmap = qvariant_cast<QPixmap>(data(index, Qt::UserRole));
            QPoint location = data(index, Qt::UserRole+1).toPoint();
            stream << pixmap << location;
        }
    }

    mimeData->setData("image/x-puzzle-piece", encodedData);
    return mimeData;

}
QStringList piecesModel::mimeTypes() const {
    QStringList types;
    types << "image/x-puzzle-piece";
    return types;

}
int piecesModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : pixmaps.size();

}
Qt::DropActions piecesModel::supportedDropActions() const{
    return Qt::CopyAction | Qt::MoveAction;

}



void piecesModel::addPiece(const QPixmap &pixmap, const QPoint &location) {
    int row;
    //Pour que les pièce se place d'une façon arbitraire
    if (QRandomGenerator::global()->bounded(2) == 1)
        row = 0;
    else
        row = pixmaps.size();

    //l'insertion dans la position row
    beginInsertRows(QModelIndex(), row, row);
    pixmaps.insert(row, pixmap);
    locations.insert(row,location);
    endInsertRows();
}

void piecesModel::addPieces(QPixmap &puzzleImage, int nb_triangles)
{
    QPixmap mask1(":/Ressources/images/triangle.png");
    QPixmap mask2(":/Ressources/images/triangle1.png");
    mask1= mask1.scaled(pieceSize,pieceSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    mask2= mask2.scaled(pieceSize,pieceSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    QBitmap mask_a(mask1);
    QBitmap mask_b(mask2);
    if (!pixmaps.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, pixmaps.size() - 1);
        pixmaps.clear();
        locations.clear();
        endRemoveRows();
    }
   /* QMessageBox msg(nullptr);
    msg.warning(nullptr,tr("Open Image"),
                path,
                QMessageBox::Close);*/


    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            QPixmap pieceImage = puzzleImage.copy(x*pieceSize, y*pieceSize, pieceSize, pieceSize);
            if(nb_triangles==0){

                addPiece(pieceImage, QPoint(x, y));
                pieceImage = puzzleImage.copy(x*pieceSize, y*pieceSize, pieceSize, pieceSize);

            }
            /*else if (nb_triangles == 25 ) {
                pieceImage.setMask(mask_a);
                addPiece(pieceImage, QPoint(x, y));
                pieceImage = puzzleImage.copy(x*pieceSize, y*pieceSize, pieceSize, pieceSize);
                pieceImage.setMask(mask_b);
                addPiece(pieceImage, QPoint(x, y));

            }
            else if (nb_triangles == 0 ) {
                addPiece(pieceImage, QPoint(x, y));
                pieceImage = puzzleImage.copy(x*pieceSize, y*pieceSize, pieceSize, pieceSize);

            }*/
            else{
                pieceImage.setMask(mask_a);
                addPiece(pieceImage, QPoint(x, y));
                pieceImage = puzzleImage.copy(x*pieceSize, y*pieceSize, pieceSize, pieceSize);
                pieceImage.setMask(mask_b);
                addPiece(pieceImage, QPoint(x, y));
                nb_triangles= nb_triangles -1;
            }

        }
    }
}

