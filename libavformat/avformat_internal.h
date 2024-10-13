/*
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/*
 * APIs internal to the generic avformat layer.
 *
 * MUST NOT be included by individual muxers or demuxers.
 */

#ifndef AVFORMAT_AVFORMAT_INTERNAL_H
#define AVFORMAT_AVFORMAT_INTERNAL_H

#include <stdint.h>

#include "avformat.h"

#define RELATIVE_TS_BASE (INT64_MAX - (1LL << 48))

static av_always_inline int is_relative(int64_t ts)
{
    return ts > (RELATIVE_TS_BASE - (1LL << 48));
}

/**
 * Wrap a given time stamp, if there is an indication for an overflow
 *
 * @param st stream
 * @param timestamp the time stamp to wrap
 * @return resulting time stamp
 */
int64_t ff_wrap_timestamp(const AVStream *st, int64_t timestamp);

typedef struct FFStreamGroup {
    /**
     * The public context.
     */
    AVStreamGroup pub;

    AVFormatContext *fmtctx;
} FFStreamGroup;

static av_always_inline FFStreamGroup *ffstreamgroup(AVStreamGroup *stg)
{
    return (FFStreamGroup*)stg;
}

static av_always_inline const FFStreamGroup *cffstreamgroup(const AVStreamGroup *stg)
{
    return (const FFStreamGroup*)stg;
}

void ff_flush_packet_queue(AVFormatContext *s);

const struct AVCodec *ff_find_decoder(AVFormatContext *s, const AVStream *st,
                                      enum AVCodecID codec_id);

/**
 * Frees a stream without modifying the corresponding AVFormatContext.
 * Must only be called if the latter doesn't matter or if the stream
 * is not yet attached to an AVFormatContext.
 */
void ff_free_stream(AVStream **st);

/**
 * Frees a stream group without modifying the corresponding AVFormatContext.
 * Must only be called if the latter doesn't matter or if the stream
 * is not yet attached to an AVFormatContext.
 */
void ff_free_stream_group(AVStreamGroup **pstg);

int ff_is_intra_only(enum AVCodecID id);

struct FFOutputFormat;
struct FFInputFormat;
void avpriv_register_devices(const struct FFOutputFormat * const o[],
                             const struct FFInputFormat * const i[]);

#endif // AVFORMAT_AVFORMAT_INTERNAL_H
