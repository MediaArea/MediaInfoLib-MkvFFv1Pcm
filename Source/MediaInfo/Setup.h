/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// All compilation definitions
// Helpers for compilers (precompilation)
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_SetupH
#define MediaInfo_SetupH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//Needed in the whole library
#include "ZenLib/Conf.h"

//***************************************************************************
// General configuration
//***************************************************************************

//---------------------------------------------------------------------------
// Legacy
#if defined(MEDIAINFO_MINIMIZESIZE)
    #if !defined (MEDIAINFO_TRACE_NO) && !defined (MEDIAINFO_TRACE_YES)
        #define MEDIAINFO_TRACE_NO
    #endif
    //#if !defined (MEDIAINFO_FILTER_NO) && !defined (MEDIAINFO_FILTER_YES)
    //    #define MEDIAINFO_FILTER_NO
    //#endif
    //#if !defined (MEDIAINFO_DUPLICATE_NO) && !defined (MEDIAINFO_DUPLICATE_YES)
    //    #define MEDIAINFO_DUPLICATE_NO
    //#endif
    #if !defined (MEDIAINFO_MACROBLOCKS_NO) && !defined (MEDIAINFO_MACROBLOCKS_YES)
        #define MEDIAINFO_MACROBLOCKS_NO
    #endif
    #if !defined (MEDIAINFO_NEXTPACKET_NO) && !defined (MEDIAINFO_NEXTPACKET_YES)
        #define MEDIAINFO_NEXTPACKET_NO
    #endif
    #if !defined (MEDIAINFO_SEEK_NO) && !defined (MEDIAINFO_SEEK_YES)
        #define MEDIAINFO_SEEK_NO
    #endif
    #if !defined (MEDIAINFO_EVENTS_NO) && !defined (MEDIAINFO_EVENTS_YES)
        #define MEDIAINFO_EVENTS_NO
    #endif
    #if !defined (MEDIAINFO_DEMUX_NO) && !defined (MEDIAINFO_DEMUX_YES)
        #define MEDIAINFO_DEMUX_NO
    #endif
    #if !defined (MEDIAINFO_IBI_NO) && !defined (MEDIAINFO_IBI_YES)
        #define MEDIAINFO_IBI_NO
    #endif
#endif
#if defined(MEDIAINFO_EVENTS)
    #undef MEDIAINFO_EVENTS
    #if !defined (MEDIAINFO_EVENTS_NO) && !defined (MEDIAINFO_EVENTS_YES)
        #define MEDIAINFO_EVENTS_YES
    #endif
#endif

