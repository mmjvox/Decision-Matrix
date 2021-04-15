#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QJsonObject ahpObject READ getAhpObject WRITE setAhpObject NOTIFY ahpObjectChanged CONSTANT)
    Q_PROPERTY(QJsonObject clustersObject READ getClustersObject WRITE setClustersObject NOTIFY clustersObjectChanged CONSTANT)
    enum TableRoles{
        TableDataRole = Qt::UserRole + 1,
        HeadingRole,
        ColumnIndex,
        RowIndex,
        ExpandMode,
        AHPkey,
        SelectedSAW,
        SelectedSAWValue,
        WeightedAverage,
        ListData,
        Positive,
        Weight,
        MaxScore
    };

public:
    explicit TableModel();

    QJsonObject clustersObject,ahpObject;
    int columnsCount=0,rowsCount=0;

    int rowCount(const QModelIndex & = QModelIndex()) const override;

    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    QVariant cellDataMap(int column, int row) const;
    QString getAHPkey(int column) const;
    int getSelectedSaw(int column, int row) const;
    int getSelectedSawValue(int column, int row) const;
    QStringList ahpValuesList(int column) const;
    double getWeight(int column) const;
    double eachRowSum(int column, int row) const;
    double matrixAllSum() const;
    int getMaxSaw(int column) const;
    QString fix(double dbl,int prec) const;
    double getScore(int row) const;
    void getMaxScore();


    Q_INVOKABLE void setExpandMode(int column,int mode);
    Q_INVOKABLE void setSelected(QString key, int row, int columnIndex, int comboindex);


    QJsonObject getAhpObject() const;
    void setAhpObject(QJsonObject ahp);
    QJsonObject getClustersObject() const;
    void setClustersObject(QJsonObject cluster);

//    Q_INVOKABLE void callLCH();

signals:
    void ahpObjectChanged();
    void clustersObjectChanged();
    void clustersObjectEdited();
    void expandModeChanged(int changedCol);
    void exportMaxData(double score, QString clusterName, int level, QString levelName);

private:
    QVector<QVector<QString>> table;
    QMap<int,int> expandMode;
    double maxScore=0;
    int maxRow=0;
};

#endif // TABLEMODEL_H
