#include "vtxhelper.h"

int ayemu_vtx_get_next_frame(vtx_info *info)
{
    int numframes = info->decoder->regdata_size / AY_FRAME_SIZE;
    if(info->vtx_pos++ >= numframes)
    {
        return 0;
    }

    unsigned char *p = info->decoder->regdata + info->vtx_pos;
    for(int n = 0 ; n < AY_FRAME_SIZE ; n++, p += numframes) 
    {
        info->regs[n] = *p;
    }
    return 1;
}

VTXHelper::VTXHelper(const QString &path)
{
    m_path = path;
    m_info = (vtx_info*)calloc(sizeof(vtx_info), 1);
    m_totalTime = 0;
}

VTXHelper::~VTXHelper()
{
    deinit();
}

void VTXHelper::deinit()
{
    if(m_info) 
    {
        if(m_info->decoder)
         {
            ayemu_vtx_free(m_info->decoder);
            m_info->decoder = nullptr;
        }
        ayemu_reset(&m_info->ay);
        free(m_info);
    }
}

bool VTXHelper::initialize()
{
    ayemu_init(&m_info->ay);
    FILE *file = stdio_open(m_path.toLocal8Bit().constData());
    if(!file)
    {
        qWarning("VTXHelper: open file failed");
        return false;
    }

    const size_t size = stdio_length(file);
    if(size <= 0)
    {
        qWarning("VTXHelper: file size invalid");
        stdio_close(file);
        return false;
    }

    char *module = (char *)malloc(size);
    if(!module)
    {
        qWarning("VTXHelper: file data read error");
        stdio_close(file);
        return false;
    }

    if(stdio_read(module, 1, size, file) != size)
    {
        qWarning("VTXHelper: file data read error");
        free(module);
        stdio_close(file);
        return false;
    }
    
    stdio_seek(file, 0, SEEK_SET);

    char header_buf[0x4000] = {0};
    size_t header_sz = stdio_read(header_buf, 1, sizeof(header_buf), file);
    ayemu_vtx_t *hdr = ayemu_vtx_header(header_buf, header_sz);
    if(!hdr)
    {
        qWarning("VTXHelper: ayemu_vtx_header error");
        free(module);
        stdio_close(file);
        return false;
    }

    m_totalTime = hdr->regdata_size * 1.0 / AY_FRAME_SIZE / hdr->playerFreq;

    m_metaData.insert(Qmmp::TITLE, hdr->title);
    m_metaData.insert(Qmmp::ARTIST, hdr->author);
    m_metaData.insert(Qmmp::ALBUM, hdr->from);
    m_metaData.insert(Qmmp::TRACK, hdr->tracker);
    m_metaData.insert(Qmmp::COMMENT, hdr->comment);

    ayemu_vtx_free(hdr);
    stdio_close(file);

    m_info->decoder = ayemu_vtx_load(module, size);
    if(!m_info->decoder)
    {
        qWarning("VTXHelper: ayemu_vtx_load error");
        free(module);
        return false;
    }
    free(module);

    ayemu_set_chip_type(&m_info->ay, m_info->decoder->chiptype, nullptr);
    ayemu_set_chip_freq(&m_info->ay, m_info->decoder->chipFreq);
    ayemu_set_stereo(&m_info->ay, (ayemu_stereo_t)m_info->decoder->stereo, nullptr);

    m_info->left = 0;
    m_info->vtx_pos = 0;
    m_info->rate = size * 8.0 / m_totalTime;

    ayemu_set_sound_format(&m_info->ay, sampleRate(), channels(), bitsPerSample());

    return true;
}

int VTXHelper::totalTime() const
{
    return m_totalTime * 1000;
}

void VTXHelper::seek(qint64 time)
{
    const int sample = time * sampleRate() / 1000;
    // get frame
    int num_frames = m_info->decoder->regdata_size / AY_FRAME_SIZE;
    int samples_per_frame = sampleRate() / m_info->decoder->playerFreq;
    // start of frame
    m_info->vtx_pos = sample / samples_per_frame;
    if(m_info->vtx_pos >= num_frames)
    {
        return; // eof
    }
    // copy register data
    int n;
    unsigned char *p = m_info->decoder->regdata + m_info->vtx_pos;
    for(n = 0 ; n < AY_FRAME_SIZE ; n++, p += num_frames)
    {
        m_info->regs[n] = *p;
    }
    // set number of bytes left in frame
    m_info->left = sampleRate() / m_info->decoder->playerFreq - (sample % samples_per_frame);
    // mul by rate to get number of bytes
    m_info->left *= m_info->rate;
    ayemu_set_regs(&m_info->ay, m_info->regs);
}

int VTXHelper::bitrate() const
{
    return m_info->rate;
}

int VTXHelper::sampleRate() const
{
    return 44100;
}

int VTXHelper::channels() const
{
    return 2;
}

int VTXHelper::bitsPerSample() const
{
    return 16;
}

int VTXHelper::read(unsigned char *buf, int size)
{
    const int initsize = size;
    int donow = 0;

    while(size > 0)
    {
        if(m_info->left > 0)
        {
            donow = (size > m_info->left) ? m_info->left : size;
            m_info->left -= donow;
            buf = (unsigned char *)ayemu_gen_sound(&m_info->ay, (char *)buf, donow);
            size -= donow;
        }
        else 
        {
            if((ayemu_vtx_get_next_frame(m_info) == 0))
            {
                break; // eof
            }
            else 
            {
                // number of samples it current frame
                m_info->left = sampleRate() / m_info->decoder->playerFreq;
                // mul by rate to get number of bytes;
                m_info->left *= m_info->rate;
                ayemu_set_regs(&m_info->ay, m_info->regs);
                donow = 0;
            }
        }
    }
    return initsize - size;
}

const QMap<Qmmp::MetaData, QString> &VTXHelper::readMetaData() const
{
    return m_metaData;
}
