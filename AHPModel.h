#ifndef AHPMODEL_H
#define AHPMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <nlohmann/json.hpp>

#include <QDebug>

using json = nlohmann::json;

class AHPModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count CONSTANT)
    Q_PROPERTY(bool editMode READ getEditMode WRITE setEditMode NOTIFY editModeChanged CONSTANT)
    Q_PROPERTY(QString indicatorName READ getIndicatorName WRITE setIndicatorName CONSTANT)
    Q_PROPERTY(QJsonObject ahpObject READ getAhpObject WRITE setAhpObject NOTIFY ahpObjectChanged CONSTANT)
    enum TableRoles{
        TableDataRole = Qt::UserRole + 1,
        Diameter,
        ColumnIndex,
        RowIndex,
        ExpandMode,
        AHPkey,
        EditMode,
        EditModeCell,
        Numerator,
        Denominator
    };

public:
    explicit AHPModel();

    QJsonObject getAhpObject() const;
    void setAhpObject(QJsonObject ahp);

    int columnsCount=0,rowsCount=0;
    bool editMode=false;

    Q_INVOKABLE int count() const;
    int rowCount(const QModelIndex & = QModelIndex()) const override;

    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void setExpandMode(int column,int mode);
    bool getEditMode() const;
    void setEditMode(bool ed);

    QVariant cellDataMap(int column, int row) const;
    double eachRowSum(int column, int row) const;
    double matrixAllSum() const;

    Q_INVOKABLE void addNewOne(QString newName);
    Q_INVOKABLE void removeItem(QString keyName);

    void setIndicatorName(QString indname);
    QString getIndicatorName();

    Q_INVOKABLE void set_AHP_values(QString name,QString key, QJsonObject vals);
    Q_INVOKABLE void set_AHP_positive(QString name,QString key, bool pos);

signals:
    void ahpObjectChanged();
    void editModeChanged();

private:
    QJsonObject ahpObject;
    QString indicatorName;
};

#endif // AHPMODEL_H
