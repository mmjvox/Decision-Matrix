#include "TableModel.h"

TableModel::TableModel(){
//    table.append({"Score","شدت اثر","وضعیت سازمان","قابلیت اجرا","احتمال وقوع","Branch","ROOT"});
//    table.append({"01","02","03","04","05","06","07"});
//    table.append({"10","20","30","40","50","60","70"});
//    table.append({"11","21","31","41","51","61","71"});
//    table.append({"11","22","32","42","52","62","72"});

}
//void TableModel::callLCH(){emit layoutChanged();}

QJsonObject TableModel::getAhpObject() const
{
    qDebug()<<__func__;
    return ahpObject;
}
void TableModel::setAhpObject(QJsonObject ahp)
{
    qDebug()<<__func__;
    ahpObject=ahp;
    columnsCount=ahpObject.count()+3;
    emit layoutChanged();
    emit ahpObjectChanged();
}

QJsonObject TableModel::getClustersObject() const
{
    qDebug()<<__func__;
    return clustersObject;
}
void TableModel::setClustersObject(QJsonObject cluster)
{
    qDebug()<<__func__;
    clustersObject=cluster;
    rowsCount=clustersObject.count()+1;
    emit layoutChanged();
    emit clustersObjectChanged();
}

int TableModel::rowCount(const QModelIndex &) const
{
    qDebug()<<__func__<<"TableModel";
    return rowsCount;
}

int TableModel::columnCount(const QModelIndex &) const
{
    qDebug()<<__func__<<"TableModel";
    return columnsCount;
}

