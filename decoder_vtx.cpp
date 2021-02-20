#include "vtxhelper.h"
#include "decoder_vtx.h"

DecoderVTX::DecoderVTX(const QString &path)
    : Decoder()
{
    m_vtx = new VTXHelper(path);
}

DecoderVTX::~DecoderVTX()
{
    delete m_vtx;
}

bool DecoderVTX::initialize()
{
    if(!m_vtx->initialize())
    {
        qWarning("DecoderVTX: initialize failed");
        return false;
    }

    const int rate = m_vtx->sampleRate();
    const int channels = m_vtx->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderVTX: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderVTX::totalTime() const
{
    return m_vtx->totalTime();
}

int DecoderVTX::bitrate() const
{
    return m_vtx->bitrate();
}

qint64 DecoderVTX::read(unsigned char *data, qint64 size)
{
    return m_vtx->read(data, size);
}

void DecoderVTX::seek(qint64 pos)
{
    m_vtx->seek(pos);
}
