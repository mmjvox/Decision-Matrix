#include "AHPModel.h"

AHPModel::AHPModel(){

}

QJsonObject AHPModel::getAhpObject() const
{
    qDebug()<<__func__;
    return ahpObject;
}
void AHPModel::setAhpObject(QJsonObject ahp)
{
    qDebug()<<ahp;
    ahpObject=ahp;
    rowsCount=ahpObject.count()+1;
    columnsCount=ahpObject.count()+2;
    emit layoutChanged();
    emit ahpObjectChanged();
}

int AHPModel::rowCount(const QModelIndex &) const
{
    qDebug()<<__func__;
    return rowsCount;
}

int AHPModel::columnCount(const QModelIndex &) const
{
    qDebug()<<__func__<<"AHPModel";
    return columnsCount;
}
int AHPModel::count() const
{
    qDebug()<<__func__;
    return ahpObject.count();
}

bool AHPModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    qDebug()<<__func__;
    switch (role) {
    case Numerator:
    {
        auto keys=ahpObject.keys();
        json json=json::parse( QJsonDocument(ahpObject).toJson(QJsonDocument::Compact).toStdString() );
        json[keys.value(index.column()-1).toStdString()]["others"][keys.value(index.row()-1).toStdString()]["numerator"]=value.toInt();
        ahpObject=QJsonDocument::fromJson(QByteArray::fromStdString(json.dump())).object();
        emit ahpObjectChanged();
    }
        break;
    case Denominator:
    {
        auto keys=ahpObject.keys();
        json json=json::parse( QJsonDocument(ahpObject).toJson(QJsonDocument::Compact).toStdString() );
        json[keys.value(index.column()-1).toStdString()]["others"][keys.value(index.row()-1).toStdString()]["denominator"]=value.toInt();
        ahpObject=QJsonDocument::fromJson(QByteArray::fromStdString(json.dump())).object();
        emit ahpObjectChanged();
    }
        break;
    case TableDataRole:
    {
        if(index.row()!=0)
        {
            if(index.column()==0)
            {
                auto keys=ahpObject.keys();
                json json=json::parse( QJsonDocument(ahpObject).toJson(QJsonDocument::Compact).toStdString() );
                json[keys.value(index.row()-1).toStdString()]["name"]=value.toString().trimmed().toStdString();
                ahpObject=QJsonDocument::fromJson(QByteArray::fromStdString(json.dump())).object();
                emit ahpObjectChanged();
            }
        }
    }
        break;
    default:
        break;
    }
//    qDebug()<<ahpObject;
    return true;
}

