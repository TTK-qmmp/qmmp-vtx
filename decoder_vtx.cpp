#include "vtxhelper.h"
#include "decoder_vtx.h"

DecoderVTX::DecoderVTX(const QString &path)
    : Decoder()
{
    m_helper = new VTXHelper(path);
}

DecoderVTX::~DecoderVTX()
{
    delete m_helper;
}

bool DecoderVTX::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderVTX: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderVTX: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderVTX: initialize succes");
    return true;
}

qint64 DecoderVTX::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderVTX::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderVTX::read(unsigned char *data, qint64 size)
{
    return m_helper->read(data, size);
}

void DecoderVTX::seek(qint64 pos)
{
    m_helper->seek(pos);
}