//---------------------------------------------------------------------------
// Special configurations
#if defined(MEDIAINFO_MINIMAL_YES)
    #if !defined (MEDIAINFO_TRACE_NO) && !defined (MEDIAINFO_TRACE_YES)
        #define MEDIAINFO_TRACE_NO
    #endif
    #if !defined (MEDIAINFO_FILTER_NO) && !defined (MEDIAINFO_FILTER_YES)
        #define MEDIAINFO_FILTER_NO
    #endif
    #if !defined (MEDIAINFO_DUPLICATE_NO) && !defined (MEDIAINFO_DUPLICATE_YES)
        #define MEDIAINFO_DUPLICATE_NO
    #endif
    #if !defined (MEDIAINFO_MACROBLOCKS_NO) && !defined (MEDIAINFO_MACROBLOCKS_YES)
        #define MEDIAINFO_MACROBLOCKS_NO
    #endif
    #if !defined (MEDIAINFO_NEXTPACKET_NO) && !defined (MEDIAINFO_NEXTPACKET_YES)
        #define MEDIAINFO_NEXTPACKET_NO
    #endif
    #if !defined (MEDIAINFO_SEEK_NO) && !defined (MEDIAINFO_SEEK_YES)
        #define MEDIAINFO_SEEK_NO
    #endif
    #if !defined (MEDIAINFO_EVENTS_NO) && !defined (MEDIAINFO_EVENTS_YES)
        #define MEDIAINFO_EVENTS_NO
    #endif
    #if !defined (MEDIAINFO_DEMUX_NO) && !defined (MEDIAINFO_DEMUX_YES)
        #define MEDIAINFO_DEMUX_NO
    #endif
    #if !defined (MEDIAINFO_IBI_NO) && !defined (MEDIAINFO_IBI_YES)
        #define MEDIAINFO_IBI_NO
    #endif
    #if !defined (MEDIAINFO_DIRECTORY_NO) && !defined (MEDIAINFO_DIRECTORY_YES)
        #define MEDIAINFO_DIRECTORY_NO
    #endif
    #if !defined (MEDIAINFO_LIBCURL_NO) && !defined (MEDIAINFO_LIBCURL_YES)
        #define MEDIAINFO_LIBCURL_NO
    #endif
    #if !defined (MEDIAINFO_LIBMMS_NO) && !defined (MEDIAINFO_LIBMM_YES)
        #define MEDIAINFO_LIBMMS_NO
    #endif
    #if !defined (MEDIAINFO_DVDIF_ANALYZE_NO) && !defined (MEDIAINFO_DVDIF_ANALYZE_YES)
        #define MEDIAINFO_DVDIF_ANALYZE_NO
    #endif
    #if !defined (MEDIAINFO_MPEGTS_DUPLICATE_NO) && !defined (MEDIAINFO_MPEGTS_DUPLICATE_YES)
        #define MEDIAINFO_MPEGTS_DUPLICATE_NO
    #endif
    #if !defined (MEDIAINFO_READTHREAD_NO) && !defined (MEDIAINFO_READTHREAD_YES)
        #define MEDIAINFO_READTHREAD_NO
    #endif
    #if !defined (MEDIAINFO_MD5_NO) && !defined (MEDIAINFO_MD5_YES)
        #define MEDIAINFO_MD5_NO
    #endif
    #if !defined (MEDIAINFO_SHA1_NO) && !defined (MEDIAINFO_SHA1_YES)
        #define MEDIAINFO_SHA1_NO
    #endif
    #if !defined (MEDIAINFO_SHA2_NO) && !defined (MEDIAINFO_SHA2_YES)
        #define MEDIAINFO_SHA2_NO
    #endif
    #if !defined (MEDIAINFO_AES_NO) && !defined (MEDIAINFO_AES_YES)
        #define MEDIAINFO_AES_NO
    #endif
    #if !defined (MEDIAINFO_EXPORT_NO) && !defined (MEDIAINFO_EXPORT_YES)
        #define MEDIAINFO_EXPORT_NO
    #endif
#endif

//---------------------------------------------------------------------------
// Optional features
#if !defined(MEDIAINFO_TRACE)
    #if defined(MEDIAINFO_TRACE_NO) && defined(MEDIAINFO_TRACE_YES)
        #undef MEDIAINFO_TRACE_NO //MEDIAINFO_TRACE_YES has priority
    #endif
    #if defined(MEDIAINFO_TRACE_NO)
        #define MEDIAINFO_TRACE 0
    #else
        #define MEDIAINFO_TRACE 1
    #endif
#endif
#if !defined(MEDIAINFO_TRACE_FFV1CONTENT)
    #if defined(MEDIAINFO_TRACE_FFV1CONTENT_NO) && defined(MEDIAINFO_TRACE_FFV1CONTENT_YES)
        #undef MEDIAINFO_TRACE_FFV1CONTENT_NO //MEDIAINFO_TRACE_FFV1CONTENT_YES has priority
    #endif
    #if defined(MEDIAINFO_TRACE_FFV1CONTENT_YES)
        #define MEDIAINFO_TRACE_FFV1CONTENT 1
    #else
        #define MEDIAINFO_TRACE_FFV1CONTENT 0
    #endif
#endif
#if MEDIAINFO_TRACE_FFV1CONTENT && !MEDIAINFO_TRACE
    #define MEDIAINFO_TRACE_FFV1CONTENT 0 //No trace if MEDIAINFO_TRACE is 0
#endif
#if !defined(MEDIAINFO_FILTER)
    #if defined(MEDIAINFO_FILTER_NO) && defined(MEDIAINFO_FILTER_YES)
        #undef MEDIAINFO_FILTER_NO //MEDIAINFO_FILTER_YES has priority
    #endif
    #if defined(MEDIAINFO_FILTER_NO)
        #define MEDIAINFO_FILTER 0
    #else
        #define MEDIAINFO_FILTER 1
    #endif
#endif
#if !defined(MEDIAINFO_DUPLICATE)
    #if defined(MEDIAINFO_DUPLICATE_NO) && defined(MEDIAINFO_DUPLICATE_YES)
        #undef MEDIAINFO_DUPLICATE_NO //MEDIAINFO_DUPLICATE_YES has priority
    #endif
    #if defined(MEDIAINFO_DUPLICATE_NO)
        #define MEDIAINFO_DUPLICATE 0
    #else
        #define MEDIAINFO_DUPLICATE 1
    #endif
