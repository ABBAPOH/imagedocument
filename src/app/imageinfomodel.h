#ifndef IMAGEINFOMODEL_H
#define IMAGEINFOMODEL_H

#include <ImageContents>
#include <QtCore/QAbstractTableModel>

class ImageInfoModel : public QAbstractTableModel
{
public:
    enum Column {
        ColumnName,
        ColumnValue,
        ColumnCount
    };

    enum Row {
        RowType,
        RowSize,
        RowImageFormat,
        RowName,
        RowImageCount,
        RowMipmapCount,
        RowImageDelay,
        RowLoopCount,
        RowCount
    };

    explicit ImageInfoModel(QObject *parent = nullptr);

    ImageContents contents() const;
    void setImageContents(const ImageContents &contents);

public: // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    ImageContents _contents;
};

#endif // IMAGEINFOMODEL_H
