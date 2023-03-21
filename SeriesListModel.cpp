#include "SeriesListModel.h"
#include "navigatorsystem.h"

SeriesListModel::SeriesListModel(QObject* parent) :
    QAbstractListModel(parent)
{

}

SeriesListModel::~SeriesListModel()
{

}

void SeriesListModel::setModelData(QVariant patientId, QVariant studyId)
{
    clearModelData();

    m_patient = patientId.toString().toStdString();
    m_study = studyId.toString().toStdString();
    std::vector<string> series = NavigatorSystem::Instance()->ListSeriesFromPatientIdStudyId(m_patient, m_study);

    beginInsertRows(QModelIndex(), m_series.size(), m_series.size());
    for(int i = 0; i < series.size(); i++)
    {
        m_series.push_back(series[i]);
    }
    endInsertRows();
}

QHash<int, QByteArray> SeriesListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PatientRole] = "patientId";
    roles[StudyRole] = "studyId";
    roles[SeriesRole] = "seriesId";

    return roles;
}

int SeriesListModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    return m_series.size();
}

QVariant SeriesListModel::data(const QModelIndex &index, int role) const
{
    //qDebug() << "PatientListModel.data called";
    if(!index.isValid())
        return QVariant();

    if(role == PatientRole)
    {
        const QString &patient = QString::fromStdString(m_patient);
        return patient;
    }
    else if(role == StudyRole)
    {
        const QString &study = QString::fromStdString(m_study);
        return study;
    }
    else if(role == SeriesRole)
    {
        const QString &series = QString::fromStdString(m_series[index.row()]);
        return series;
    }
    else
        return QVariant();
}

/*bool PatientListModel::setData(const QModelIndex& index, const QVariant &value, int role)
{
    if(data(index, role) != value)
    {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}*/

/*void StudyListModel::openFolder(QVariant path)
{
    QString qstr = path.toString();
    std::string str = qstr.toStdString();

    //Remove file name and extraneous extension on front of path
    str = str.substr(8, str.length());
    NavigatorSystem::Instance()->ListStudiesFromPatientId()
    setModelData();
}*/


void SeriesListModel::clearModelData()
{
    m_patient = "";
    m_study = "";

    beginResetModel();
    m_series.clear();
    endResetModel();
}