#include "vtxmetadatamodel.h"

VTXMetaDataModel::VTXMetaDataModel(const QString &path)
    : MetaDataModel(true)
{
    m_vtx = new VTXHelper(path);
    m_tags << new VTXFileTagModel(m_vtx);
}

VTXMetaDataModel::~VTXMetaDataModel()
{
    while(!m_tags.isEmpty())
        delete m_tags.takeFirst();
    delete m_vtx;
}

QList<TagModel* > VTXMetaDataModel::tags() const
{
    return m_tags;
}


VTXFileTagModel::VTXFileTagModel(VTXHelper* vtx)
    : TagModel()
{
    m_vtx = vtx;
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
    if(m_vtx && m_vtx->initialize())
    {
        m_vtx->readMetaTags();
        switch((int) key)
        {
        case Qmmp::TITLE: return m_vtx->title();
        case Qmmp::ARTIST: return m_vtx->artist();
        case Qmmp::ALBUM: return m_vtx->album();
        case Qmmp::COMMENT: return m_vtx->comment();
        case Qmmp::TRACK: return m_vtx->tracker();
        }
    }
    return QString();
}

void VTXFileTagModel::setValue(Qmmp::MetaData , const QString &)
{

}
