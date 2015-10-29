#ifndef VARIANTMAPMODEL_H
#define VARIANTMAPMODEL_H

#include <QtCore/QAbstractTableModel>

class VariantMapModel : public QAbstractTableModel
{
public:
    enum Column { Key, Value };
    explicit VariantMapModel(QObject *parent = nullptr);

public: // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void setVariantMap(const QVariantMap &map);

signals:

public slots:

private:
    struct Data { QString key; QVariant value; };
    QVector<Data> _data;
};

#endif // VARIANTMAPMODEL_H