void TableModel::setSelected(QString key,int row,int columnIndex,int comboindex)
{
    qDebug()<<__func__;
    QString clusterName=clustersObject.keys().value(row-1);
    QStringList levelNames=clustersObject.value(clusterName).toObject().value("levels").toObject().keys();
    json json=json::parse( QJsonDocument(clustersObject).toJson(QJsonDocument::Compact).toStdString() );
    json[clusterName.toStdString()]["levels"][ levelNames.last().toStdString() ]["SAW"][key.toStdString()]=comboindex;
    clustersObject=QJsonDocument::fromJson(QByteArray::fromStdString(json.dump())).object();

    getMaxScore();

    emit dataChanged(index(row, columnIndex), index(row, columnIndex));
    emit dataChanged(index(row, 0), index(row, 0));
    //emit dataChanged(index(0, 100), index(0, 100));

    emit clustersObjectEdited();
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    qDebug()<<__func__<<"TableModel";
    switch (role) {
    case TableDataRole:
    {
        return cellDataMap(index.column(),index.row());
    }
        break;
    case HeadingRole:
    {
        if( index.column()>0 && index.column()<columnCount()-2 ){
            if(index.row()==0)
                return 1;
            else
                return 3;
        } else {
            if(index.row()==0)
                return 0;
            else
                return 2;
        }
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
        return expandMode.value(index.column());
    }
        break;
    case AHPkey:
    {
        return getAHPkey(index.column());
    }
        break;
    case SelectedSAW:
    {
        return getSelectedSaw(index.column(),index.row());
    }
        break;
    case SelectedSAWValue:
    {
        return getSelectedSawValue( index.column(),index.row() );
    }
        break;
    case WeightedAverage:
    {
        double max=getMaxSaw( index.column() );
        if(max==0)
            return 0;
        return fix((getSelectedSawValue( index.column(),index.row() )/max)*getWeight(index.column()),4);
    }
        break;
    case ListData:
    {
        return ahpValuesList(index.column());
    }
        break;
    case Positive:
    {
        return ahpObject.value(getAHPkey(index.column())).toObject().value("positive").toBool()?"positive":"negative";
    }
        break;
    case Weight:
    {
        return fix(getWeight(index.column()),3);
    }
        break;
    case MaxScore:
    {
        return index.row()==maxRow;
    }
        break;
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> TableModel::roleNames() const
{
    qDebug()<<__func__;
    QHash<int, QByteArray> roles;
    roles[TableDataRole] = "tabledata";
    roles[HeadingRole] = "heading";
    roles[ColumnIndex] = "columnIndex";
    roles[RowIndex] = "rowIndex";
    roles[ExpandMode] = "expandMode";
    roles[AHPkey] = "ahpKey";
    roles[SelectedSAW] = "selectedSAW";
    roles[SelectedSAWValue] = "selectedSAWValue";
    roles[WeightedAverage] = "weightedAverage";
    roles[ListData] = "listdata";
    roles[Positive] = "positive";
    roles[Weight] = "weight";
    roles[MaxScore] = "maxScore";
    return roles;
}

void TableModel::setExpandMode(int column, int mode)
{
    qDebug()<<__func__;
    expandMode[column]=mode;
    for(int row=0; row<rowsCount; row++)
        emit dataChanged(index(row, column), index(row, column));
    //emit expandModeChanged(column);
}

QStringList TableModel::ahpValuesList(int column) const
{
    qDebug()<<__func__;
    QStringList ahpNames=ahpObject.keys();
    std::reverse(ahpNames.begin(), ahpNames.end());
    QJsonObject ahpValueObjects=ahpObject.value(ahpNames.value(column-1)).toObject().value("values").toObject();
    QStringList names;
    names<<"";
    for(auto valueObject : ahpValueObjects){
        names<<valueObject.toObject().value("name").toString();
    }
    return names;
}

QVariant TableModel::cellDataMap(int column, int row) const
{
    qDebug()<<__func__;
    if(row==0)
    {
        if(column==0)
            return "Score";
        else if(column==columnsCount-1)
            return "ROOT";
        else if(column==columnsCount-2)
            return "Branch";
        else {
            QStringList ahpNames=ahpObject.keys();
            std::reverse(ahpNames.begin(), ahpNames.end());
            return ahpObject.value(ahpNames.value(column-1)).toObject().value("name");
        }
    } else {
        if(column==0)
            return fix(getScore(row),3);
        else if(column==columnsCount-1)
            return clustersObject.keys().value(row-1);
        else if(column==columnsCount-2)
        {
            QJsonObject levels=clustersObject.value(clustersObject.keys().value(row-1)).toObject().value("levels").toObject();
            QStringList levelNames=levels.keys();
            return levels.value(levelNames.last()).toObject().value("name");
        }
    }
    return QVariant("");
}

QString TableModel::getAHPkey(int column) const
{
    qDebug()<<__func__<<"TableModel";
    QStringList ahpNames=ahpObject.keys();
    std::reverse(ahpNames.begin(), ahpNames.end());
    return ahpNames.value(column-1);
}
int TableModel::getSelectedSaw(int column,int row) const
{
    qDebug()<<__func__;
    QJsonObject levels=clustersObject.value(clustersObject.keys().value(row-1)).toObject().value("levels").toObject();
    QJsonObject saw=levels.value(levels.keys().value(levels.count()-1)).toObject().value("SAW").toObject();
    QString kname=getAHPkey(column);

    if(!kname.isEmpty())
        return saw.value(kname).toInt();
    else
        return 0;
}

int TableModel::getSelectedSawValue(int column, int row) const
{
    qDebug()<<__func__;
    QJsonObject levels=clustersObject.value(clustersObject.keys().value(row-1)).toObject().value("levels").toObject();
    QJsonObject saw=levels.value(levels.keys().value(levels.count()-1)).toObject().value("SAW").toObject();
    //QStringList sawNames=saw.keys();
    //std::reverse(sawNames.begin(), sawNames.end());
    QString kname=getAHPkey(column);

    //if(!sawNames.value(column-1).isEmpty())
    if(!kname.isEmpty())
    {
        //QString kname=sawNames.value(column-1);
        QString valuesName=QString::number(saw.value(kname).toInt());
        return ahpObject.value(kname).toObject().value("values").toObject().value(valuesName).toObject().value("value").toInt();
    }
    else
        return 0;
}

double TableModel::eachRowSum(int column, int row) const
{
    qDebug()<<__func__;
    double rowSum=0;
    auto keys=ahpObject.keys();
    //std::reverse(keys.begin(),keys.end());
    for(int col=column;col>0;col--){
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

double TableModel::matrixAllSum() const
{
    qDebug()<<__func__;
    double rowSum=0,allSum=0;
    auto keys=ahpObject.keys();
    //std::reverse(keys.begin(),keys.end());
    for(int ro=keys.count();ro>0;ro--){
        for(int col=keys.count();col>0;col--){
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

double TableModel::getWeight(int column) const
{
    qDebug()<<__func__;
    auto keys=ahpObject.keys();
    if(keys.count()<column)
        return 0;

    return eachRowSum(keys.count(), keys.count()+1-column )/matrixAllSum();

}

int TableModel::getMaxSaw(int column) const
{
    qDebug()<<__func__;
    int max=0;
    for(int row=0;row<rowsCount-1;row++){
     QJsonObject levels=clustersObject.value(clustersObject.keys().value(row)).toObject().value("levels").toObject();
     QJsonObject saw=levels.value(levels.keys().value(levels.count()-1)).toObject().value("SAW").toObject();
     QString kname=getAHPkey(column);
     if(!kname.isEmpty())
     {
         QString valuesName=QString::number(saw.value(kname).toInt());
         int value = ahpObject.value(kname).toObject().value("values").toObject().value(valuesName).toObject().value("value").toInt();
         if(value>max)
             max=value;
     }
    }
    return max;
}

QString TableModel::fix(double dbl,int prec) const
{
    qDebug()<<__func__;
    QString num=QString::number(dbl,'f',prec);
    while(num.endsWith("0") || num.endsWith("."))
        num.remove(num.length()-1,1);
    return num;
}

double TableModel::getScore(int row) const
{
    qDebug()<<__func__;
    double rowSum=0;
    for(int column=ahpObject.count(); column>0 ; column-- ){
        double max=getMaxSaw( column );
        if(max!=0){
            rowSum=rowSum+(getSelectedSawValue( column , row )/max)*getWeight( column );
        }
    }
    return rowSum;
}

void TableModel::getMaxScore()
{
    qDebug()<<__func__;
    for(int row=rowsCount-1; row>0 ; row-- ){
        double rowScore=getScore(row);
        if(rowScore>maxScore){
            maxScore=rowScore;
            maxRow=row;
        }
    }

        QJsonObject levels=clustersObject.value(clustersObject.keys().value(maxRow-1)).toObject().value("levels").toObject();
        QStringList levelNames=levels.keys();
        emit exportMaxData( maxScore ,
                            clustersObject.keys().value(maxRow-1) ,
                            levelNames.last().remove("level ").toInt() ,
                            levels.value(levelNames.last()).toObject().value("name").toString() );

}
