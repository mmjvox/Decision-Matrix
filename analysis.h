#ifndef CSI_ANALYSIS_H
#define CSI_ANALYSIS_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <nlohmann/json.hpp>

#include <QFile>

using json = nlohmann::json;

class Analysis : public QObject
{
    Q_OBJECT
public:
    explicit Analysis(QObject *parent = nullptr);

    QFile file;

    Q_INVOKABLE bool selectIndicator(QString indicatorName);
    Q_INVOKABLE void set_indicatorValue(QString indicatorName, int indicatorValue);
    Q_INVOKABLE void set_selectIndicator(QString indicatorName, bool checked);
    Q_INVOKABLE void get_selectIndicators();

    Q_INVOKABLE QJsonObject get_indicator(QString indicator);
    Q_INVOKABLE QVariant             get_indicatorValue(QString indicator);
    Q_INVOKABLE QJsonObject     get_cluster(QString indicator);
    Q_INVOKABLE QJsonObject         get_cluster_item(QString indicator,QString cluster);
    Q_INVOKABLE QJsonObject             get_levels(QString indicator,QString cluster);
    Q_INVOKABLE QJsonObject                 get_level(QString indicator,QString cluster,QString level);
    Q_INVOKABLE QString                         get_level_name(QString indicator,QString cluster,QString level);
    Q_INVOKABLE QJsonObject                     get_saw(QString indicator,QString cluster,QString level);
    Q_INVOKABLE QString                             get_saw_value(QString indicator,QString cluster,QString level,QString sawKey);
    Q_INVOKABLE QJsonObject         get_AHP(QString indicator);
    Q_INVOKABLE QJsonObject             get_AHP_item(QString indicator, QString ahpKey);
    Q_INVOKABLE bool                        get_AHP_item_positive(QString indicator, QString ahpKey);
    Q_INVOKABLE QString                     get_AHP_item_label(QString indicator, QString ahpKey);
    Q_INVOKABLE QJsonObject                 get_AHP_item_values(QString indicator, QString ahpKey);
    Q_INVOKABLE QJsonObject                     get_AHP_item_value(QString indicator, QString ahpKey, QString valueNum);
    Q_INVOKABLE QString                             get_AHP_item_value_detail(QString indicator, QString ahpKey, QString valueNum, QString detailKey);
    Q_INVOKABLE QJsonObject         get_Corrections(QString indicator);
    Q_INVOKABLE QString get_Correction_detail(QString indicator, QString key);
    Q_INVOKABLE QJsonArray                 get_Correction_values(QString indicator);

    Q_INVOKABLE void set_Level(QString indic, QString cluster, QString levelName, int levelIndex);
    Q_INVOKABLE void clear_Levels(QString indic, QString cluster);
    Q_INVOKABLE void set_AHP(QString indicator,QJsonObject ahp);
    Q_INVOKABLE void set_Clusters(QString indicator,QJsonObject clusters);
    Q_INVOKABLE void set_Correction(QString indicator, double score, QString clusterName, int level, QString levelName);
    Q_INVOKABLE void set_Correction_values(QString indicator, QJsonObject values);


    Q_INVOKABLE QString getPriorityColor(QString indicator);

    void saveFile();

signals:
    void appendSelectedInds(QString ind);

public slots:


private:
    QJsonObject mainAnalysis;
    QStringList selectedIndicators;
};

#endif // CSI_ANALYSIS_H
