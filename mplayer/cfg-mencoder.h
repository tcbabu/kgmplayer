/*
 * This file is part of MPlayer.
 *
 * MPlayer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * MPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with MPlayer; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MPLAYER_CFG_MENCODER_H
#define MPLAYER_CFG_MENCODER_H

/*
 * config for cfgparser
 */

#include "libmpcodecs/ae_faac.h"
#include "libmpcodecs/ae_lame.h"
#include "libmpcodecs/ae_toolame.h"
#include "libmpcodecs/ae_twolame.h"
#include "libmpcodecs/ve.h"
#include "libmpcodecs/ve_x264.h"
#include "libmpdemux/muxer.h"
#include "libmpdemux/muxer_avi.h"
#ifdef D_KULINA
#include "cfg-common_kulina.h"
#else
#include "cfg-common.h"
#endif

const m_option_t ovc_conf[]={
    {"copy", &out_video_codec, CONF_TYPE_FLAG, 0, 0, VCODEC_COPY, NULL},
    {"frameno", &out_video_codec, CONF_TYPE_FLAG, 0, 0, VCODEC_FRAMENO, NULL},
    {"lavc", &out_video_codec, CONF_TYPE_FLAG, 0, 0, VCODEC_LIBAVCODEC, NULL},
    {"raw", &out_video_codec, CONF_TYPE_FLAG, 0, 0, VCODEC_RAW, NULL},
    {"vfw", &out_video_codec, CONF_TYPE_FLAG, 0, 0, VCODEC_VFW, NULL},
    {"libdv", &out_video_codec, CONF_TYPE_FLAG, 0, 0, VCODEC_LIBDV, NULL},
    {"xvid", &out_video_codec, CONF_TYPE_FLAG, 0, 0, VCODEC_XVID, NULL},
    {"qtvideo", &out_video_codec, CONF_TYPE_FLAG, 0, 0, VCODEC_QTVIDEO, NULL},
    {"nuv", &out_video_codec, CONF_TYPE_FLAG, 0, 0, VCODEC_NUV, NULL},
    {"x264", &out_video_codec, CONF_TYPE_FLAG, 0, 0, VCODEC_X264, NULL},
    {"help", "\nAvailable codecs:\n"
    "   copy     - frame copy, without re-encoding. Doesn't work with filters.\n"
    "   frameno  - special audio-only file for 3-pass encoding, see DOCS.\n"
    "   raw      - uncompressed video. Use fourcc option to set format explicitly.\n"
#ifdef CONFIG_LIBLZO
    "   nuv      - nuppel video\n"
#endif
#ifdef CONFIG_FFMPEG
    "   lavc     - libavcodec codecs - best quality!\n"
#endif
#ifdef CONFIG_WIN32DLL
    "   vfw      - VfW DLLs, read DOCS/HTML/en/encoding-guide.html.\n"
    "   qtvideo  - QuickTime DLLs, currently only SVQ1/3 are supported.\n"
#endif
#ifdef CONFIG_LIBDV095
    "   libdv    - DV encoding with libdv v0.9.5\n"
#endif
#ifdef CONFIG_XVID4
    "   xvid     - XviD encoding\n"
#endif
#ifdef CONFIG_X264
    "   x264     - H.264 encoding\n"
#endif
    "\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {NULL, NULL, 0, 0, 0, 0, NULL}
};

const m_option_t oac_conf[]={
    {"copy", &out_audio_codec, CONF_TYPE_FLAG, 0, 0, ACODEC_COPY, NULL},
    {"pcm", &out_audio_codec, CONF_TYPE_FLAG, 0, 0, ACODEC_PCM, NULL},
#ifdef CONFIG_MP3LAME
    {"mp3lame", &out_audio_codec, CONF_TYPE_FLAG, 0, 0, ACODEC_VBRMP3, NULL},
#else
    {"mp3lame", "MPlayer was compiled without libmp3lame support.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
#endif /* CONFIG_MP3LAME */
#ifdef CONFIG_FFMPEG
    {"lavc", &out_audio_codec, CONF_TYPE_FLAG, 0, 0, ACODEC_LAVC, NULL},
#else
    {"lavc", "MPlayer was compiled without libavcodec. See README or DOCS.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
#endif /* CONFIG_FFMPEG */
#ifdef CONFIG_TOOLAME
    {"toolame", &out_audio_codec, CONF_TYPE_FLAG, 0, 0, ACODEC_TOOLAME, NULL},
#else
    {"toolame", "MPlayer was compiled without libtoolame. See README or DOCS.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
#endif /* CONFIG_TOOLAME */
#ifdef CONFIG_TWOLAME
    {"twolame", &out_audio_codec, CONF_TYPE_FLAG, 0, 0, ACODEC_TWOLAME, NULL},
#else
    {"twolame", "MPlayer was compiled without libtwolame. See README or DOCS.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
#endif /* CONFIG_TWOLAME */
#ifdef CONFIG_FAAC
    {"faac", &out_audio_codec, CONF_TYPE_FLAG, 0, 0, ACODEC_FAAC, NULL},
#else
    {"faac", "MPlayer was compiled without libfaac. See README or DOCS.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
#endif /* CONFIG_FAAC */
    {"help", "\nAvailable codecs:\n"
    "   copy     - frame copy, without re-encoding (useful for AC3)\n"
    "   pcm      - uncompressed PCM audio\n"
#ifdef CONFIG_MP3LAME
    "   mp3lame  - cbr/abr/vbr MP3 using libmp3lame\n"
#endif
#ifdef CONFIG_FFMPEG
    "   lavc     - FFmpeg audio encoder (MP2, AC3, ...)\n"
#endif
#ifdef CONFIG_TOOLAME
    "   toolame  - Toolame MP2 audio encoder\n"
#endif
#ifdef CONFIG_TWOLAME
    "   twolame  - Twolame MP2 audio encoder\n"
#endif
#ifdef CONFIG_FAAC
    "   faac     - FAAC AAC audio encoder\n"
#endif
    "\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {NULL, NULL, 0, 0, 0, 0, NULL}
};

const m_option_t info_conf[]={
    {"name", &info_name, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"artist", &info_artist, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"genre", &info_genre, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"subject", &info_subject, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"copyright", &info_copyright, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"srcform", &info_sourceform, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"comment", &info_comment, CONF_TYPE_STRING, 0, 0, 0, NULL},
    {"help", "\nAvailable INFO fields:\n"
    "   name      - title of the work\n"
    "   artist    - artist or author of the work\n"
    "   genre     - original work category\n"
    "   subject   - contents of the work\n"
    "   copyright - copyright information\n"
    "   srcform   - original format of the digitzed material\n"
    "   comment   - general comments about the work\n"
    "\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {NULL, NULL, 0, 0, 0, 0, NULL}
};

const m_option_t of_conf[]={
    {"avi", &out_file_format, CONF_TYPE_FLAG, 0, 0, MUXER_TYPE_AVI, NULL},
    {"mpeg", &out_file_format, CONF_TYPE_FLAG, 0, 0, MUXER_TYPE_MPEG, NULL},
#ifdef CONFIG_FFMPEG
    {"lavf", &out_file_format, CONF_TYPE_FLAG, 0, 0, MUXER_TYPE_LAVF, NULL},
#endif
    {"rawvideo", &out_file_format, CONF_TYPE_FLAG, 0, 0, MUXER_TYPE_RAWVIDEO, NULL},
    {"rawaudio", &out_file_format, CONF_TYPE_FLAG, 0, 0, MUXER_TYPE_RAWAUDIO, NULL},
    {"help", "\nAvailable output formats:\n"
    "   avi      - Microsoft Audio/Video Interleaved\n"
    "   mpeg     - MPEG-1/2 system stream format\n"
#ifdef CONFIG_FFMPEG
    "   lavf     - FFmpeg libavformat muxers\n"
#endif
    "   rawvideo - (video only, one stream only) raw stream, no muxing\n"
    "   rawaudio - (audio only, one stream only) raw stream, no muxing\n"
    "\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {NULL, NULL, 0, 0, 0, 0, NULL}
};

const m_option_t mencoder_opts[]={
    /* name, pointer, type, flags, min, max */

    {"frameno-file", &frameno_filename, CONF_TYPE_STRING, CONF_GLOBAL, 0, 0, NULL},

    {"hr-edl-seek", &edl_seek_type, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"nohr-edl-seek", &edl_seek_type, CONF_TYPE_FLAG, 0, 1, 0, NULL},

    // set output framerate - recommended for variable-FPS (ASF etc) files
    // and for 29.97FPS progressive MPEG2 streams
    {"ofps", &force_ofps, CONF_TYPE_DOUBLE, CONF_MIN|CONF_GLOBAL, 0, 0, NULL},
    {"o", &out_filename, CONF_TYPE_STRING, CONF_GLOBAL, 0, 0, NULL},

    // limit number of skippable frames after a non-skipped one
    {"skiplimit", &skip_limit, CONF_TYPE_INT, 0, 0, 0, NULL},
    {"noskiplimit", &skip_limit, CONF_TYPE_FLAG, 0, 0, -1, NULL},
    {"noskip", &skip_limit, CONF_TYPE_FLAG, 0, 0, 0, NULL},

    {"audio-density", &audio_density, CONF_TYPE_INT, CONF_RANGE|CONF_GLOBAL, 1, 50, NULL},
    {"audio-preload", &audio_preload, CONF_TYPE_FLOAT, CONF_RANGE|CONF_GLOBAL, 0, 2, NULL},
    {"audio-delay",   &audio_delay_fix, CONF_TYPE_FLOAT, CONF_GLOBAL, 0, 0, NULL},

    {"x", "-x has been removed, use -vf scale=w:h for scaling.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {"xsize", "-xsize has been removed, use -vf crop=w:h:x:y for cropping.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},

    // output audio/video codec selection
    {"oac", oac_conf, CONF_TYPE_SUBCONFIG, CONF_GLOBAL, 0, 0, NULL},
    {"ovc", ovc_conf, CONF_TYPE_SUBCONFIG, CONF_GLOBAL, 0, 0, NULL},

    // output file format
    {"of", of_conf, CONF_TYPE_SUBCONFIG, CONF_GLOBAL, 0, 0, NULL},

    // override audio format tag in output file
    {"fafmttag", &force_audiofmttag, CONF_TYPE_INT, CONF_GLOBAL, 0, 0, NULL},
    // override FOURCC in output file
    {"ffourcc", &force_fourcc, CONF_TYPE_STRING, CONF_GLOBAL, 4, 4, NULL},

    // override avi aspect autodetection
    {"force-avi-aspect", &avi_aspect_override, CONF_TYPE_FLOAT, CONF_RANGE|CONF_GLOBAL, 0.2, 3.0, NULL},

    {"pass", "-pass has been removed, use -lavcopts vpass=n, -xvidencopts pass=n\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
    {"passlogfile", &passtmpfile, CONF_TYPE_STRING, CONF_GLOBAL, 0, 0, NULL},

    {"vobsubout", &vobsub_out, CONF_TYPE_STRING, CONF_GLOBAL, 0, 0, NULL},
    {"vobsuboutindex", &vobsub_out_index, CONF_TYPE_INT, CONF_RANGE|CONF_GLOBAL, 0, 31, NULL},
    {"vobsuboutid", &vobsub_out_id, CONF_TYPE_STRING, CONF_GLOBAL, 0, 0, NULL},

    {"autoexpand", &auto_expand, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"noautoexpand", &auto_expand, CONF_TYPE_FLAG, 0, 1, 0, NULL},

    {"encodedups", &encode_duplicates, CONF_TYPE_FLAG, 0, 0, 1, NULL},
    {"noencodedups", &encode_duplicates, CONF_TYPE_FLAG, 0, 1, 0, NULL},

    {"odml", &write_odml, CONF_TYPE_FLAG, CONF_GLOBAL, 0, 1, NULL},
    {"noodml", &write_odml, CONF_TYPE_FLAG, CONF_GLOBAL, 1, 0, NULL},

    // info header strings
    {"info", info_conf, CONF_TYPE_SUBCONFIG, CONF_GLOBAL, 0, 0, NULL},

    {"force-key-frames", parse_forced_key_frames, CONF_TYPE_FUNC_PARAM, CONF_GLOBAL, 0, 0, NULL},

#ifdef CONFIG_MP3LAME
    {"lameopts", lameopts_conf, CONF_TYPE_SUBCONFIG, CONF_GLOBAL, 0, 0, NULL},
#endif
#ifdef CONFIG_FFMPEG
    {"lavcopts", lavcopts_conf, CONF_TYPE_SUBCONFIG, CONF_GLOBAL, 0, 0, NULL},
#else
    {"lavcopts", "MPlayer was compiled without libavcodec. See README or DOCS.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
#endif /* CONFIG_FFMPEG */
#ifdef CONFIG_TOOLAME
    {"toolameopts", toolameopts_conf, CONF_TYPE_SUBCONFIG, CONF_GLOBAL, 0, 0, NULL},
#else
    {"toolameopts", "MPlayer was compiled without libtoolame. See README or DOCS.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
#endif /* CONFIG_TOOLAME */
#ifdef CONFIG_TWOLAME
    {"twolameopts", twolameopts_conf, CONF_TYPE_SUBCONFIG, CONF_GLOBAL, 0, 0, NULL},
#else
    {"twolameopts", "MPlayer was compiled without libtwolame. See README or DOCS.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
#endif /* CONFIG_TWOLAME */
#ifdef CONFIG_FAAC
    {"faacopts", faacopts_conf, CONF_TYPE_SUBCONFIG, CONF_GLOBAL, 0, 0, NULL},
#else
    {"faacopts", "MPlayer was compiled without libfaac. See README or DOCS.\n", CONF_TYPE_PRINT, CONF_NOCFG, 0, 0, NULL},
#endif /* CONFIG_FAAC */
#ifdef CONFIG_WIN32DLL
    {"xvfwopts", vfwopts_conf, CONF_TYPE_SUBCONFIG, CONF_GLOBAL, 0, 0, NULL},
#endif
#ifdef CONFIG_XVID4
    {"xvidencopts", xvidencopts_conf, CONF_TYPE_SUBCONFIG, CONF_GLOBAL, 0, 0, NULL},
#endif
#if defined(CONFIG_X264)
    {"x264encopts", &x264enc_set_param, CONF_TYPE_FUNC_PARAM, CONF_GLOBAL, 0, 0, NULL},
#endif

#ifdef CONFIG_LIBLZO
    {"nuvopts",  nuvopts_conf, CONF_TYPE_SUBCONFIG, CONF_GLOBAL, 0, 0, NULL},
#endif
    {"mpegopts",  mpegopts_conf, CONF_TYPE_SUBCONFIG, CONF_GLOBAL, 0, 0, NULL},
#ifdef CONFIG_FFMPEG
    {"lavfopts",  lavfopts_conf, CONF_TYPE_SUBCONFIG, CONF_GLOBAL, 0, 0, NULL},
#endif

    {NULL, NULL, 0, 0, 0, 0, NULL}
};

#endif /* MPLAYER_CFG_MENCODER_H */