#endif
#if !defined(MEDIAINFO_MACROBLOCKS)
    #if defined(MEDIAINFO_MACROBLOCKS_NO) && defined(MEDIAINFO_MACROBLOCKS_YES)
        #undef MEDIAINFO_MACROBLOCKS_NO //MEDIAINFO_MACROBLOCKS_YES has priority
    #endif
    #if defined(MEDIAINFO_MACROBLOCKS_NO)
        #define MEDIAINFO_MACROBLOCKS 0
    #else
        #define MEDIAINFO_MACROBLOCKS 1
    #endif
#endif
#if !defined(MEDIAINFO_AES)
    #if defined(MEDIAINFO_AES_NO) && defined(MEDIAINFO_AES_YES)
        #undef MEDIAINFO_AES_NO //MEDIAINFO_AES_YES has priority
    #endif
    #if defined(MEDIAINFO_AES_NO)
        #define MEDIAINFO_AES 0
    #else
        #define MEDIAINFO_AES 1
    #endif
#endif
#if !defined(MEDIAINFO_NEXTPACKET)
    #if defined(MEDIAINFO_NEXTPACKET_NO) && defined(MEDIAINFO_NEXTPACKET_YES)
        #undef MEDIAINFO_NEXTPACKET_NO //MEDIAINFO_NEXTPACKET_YES has priority
    #endif
    #if defined(MEDIAINFO_NEXTPACKET_NO)
        #define MEDIAINFO_NEXTPACKET 0
    #else
        #define MEDIAINFO_NEXTPACKET 1
    #endif
#endif
#if !defined(MEDIAINFO_SEEK)
    #if defined(MEDIAINFO_SEEK_NO) && defined(MEDIAINFO_SEEK_YES)
        #undef MEDIAINFO_SEEK_NO //MEDIAINFO_SEEK_YES has priority
    #endif
    #if defined(MEDIAINFO_SEEK_NO)
        #define MEDIAINFO_SEEK 0
    #else
        #define MEDIAINFO_SEEK 1
    #endif
#endif
#if !defined(MEDIAINFO_EVENTS)
    #if defined(MEDIAINFO_EVENTS_NO) && defined(MEDIAINFO_EVENTS_YES)
        #undef MEDIAINFO_EVENTS_NO //MEDIAINFO_EVENTS_YES has priority
    #endif
    #if defined(MEDIAINFO_EVENTS_NO)
        #define MEDIAINFO_EVENTS 0
    #else
        #define MEDIAINFO_EVENTS 1
    #endif
#endif
#if !defined(MEDIAINFO_ADVANCED)
    #if defined(MEDIAINFO_ADVANCED_NO) && defined(MEDIAINFO_ADVANCED_YES)
        #undef MEDIAINFO_ADVANCED_NO //MEDIAINFO_ADVANCED_YES has priority
    #endif
    #if defined(MEDIAINFO_ADVANCED_NO)
        #define MEDIAINFO_ADVANCED 0
    #else
        #define MEDIAINFO_ADVANCED 1
    #endif
#endif
#if !defined(MEDIAINFO_ADVANCED2) //ADVANCED2 is for optional build during defualt build
    #if defined(MEDIAINFO_ADVANCED2_NO) && defined(MEDIAINFO_ADVANCED2_YES)
        #undef MEDIAINFO_ADVANCED2_NO //MEDIAINFO_ADVANCED2_YES has priority
    #endif
    #if defined(MEDIAINFO_ADVANCED2_YES)
        #define MEDIAINFO_ADVANCED2 1
    #else
        #define MEDIAINFO_ADVANCED2 0
    #endif
#endif
#if !defined(MEDIAINFO_MD5)
    #if defined(MEDIAINFO_MD5_NO) && defined(MEDIAINFO_MD5_YES)
        #undef MEDIAINFO_MD5_NO //MEDIAINFO_MD5_YES has priority
    #endif
    #if defined(MEDIAINFO_MD5_NO)
        #define MEDIAINFO_MD5 0
    #else
        #define MEDIAINFO_MD5 1
    #endif
#endif
#if !defined(MEDIAINFO_SHA1)
    #if defined(MEDIAINFO_SHA1_NO) && defined(MEDIAINFO_SHA1_YES)
        #undef MEDIAINFO_SHA1_NO //MEDIAINFO_SHA1_YES has priority
    #endif
    #if defined(MEDIAINFO_SHA1_NO)
        #define MEDIAINFO_SHA1 0
    #else
        #define MEDIAINFO_SHA1 1
    #endif
