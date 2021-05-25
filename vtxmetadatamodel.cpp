#include "vtxmetadatamodel.h"

VTXMetaDataModel::VTXMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    m_helper = new VTXHelper(path);
    m_tags << new VTXFileTagModel(m_helper);
}

VTXMetaDataModel::~VTXMetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();
    delete m_helper;
}

QList<TagModel* > VTXMetaDataModel::tags() const
{
    return m_tags;
}


VTXFileTagModel::VTXFileTagModel(VTXHelper* vtx)
    : TagModel()
{
    m_helper = vtx;
}

VTXFileTagModel::~VTXFileTagModel()
{

}

QString VTXFileTagModel::name() const
{
    return "ID3v1";
}

QList<Qmmp::MetaData> VTXFileTagModel::keys() const
{
    QList<Qmmp::MetaData> list = TagModel::keys();
    list.removeAll(Qmmp::COMPOSER);
    list.removeAll(Qmmp::ALBUMARTIST);
    list.removeAll(Qmmp::DISCNUMBER);
    list.removeAll(Qmmp::GENRE);
    list.removeAll(Qmmp::YEAR);
    return list;
}

QString VTXFileTagModel::value(Qmmp::MetaData key) const
{
    if(m_helper && m_helper->initialize())
    {
        const QMap<Qmmp::MetaData, QString> metaData(m_helper->readMetaData());
        switch((int) key)
        {
        case Qmmp::TITLE: return metaData.value(Qmmp::TITLE);
        case Qmmp::ARTIST: return metaData.value(Qmmp::ARTIST);
        case Qmmp::ALBUM: return metaData.value(Qmmp::ALBUM);
        case Qmmp::COMMENT: return metaData.value(Qmmp::COMMENT);
        case Qmmp::TRACK: return metaData.value(Qmmp::TRACK);
        }
    }
    return QString();
}

void VTXFileTagModel::setValue(Qmmp::MetaData , const QString &)
{

}
