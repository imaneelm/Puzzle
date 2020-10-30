#ifndef PIECESMODEL_H
#define PIECESMODEL_H
#include <QAbstractListModel>
#include <QPixmap>
#include <QVector>
#include <QPoint>

class piecesModel : public QAbstractListModel
{
public:
    piecesModel(int pieceSize, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
       Qt::ItemFlags flags(const QModelIndex &index) const override;
       bool removeRows(int row, int count, const QModelIndex &parent) override;

       bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                         int row, int column, const QModelIndex &parent) override;
       QMimeData *mimeData(const QModelIndexList &indexes) const override;
       QStringList mimeTypes() const override;
       int rowCount(const QModelIndex &parent) const override;
       Qt::DropActions supportedDropActions() const override;

    void addPiece(const QPixmap &pixmap, const QPoint &location);
    void addPieces(QPixmap &puzzleImage, int nb_triangles);



private:

    QVector<QPixmap> pixmaps;
    QVector<QPoint> locations;
    int pieceSize;

};

#endif // PIECESMODEL_H