#endif
#if !defined(MEDIAINFO_SHA2)
    #if defined(MEDIAINFO_SHA2_NO) && defined(MEDIAINFO_SHA2_YES)
        #undef MEDIAINFO_SHA2_NO //MEDIAINFO_SHA2_YES has priority
    #endif
    #if defined(MEDIAINFO_SHA2_NO)
        #define MEDIAINFO_SHA2 0
    #else
        #define MEDIAINFO_SHA2 1
    #endif
#endif
#if !defined(MEDIAINFO_DEMUX)
    #if !defined(MEDIAINFO_DEMUX_NO) && !defined(MEDIAINFO_DEMUX_YES) && !MEDIAINFO_EVENTS
        #define MEDIAINFO_DEMUX_NO //MEDIAINFO_DEMUX is disabled by default if MEDIAINFO_EVENTS is set to 0
    #endif
    #if defined(MEDIAINFO_DEMUX_NO) && defined(MEDIAINFO_DEMUX_YES)
        #undef MEDIAINFO_DEMUX_NO //MEDIAINFO_DEMUX_YES has priority
    #endif
    #if defined(MEDIAINFO_DEMUX_NO)
        #define MEDIAINFO_DEMUX 0
    #else
        #define MEDIAINFO_DEMUX 1
    #endif
#endif
#if MEDIAINFO_DEMUX && !MEDIAINFO_EVENTS
    pragma error MEDIAINFO_DEMUX can be set to 1 only if MEDIAINFO_EVENTS is set to 1
#endif
#if !defined(MEDIAINFO_IBI)
    #if defined(MEDIAINFO_IBI_NO) && defined(MEDIAINFO_IBI_YES)
        #undef MEDIAINFO_IBI_NO //MEDIAINFO_IBI_YES has priority
    #endif
    #if defined(MEDIAINFO_IBI_NO)
        #define MEDIAINFO_IBI 0
    #else
        #define MEDIAINFO_IBI 1
    #endif
#endif
#if !defined(MEDIAINFO_IBIUSAGE)
    #if defined(MEDIAINFO_IBIUSAGE_NO) && defined(MEDIAINFO_IBIUSAGE_YES)
        #undef MEDIAINFO_IBIUSAGE_NO //MEDIAINFO_IBIUSAGE_YES has priority
    #endif
    #if defined(MEDIAINFO_IBIUSAGE_NO)
        #define MEDIAINFO_IBIUSAGE 0
    #else
        #if MEDIAINFO_ADVANCED2
            #define MEDIAINFO_IBIUSAGE 1
        #else //MEDIAINFO_ADVANCED2
            #define MEDIAINFO_IBIUSAGE 0
        #endif //MEDIAINFO_ADVANCED2
    #endif
#endif
#if !defined(MEDIAINFO_READTHREAD)
    #if defined(MEDIAINFO_READTHREAD_NO) && defined(MEDIAINFO_READTHREAD_YES)
        #undef MEDIAINFO_READTHREAD_NO //MEDIAINFO_READTHREAD_YES has priority
    #endif
    #if defined(MEDIAINFO_READTHREAD_NO) || !defined(WINDOWS) //Currently supported only on Windows TODO: add support of non Windows OS
        #define MEDIAINFO_READTHREAD 0
    #else
        #define MEDIAINFO_READTHREAD 1
    #endif
#endif
#if !defined(MEDIAINFO_FIXITY)
    #if defined(MEDIAINFO_FIXITY_NO) && defined(MEDIAINFO_FIXITY_YES)
        #undef MEDIAINFO_FIXITY_NO //MEDIAINFO_FIXITY_YES has priority
    #endif
    #if defined(MEDIAINFO_FIXITY_NO)
        #define MEDIAINFO_FIXITY 0
    #else
        #if MEDIAINFO_ADVANCED
            #define MEDIAINFO_FIXITY 1
        #else //MEDIAINFO_ADVANCED
            #define MEDIAINFO_FIXITY 0
        #endif //MEDIAINFO_ADVANCED
    #endif
#endif

//***************************************************************************
// Precise configuration
//***************************************************************************

//---------------------------------------------------------------------------
// Readers
#if !defined(MEDIAINFO_READER_NO) && !defined(MEDIAINFO_DIRECTORY_NO) && !defined(MEDIAINFO_DIRECTORY_YES)
    #define MEDIAINFO_DIRECTORY_YES
#endif
#if !defined(MEDIAINFO_READER_NO) && !defined(MEDIAINFO_FILE_NO) && !defined(MEDIAINFO_FILE_YES)
    #define MEDIAINFO_FILE_YES
