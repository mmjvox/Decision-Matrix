#include "analysis.h"

Analysis::Analysis(QObject *parent) : QObject(parent)
{
    qDebug()<<__func__;
    file.setFileName(":/struct.json");
    file.open(QIODevice::ReadOnly);
    QJsonDocument doc=QJsonDocument::fromJson(file.readAll());
    mainAnalysis=doc.object();
    file.close();
    /////
    selectedIndicators<<"Quality of repairs"<<"Description of repairs";
}

void Analysis::saveFile()
{
    qDebug()<<__func__;
    file.remove();
    file.open(QIODevice::WriteOnly);
    file.write(QJsonDocument(mainAnalysis).toJson(QJsonDocument::Compact));
    file.close();
}


void Analysis::set_indicatorValue(QString indicatorName,int indicatorValue)
{
    qDebug()<<__func__;
    json json=json::parse( QJsonDocument(mainAnalysis).toJson(QJsonDocument::Compact).toStdString() );
    json[indicatorName.toStdString()]["indicatorValue"]=indicatorValue;
    mainAnalysis=QJsonDocument::fromJson(QByteArray::fromStdString(json.dump())).object();

    saveFile();
}

void Analysis::set_Level(QString indic, QString cluster, QString levelName,int levelIndex)
{
    qDebug()<<__func__;
    if(indic.isEmpty() || cluster.isEmpty() || levelName.isEmpty() || levelIndex<1)
        return;

    json json=json::parse( QJsonDocument(mainAnalysis).toJson(QJsonDocument::Compact).toStdString() );
    json[indic.toStdString()]["clusters"][cluster.toStdString()]["levels"]["level "+ std::to_string(levelIndex) ]["name"]=levelName.toStdString();
    mainAnalysis=QJsonDocument::fromJson(QByteArray::fromStdString(json.dump())).object();

    saveFile();
}
void Analysis::clear_Levels(QString indic, QString cluster)
{
    qDebug()<<__func__;
    if(indic.isEmpty() || cluster.isEmpty())
        return;

    json json=json::parse( QJsonDocument(mainAnalysis).toJson(QJsonDocument::Compact).toStdString() );
    json[indic.toStdString()]["clusters"][cluster.toStdString()]["levels"]={};
    mainAnalysis=QJsonDocument::fromJson(QByteArray::fromStdString(json.dump())).object();

    saveFile();
}

void Analysis::set_selectIndicator(QString indicatorName,bool checked)
{
    qDebug()<<__func__;
    if(checked){
        if(!selectedIndicators.contains(indicatorName))
            selectedIndicators.append(indicatorName);
    } else {
        selectedIndicators.removeOne(indicatorName);
    }
    get_selectIndicators();
}

bool Analysis::selectIndicator(QString indicatorName)
{
    qDebug()<<__func__;
    return selectedIndicators.contains(indicatorName);
}

void Analysis::get_selectIndicators()
{
    qDebug()<<__func__;
    //prevent crash when append while in loop
    auto list=selectedIndicators;
    for(QString indicator : list)
        emit appendSelectedInds(indicator);
}

QJsonObject Analysis::get_indicator(QString indicator)
{
    qDebug()<<__func__;
    if(indicator.isEmpty())
        return QJsonObject();
    return mainAnalysis.value(indicator).toObject();
}
QVariant Analysis::get_indicatorValue(QString indicator)
{
    qDebug()<<__func__;
    if(indicator.isEmpty())
        return "";

    if(!get_indicator(indicator).contains("indicatorValue"))
        return "";

    return QString::number(get_indicator(indicator).value("indicatorValue").toInt());
}
QJsonObject Analysis::get_cluster(QString indicator)
{
    qDebug()<<__func__;
    return get_indicator(indicator).value("clusters").toObject();
}
QJsonObject Analysis::get_cluster_item(QString indicator,QString cluster)
{
    qDebug()<<__func__;
    if(cluster.isEmpty())
        return QJsonObject();
    return get_indicator(indicator).value("clusters").toObject().value(cluster).toObject();
}
QJsonObject Analysis::get_levels(QString indicator,QString cluster)
{
    qDebug()<<__func__;
    return get_cluster_item(indicator,cluster).value("levels").toObject();
}
QJsonObject Analysis::get_level(QString indicator,QString cluster,QString level)
{
    qDebug()<<__func__;
    if(level.isEmpty())
        return QJsonObject();
    return get_levels(indicator,cluster).value(level).toObject();
}
QString Analysis::get_level_name(QString indicator,QString cluster,QString level)
{
    qDebug()<<__func__;
    return get_level(indicator,cluster,level).value("name").toString();
}
QJsonObject Analysis::get_saw(QString indicator,QString cluster,QString level)
{
    qDebug()<<__func__;
    return get_level(indicator,cluster,level).value("SAW").toObject();
}
QString Analysis::get_saw_value(QString indicator,QString cluster,QString level,QString sawKey)
{
    qDebug()<<__func__;
    return get_level(indicator,cluster,level).value("SAW").toObject().value(sawKey).toString();
}
QJsonObject Analysis::get_AHP(QString indicator)
{
    qDebug()<<__func__;
    if(indicator.isEmpty())
        return QJsonObject();
    return get_indicator(indicator).value("AHP").toObject();
}
QJsonObject Analysis::get_AHP_item(QString indicator,QString ahpKey)
{
    qDebug()<<__func__;
    return get_AHP(indicator).value(ahpKey).toObject();
}
bool Analysis::get_AHP_item_positive(QString indicator,QString ahpKey)
{
    qDebug()<<__func__;
    return get_AHP_item(indicator,ahpKey).value("positive").toBool();
}
QString Analysis::get_AHP_item_label(QString indicator,QString ahpKey)
{
    qDebug()<<__func__;
    return get_AHP_item(indicator,ahpKey).value("label").toString();
}
QJsonObject Analysis::get_AHP_item_values(QString indicator,QString ahpKey)
{
    qDebug()<<__func__;
    return get_AHP_item(indicator,ahpKey).value("values").toObject();
}
QJsonObject Analysis::get_AHP_item_value(QString indicator,QString ahpKey,QString valueNum)
{
    qDebug()<<__func__;
    return get_AHP_item_values(indicator,ahpKey).value(valueNum).toObject();
}
QString Analysis::get_AHP_item_value_detail(QString indicator,QString ahpKey,QString valueNum,QString detailKey)
{
    qDebug()<<__func__;
    return get_AHP_item_value(indicator,ahpKey,valueNum).value(detailKey).toString();
}
QJsonObject Analysis::get_Corrections(QString indicator)
{
    qDebug()<<__func__;
    if(indicator.isEmpty())
        return QJsonObject();
    return get_indicator(indicator).value("corrections").toObject();
}
QString Analysis::get_Correction_detail(QString indicator,QString key)
{
    qDebug()<<__func__;
    return get_Corrections(indicator).value(key).toString();
}
QJsonArray Analysis::get_Correction_values(QString indicator)
{
    qDebug()<<__func__;
    QJsonObject values=get_Corrections(indicator).value("values").toObject();
    QJsonArray valuesArray;
    for(QJsonValue val : values)
    {
        valuesArray.append(val);
    }
    return valuesArray;
}