QVariant AHPModel::data(const QModelIndex &index, int role) const
{
    qDebug()<<__func__;
    switch (role) {
    case TableDataRole:
    {
        return cellDataMap(index.column(),index.row());
    }
        break;
    case Diameter:
    {
        if( index.row()==index.column() && (index.column()!=0) )
            return true;
        else return false;

    }
        break;
    case ColumnIndex:
        return index.column();
        break;
    case RowIndex:
        return index.row();
        break;
    case ExpandMode:
    {
        //return expandMode.value(index.column());
    }
        break;
    case AHPkey:
    {
        QStringList ahpNames=ahpObject.keys();
        //std::reverse(ahpNames.begin(), ahpNames.end());
        return ahpNames.value(index.row()-1);
    }
        break;
    case EditMode:
    {
        return editMode;
    }
        break;
    case EditModeCell:
    {
        if(editMode)
        {
            if(index.row()==0)
            {
                if(index.column()==0)
                    return "qrc:/AHP_Add_BTN.qml";
                else if(index.column()==columnsCount-1)
                    return "qrc:/AHP_SimpleCell.qml";
                else
                {
                    return "qrc:/AHP_SimpleCell.qml";
                }

            } else {
                if(index.column()==0)
                {
                    return "qrc:/AHP_NamesEdit.qml";
                }
                else if(index.column()==columnsCount-1)
                {
                    return "qrc:/AHP_EMPTY.qml";
                }
                else {
                    if(index.row()>index.column()){
                        return "qrc:/AHP_EMPTY.qml";
                    } else {
                        return "qrc:/AHP_FractionEdit.qml";
                    }
                }
            }

        } else {
            return "qrc:/AHP_SimpleCell.qml";
        }
    }
        break;
    case Numerator:
    {
        auto keys=ahpObject.keys();
        return ahpObject.value(keys.value(index.column()-1)).toObject().value("others").toObject().value(keys.value(index.row()-1)).toObject().value("numerator").toDouble();
    }
        break;
    case Denominator:
    {
        auto keys=ahpObject.keys();
        return ahpObject.value(keys.value(index.column()-1)).toObject().value("others").toObject().value(keys.value(index.row()-1)).toObject().value("denominator").toDouble();
    }
        break;
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> AHPModel::roleNames() const
{
    qDebug()<<__func__;
    QHash<int, QByteArray> roles;
    roles[TableDataRole] = "tabledata";
    roles[Diameter] = "diameter";
    roles[ColumnIndex] = "columnIndex";
    roles[RowIndex] = "rowIndex";
    roles[ExpandMode] = "expandMode";
    roles[AHPkey] = "ahpKey";
    roles[EditMode] = "editMode";
    roles[EditModeCell] = "editModeCell";
    roles[Numerator] = "numerator";
    roles[Denominator] = "denominator";
    return roles;
}

void AHPModel::setExpandMode(int column, int mode)
{
    qDebug()<<__func__;
    //expandMode[column]=mode;
    emit layoutChanged();
}

QVariant AHPModel::cellDataMap(int column, int row) const
{
    qDebug()<<__func__;
    if(row==0)
    {
        if(column==0)
            return "";
        else if(column==columnsCount-1)
            return "وزن نهایی";
        else
        {
            auto keys=ahpObject.keys();
            //std::reverse(keys.begin(),keys.end());
            return ahpObject.value(keys.value(column-1)).toObject().value("name");
        }

    } else {
        if(column==0)
        {
            auto keys=ahpObject.keys();
            //std::reverse(keys.begin(),keys.end());
            return ahpObject.value(keys.value(row-1)).toObject().value("name");
        }
        else if(column==columnsCount-1)
        {
            QString num=QString::number(eachRowSum(column,row)/matrixAllSum(),'f',3);
            while(num.endsWith("0") || num.endsWith("."))
                num.remove(num.length()-1,1);
            return num;
        }
        else {
            auto keys=ahpObject.keys();
            //std::reverse(keys.begin(),keys.end());
            if(row>column){
                double numerator=ahpObject.value(keys.value(row-1)).toObject().value("others").toObject().value(keys.value(column-1)).toObject().value("numerator").toDouble();
                double denominator=ahpObject.value(keys.value(row-1)).toObject().value("others").toObject().value(keys.value(column-1)).toObject().value("denominator").toDouble();
                if(numerator!=0) {
                    double val=denominator/numerator;
                    QString num=QString::number(val,'f',3);
                    while(num.endsWith("0") || num.endsWith("."))
                        num.remove(num.length()-1,1);
                    return num;
                }  else {
                    return 0;
                }
            } else {
                double numerator=ahpObject.value(keys.value(column-1)).toObject().value("others").toObject().value(keys.value(row-1)).toObject().value("numerator").toDouble();
                double denominator=ahpObject.value(keys.value(column-1)).toObject().value("others").toObject().value(keys.value(row-1)).toObject().value("denominator").toDouble();
                if(denominator!=0) {
                    double val=numerator/denominator;
                    QString num=QString::number(val,'f',3);
                    while(num.endsWith("0") || num.endsWith("."))
                        num.remove(num.length()-1,1);
                    return num;
                }  else {
                    return 0;
                }
            }
        }
    }
    return QVariant("");
}

double AHPModel::eachRowSum(int column, int row) const
{
    qDebug()<<__func__;
    double rowSum=0;
    auto keys=ahpObject.keys();
    //std::reverse(keys.begin(),keys.end());
    for(int col=column-1;col>0;col--){
        if(col==row) continue;
        if(row>col)
        {
            double numerator=ahpObject.value(keys.value(row-1)).toObject().value("others").toObject().value(keys.value(col-1)).toObject().value("numerator").toDouble();
            double denominator=ahpObject.value(keys.value(row-1)).toObject().value("others").toObject().value(keys.value(col-1)).toObject().value("denominator").toDouble();
            if(numerator!=0)
                rowSum=rowSum+(denominator/numerator);
        } else {
            double numerator=ahpObject.value(keys.value(col-1)).toObject().value("others").toObject().value(keys.value(row-1)).toObject().value("numerator").toDouble();
            double denominator=ahpObject.value(keys.value(col-1)).toObject().value("others").toObject().value(keys.value(row-1)).toObject().value("denominator").toDouble();
            if(denominator!=0)
                rowSum=rowSum+(numerator/denominator);
        }
    }
    rowSum++;
    return rowSum;
}

double AHPModel::matrixAllSum() const
{
    qDebug()<<__func__;
    double rowSum=0,allSum=0;
    auto keys=ahpObject.keys();
    //std::reverse(keys.begin(),keys.end());
    for(int ro=rowsCount-1;ro>0;ro--){
        for(int col=columnsCount-1;col>0;col--){
            if(col==ro) continue;
            if(ro>col)
            {
                double numerator=ahpObject.value(keys.value(ro-1)).toObject().value("others").toObject().value(keys.value(col-1)).toObject().value("numerator").toDouble();
                double denominator=ahpObject.value(keys.value(ro-1)).toObject().value("others").toObject().value(keys.value(col-1)).toObject().value("denominator").toDouble();
                if(numerator!=0)
                    rowSum=rowSum+(denominator/numerator);
            } else {

                double numerator=ahpObject.value(keys.value(col-1)).toObject().value("others").toObject().value(keys.value(ro-1)).toObject().value("numerator").toDouble();
                double denominator=ahpObject.value(keys.value(col-1)).toObject().value("others").toObject().value(keys.value(ro-1)).toObject().value("denominator").toDouble();
                if(denominator!=0)
                    rowSum=rowSum+(numerator/denominator);
            }
        }
        allSum=allSum+rowSum+1;
        rowSum=0;
    }
    return allSum;
}

void AHPModel::setEditMode(bool ed)
{
    qDebug()<<__func__;
    editMode=ed;
    emit layoutChanged();
    emit editModeChanged();
}
bool AHPModel::getEditMode() const
{
    return editMode;
}

void AHPModel::addNewOne(QString newName)
{
    qDebug()<<__func__;
    auto keys=ahpObject.keys();
    QString lastKey=keys.count()>0? keys.last() : "";
    QString newKey="key "+QString::number(lastKey.right(1).toInt()+1);
    json json=json::parse( QJsonDocument(ahpObject).toJson(QJsonDocument::Compact).toStdString() );
    json[newKey.toStdString()]["name"]=newName.trimmed().toStdString();
    ahpObject=QJsonDocument::fromJson(QByteArray::fromStdString(json.dump())).object();

    rowsCount=ahpObject.count()+1;
    columnsCount=ahpObject.count()+2;

    emit layoutChanged();
    emit ahpObjectChanged();
}

void AHPModel::removeItem(QString keyName)
{
    qDebug()<<__func__;
    ahpObject.remove(keyName);

    json json=json::parse( QJsonDocument(ahpObject).toJson(QJsonDocument::Compact).toStdString() );
    for(QString key: ahpObject.keys())
    {
        if( json[key.toStdString()]["others"].contains(keyName.toStdString()) ){
            json[key.toStdString()]["others"].erase(json[key.toStdString()]["others"].find(keyName.toStdString()));
        }
    }
    ahpObject=QJsonDocument::fromJson(QByteArray::fromStdString(json.dump())).object();

    rowsCount=ahpObject.count()+1;
    columnsCount=ahpObject.count()+2;

    emit layoutChanged();
    emit ahpObjectChanged();
}

void AHPModel::setIndicatorName(QString indname)
{
    qDebug()<<__func__;
    indicatorName=indname;
}
QString AHPModel::getIndicatorName()
{
    qDebug()<<__func__;
    return indicatorName;
}

void AHPModel::set_AHP_values(QString name,QString key, QJsonObject vals)
{
    qDebug()<<__func__;
    if(indicatorName!=name)
        return;
    json json=json::parse( QJsonDocument(ahpObject).toJson(QJsonDocument::Compact).toStdString() );
    json[key.toStdString()]["values"]=json::parse( QJsonDocument(vals).toJson(QJsonDocument::Compact).toStdString() );
    ahpObject=QJsonDocument::fromJson(QByteArray::fromStdString(json.dump())).object();
    emit ahpObjectChanged();
}
void AHPModel::set_AHP_positive(QString name,QString key, bool pos)
{
    qDebug()<<__func__;
    if(indicatorName!=name)
        return;
    json json=json::parse( QJsonDocument(ahpObject).toJson(QJsonDocument::Compact).toStdString() );
    json[key.toStdString()]["positive"]=pos;
    ahpObject=QJsonDocument::fromJson(QByteArray::fromStdString(json.dump())).object();
    emit ahpObjectChanged();
}