#endif
#if !defined(MEDIAINFO_READER_NO) && !defined(MEDIAINFO_LIBCURL_NO) && !defined(MEDIAINFO_LIBCURL_YES)
    #define MEDIAINFO_LIBCURL_YES
#endif
#if !defined(MEDIAINFO_READER_NO) && !defined(MEDIAINFO_LIBMMS_NO) && !defined(MEDIAINFO_LIBMMS_YES)
    #ifndef WINDOWS
        #define MEDIAINFO_LIBMMS_YES
    #endif //WINDOWS
#endif

//---------------------------------------------------------------------------
// Export
#if !defined(MEDIAINFO_EXPORT_NO) && !defined(MEDIAINFO_TEXT_NO) && !defined(MEDIAINFO_TEXT_YES)
    #define MEDIAINFO_TEXT_YES
#endif
#if !defined(MEDIAINFO_EXPORT_NO) && !defined(MEDIAINFO_HTML_NO) && !defined(MEDIAINFO_HTML_YES)
    #define MEDIAINFO_HTML_YES
#endif
#if !defined(MEDIAINFO_EXPORT_NO) && !defined(MEDIAINFO_XML_NO) && !defined(MEDIAINFO_XML_YES)
    #define MEDIAINFO_XML_YES
#endif
#if !defined(MEDIAINFO_EXPORT_NO) && !defined(MEDIAINFO_CSV_NO) && !defined(MEDIAINFO_CSV_YES)
    #define MEDIAINFO_CSV_YES
#endif
#if !defined(MEDIAINFO_EXPORT_NO) && !defined(MEDIAINFO_CUSTOM_NO) && !defined(MEDIAINFO_CUSTOM_YES)
    #define MEDIAINFO_CUSTOM_YES
#endif
#if !defined(MEDIAINFO_EXPORT_NO) && !defined(MEDIAINFO_EBUCORE_NO) && !defined(MEDIAINFO_EBUCORE_YES)
    #define MEDIAINFO_EBUCORE_YES
#endif
#if !defined(MEDIAINFO_EXPORT_NO) && !defined(MEDIAINFO_FIMS_NO) && !defined(MEDIAINFO_FIMS_YES)
    #define MEDIAINFO_FIMS_YES
#endif
#if !defined(MEDIAINFO_EXPORT_NO) && !defined(MEDIAINFO_MPEG7_NO) && !defined(MEDIAINFO_MPEG7_YES)
    #define MEDIAINFO_MPEG7_YES
#endif
#if !defined(MEDIAINFO_EXPORT_NO) && !defined(MEDIAINFO_PBCORE_NO) && !defined(MEDIAINFO_PBCORE_YES)
    #define MEDIAINFO_PBCORE_YES
#endif
#if !defined(MEDIAINFO_EXPORT_NO) && !defined(MEDIAINFO_REVTMD_NO) && !defined(MEDIAINFO_REVTMD_YES)
    #define MEDIAINFO_REVTMD_YES
#endif

//---------------------------------------------------------------------------
// All in one for no parsers
#if defined(MEDIAINFO_ALL_NO) && !defined(MEDIAINFO_MULTI_NO)
    #define MEDIAINFO_MULTI_NO
#endif
#if defined(MEDIAINFO_ALL_NO) && !defined(MEDIAINFO_VIDEO_NO)
    #define MEDIAINFO_VIDEO_NO
#endif
#if defined(MEDIAINFO_ALL_NO) && !defined(MEDIAINFO_AUDIO_NO)
    #define MEDIAINFO_AUDIO_NO
#endif

//---------------------------------------------------------------------------
// Multiple
#if !defined(MEDIAINFO_MULTI_NO) && !defined(MEDIAINFO_MK_NO) && !defined(MEDIAINFO_MK_YES)
    #define MEDIAINFO_MK_YES
#endif

//---------------------------------------------------------------------------
// Video
#if !defined(MEDIAINFO_VIDEO_NO) && !defined(MEDIAINFO_FFV1_NO) && !defined(MEDIAINFO_FFV1_YES)
    #define MEDIAINFO_FFV1_YES
#endif

//---------------------------------------------------------------------------
// Audio
#if !defined(MEDIAINFO_AUDIO_NO) && !defined(MEDIAINFO_PCM_NO) && !defined(MEDIAINFO_PCM_YES)
    #define MEDIAINFO_PCM_YES
#endif

//---------------------------------------------------------------------------
// Other
#if !defined(MEDIAINFO_UNKNOWN_NO)
    #define MEDIAINFO_UNKNOWN_YES
#endif

#endif