void Analysis::set_AHP(QString indicator,QJsonObject ahp)
{
    qDebug()<<__func__;
    json json=json::parse( QJsonDocument(mainAnalysis).toJson(QJsonDocument::Compact).toStdString() );
    json[indicator.toStdString()]["AHP"]=json::parse( QJsonDocument(ahp).toJson(QJsonDocument::Compact).toStdString() );;
    mainAnalysis=QJsonDocument::fromJson(QByteArray::fromStdString(json.dump())).object();

    saveFile();
}

void Analysis::set_Clusters(QString indicator, QJsonObject clusters)
{
    qDebug()<<__func__;
    json json=json::parse( QJsonDocument(mainAnalysis).toJson(QJsonDocument::Compact).toStdString() );
    json[indicator.toStdString()]["clusters"]=json::parse( QJsonDocument(clusters).toJson(QJsonDocument::Compact).toStdString() );
    mainAnalysis=QJsonDocument::fromJson(QByteArray::fromStdString(json.dump())).object();

    saveFile();
}

void Analysis::set_Correction(QString indicator,double score, QString clusterName, int level, QString levelName)
{
    qDebug()<<__func__;
    json json=json::parse( QJsonDocument(mainAnalysis).toJson(QJsonDocument::Compact).toStdString() );
    json[indicator.toStdString()]["corrections"]={};
    json[indicator.toStdString()]["corrections"]["score"]=score;
    json[indicator.toStdString()]["corrections"]["clusterName"]=clusterName.toStdString();
    json[indicator.toStdString()]["corrections"]["level"]=level;
    json[indicator.toStdString()]["corrections"]["levelName"]=levelName.toStdString();
    json[indicator.toStdString()]["corrections"]["values"]={};
    mainAnalysis=QJsonDocument::fromJson(QByteArray::fromStdString(json.dump())).object();

    saveFile();
}
void Analysis::set_Correction_values(QString indicator,QJsonObject values)
{
    qDebug()<<__func__;
    json json=json::parse( QJsonDocument(mainAnalysis).toJson(QJsonDocument::Compact).toStdString() );
    json[indicator.toStdString()]["corrections"]["values"]={};
    json[indicator.toStdString()]["corrections"]["values"]=json::parse( QJsonDocument(values).toJson(QJsonDocument::Compact).toStdString() );
    mainAnalysis=QJsonDocument::fromJson(QByteArray::fromStdString(json.dump())).object();

    saveFile();
}

QString Analysis::getPriorityColor(QString indicator)
{
    qDebug()<<__func__;
    QStringList neededIndicators;
    neededIndicators<<"تامین به موقع قطعات توسط نمایندگی"
                      <<"مدت زمان تعمیر و تحویل"
                        <<"زمان صرف شده جهت پذیرش"
                          <<"نوبت دهی"
                            <<"ثبت ایرادات مطرح شده"
                              <<"کیفیت تعمیرات"
                                <<"کیفیت سرویس ادواری"
                                  <<"هزینه پرداختی"
                                    <<"Description of repairs"
                                      <<"رفتار و برخورد پرسنل";
    double current=get_indicatorValue(indicator).toDouble();


    QList<int> valueList;

    for(QString indicator : mainAnalysis.keys())
        if(neededIndicators.contains(indicator))
            valueList.append(mainAnalysis.value(indicator).toObject().value("indicatorValue").toInt());

    std::sort(valueList.begin(),valueList.end());

    double m=static_cast<double>(valueList.last()-valueList.first())/4;

    double l0=static_cast<double>(valueList.first());
    double l1=l0+m;
    double l2=l1+m;
    double l3=l2+m;
    double l4=l3+m;

    if(current>=l0 && current<=l1)
        return "#99F44336";//red
    else if(current>l1 && current<=l2)
        return "#99FF9800";//orange
    else if(current>l2 && current<=l3)
        return "#994CAF50";//green
    else if(current>l3 && current<=l4)
        return "#99FFEB3B";//yellow

    return "#ffffff";
}
