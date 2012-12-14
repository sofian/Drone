/* 
 * Sierra SOL decoder
 * Copyright Konstantin Shishkov.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* 
 * Based on documents from Game Audio Player and own research
 */

#include "avformat.h"
#include "avi.h"
#include "bswap.h"

/* if we don't know the size in advance */
#define AU_UNKOWN_SIZE ((uint32_t)(~0))

static int sol_probe(AVProbeData *p)
{
    /* check file header */
    uint16_t magic;
    if (p->buf_size <= 14)
        return 0;
    magic=le2me_16(*((uint16_t*)p->buf));
    if ((magic == 0x0B8D || magic == 0x0C0D || magic == 0x0C8D) &&
        p->buf[2] == 'S' && p->buf[3] == 'O' &&
        p->buf[4] == 'L' && p->buf[5] == 0)
        return AVPROBE_SCORE_MAX;
    else
        return 0;
}

#define SOL_DPCM    1
#define SOL_16BIT   4
#define SOL_STEREO 16

static int sol_codec_id(int magic, int type)
{
    if (magic == 0x0B8D)
    {
        if (type & SOL_DPCM) return CODEC_ID_SOL_DPCM;
        else return CODEC_ID_PCM_U8;
    }
    if (type & SOL_DPCM)
    {
        if (type & SOL_16BIT) return CODEC_ID_SOL_DPCM;
        else if (magic == 0x0C8D) return CODEC_ID_SOL_DPCM;
        else return CODEC_ID_SOL_DPCM;
    }
    if (type & SOL_16BIT) return CODEC_ID_PCM_S16LE;
    return CODEC_ID_PCM_U8;
}

static int sol_codec_type(int magic, int type)
{
    if (magic == 0x0B8D) return 1;//SOL_DPCM_OLD;
    if (type & SOL_DPCM)
    {
        if (type & SOL_16BIT) return 3;//SOL_DPCM_NEW16;
        else if (magic == 0x0C8D) return 1;//SOL_DPCM_OLD;
        else return 2;//SOL_DPCM_NEW8;
    }
    return -1;
}

static int sol_channels(int magic, int type)
{
    if (magic == 0x0B8D || !(type & SOL_STEREO)) return 1;
    return 2;
}
    
static int sol_read_header(AVFormatContext *s,
                          AVFormatParameters *ap)
{
    int size;
    unsigned int magic,tag;
    ByteIOContext *pb = &s->pb;
    unsigned int id, codec, channels, rate, type;
    AVStream *st;

    /* check ".snd" header */
    magic = get_le16(pb);
    tag = get_le32(pb);
    if (tag != MKTAG('S', 'O', 'L', 0))
        return -1;
    rate = get_le16(pb);
    type = get_byte(pb);
    size = get_le32(pb);
    if (magic != 0x0B8D)
        get_byte(pb); /* newer SOLs contain padding byte */
    
    codec = sol_codec_id(magic, type);
    channels = sol_channels(magic, type);
    
    if (codec == CODEC_ID_SOL_DPCM)
        id = sol_codec_type(magic, type);
    else id = 0;
    
    /* now we are ready: build format streams */
    st = av_new_stream(s, 0);
    if (!st)
        return -1;
    st->codec.codec_type = CODEC_TYPE_AUDIO;
    st->codec.codec_tag = id;
    st->codec.codec_id = codec;
    st->codec.channels = channels;
    st->codec.sample_rate = rate;    
    av_set_pts_info(st, 64, 1, rate);
    return 0;
}

#define MAX_SIZE 4096

static int sol_read_packet(AVFormatContext *s,
                          AVPacket *pkt)
{
    int ret;

    if (url_feof(&s->pb))
        return -EIO;
    if (av_new_packet(pkt, MAX_SIZE))
        return -EIO;
    pkt->stream_index = 0;

    ret = get_buffer(&s->pb, pkt->data, pkt->size);
    if (ret < 0)
        av_free_packet(pkt);
    /* note: we need to modify the packet size here to handle the last
       packet */
    pkt->size = ret;
    return 0;
}

static int sol_read_close(AVFormatContext *s)
{
    return 0;
}

static AVInputFormat sol_iformat = {
    "sol",
    "Sierra SOL Format",
    0,
    sol_probe,
    sol_read_header,
    sol_read_packet,
    sol_read_close,
    pcm_read_seek,
};

int sol_init(void)
{
    av_register_input_format(&sol_iformat);
    return 0;
}