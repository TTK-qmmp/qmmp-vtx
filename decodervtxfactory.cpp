#include "vtxhelper.h"
#include "decoder_vtx.h"
#include "decodervtxfactory.h"
#include "vtxmetadatamodel.h"

#include <QMessageBox>

bool DecoderVTXFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderVTXFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("VTX Plugin");
    properties.filters << "*.vtx";
    properties.description = tr("AY/YM Audio Files");
    properties.shortName = "vtx";
    properties.noInput = true;
    properties.protocols << "vtx";
    return properties;
}

Decoder *DecoderVTXFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderVTX(path);
}

QList<TrackInfo*> DecoderVTXFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    VTXHelper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::MetaData)
    {
        helper.readMetaTags();
        info->setValue(Qmmp::ALBUM, helper.album());
        info->setValue(Qmmp::ARTIST, helper.artist());
        info->setValue(Qmmp::COMMENT, helper.comment());
        info->setValue(Qmmp::TITLE, helper.title());
        info->setValue(Qmmp::TRACK, helper.tracker());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.bitsPerSample());
        info->setValue(Qmmp::FORMAT_NAME, "vtx");
        info->setDuration(helper.totalTime());
    }

    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderVTXFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new VTXMetaDataModel(path);
}

void DecoderVTXFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

void DecoderVTXFactory::showAbout(QWidget *parent)
{
    QMessageBox::about (parent, tr("About VTX Reader Plugin"),
                        tr("Qmmp VTX Reader Plugin")+"\n"+
                        tr("Written by: Greedysky <greedysky@163.com>"));
}

QString DecoderVTXFactory::translation() const
{
    return QString();
}
