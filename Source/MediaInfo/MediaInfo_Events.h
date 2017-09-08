/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

/*Structures for MediaInfo events */

#ifndef MediaInfo_EventsH
#define MediaInfo_EventsH

#ifdef __cplusplus
#include <stdlib.h>
#include <string.h>
#include <sstream>
#endif //__cplusplus

/***************************************************************************/
/* Platforms (from libzen)                                                 */
/***************************************************************************/

/*-------------------------------------------------------------------------*/
/*Win32*/
#if defined(__NT__) || defined(_WIN32) || defined(WIN32)
    #ifndef WIN32
        #define WIN32
    #endif
    #ifndef _WIN32
        #define _WIN32
    #endif
    #ifndef __WIN32__
        #define __WIN32__ 1
    #endif
#endif

/*-------------------------------------------------------------------------*/
/*Win64*/
#if defined(_WIN64) || defined(WIN64)
    #ifndef WIN64
        #define WIN64
    #endif
    #ifndef _WIN64
        #define _WIN64
    #endif
    #ifndef __WIN64__
        #define __WIN64__ 1
    #endif
#endif

/*-------------------------------------------------------------------------*/
/*Windows*/
#if defined(WIN32) || defined(WIN64)
    #ifndef WINDOWS
        #define WINDOWS
    #endif
    #ifndef _WINDOWS
        #define _WINDOWS
    #endif
    #ifndef __WINDOWS__
        #define __WINDOWS__ 1
    #endif
#endif

/*-------------------------------------------------------------------------*/
/*Unix (Linux, HP, Sun, BeOS...)*/
#if defined(UNIX) || defined(_UNIX) || defined(__UNIX__) \
    || defined(__unix) || defined(__unix__) \
    || defined(____SVR4____) || defined(__LINUX__) || defined(__sgi) \
    || defined(__hpux) || defined(sun) || defined(__SUN__) || defined(_AIX) \
    || defined(__EMX__) || defined(__VMS) || defined(__BEOS__)
    #ifndef UNIX
        #define UNIX
    #endif
    #ifndef _UNIX
        #define _UNIX
    #endif
    #ifndef __UNIX__
        #define __UNIX__ 1
    #endif
#endif

/*-------------------------------------------------------------------------*/
/*MacOS Classic*/
#if defined(macintosh)
    #ifndef MACOS
        #define MACOS
    #endif
    #ifndef _MACOS
        #define _MACOS
    #endif
    #ifndef __MACOS__
        #define __MACOS__ 1
    #endif
#endif

/*-------------------------------------------------------------------------*/
/*MacOS X*/
#if defined(__APPLE__) && defined(__MACH__)
    #ifndef MACOSX
        #define MACOSX
    #endif
    #ifndef _MACOSX
        #define _MACOSX
    #endif
    #ifndef __MACOSX__
        #define __MACOSX__ 1
    #endif
#endif

/*Test of targets*/
#if defined(WINDOWS) && defined(UNIX) && defined(MACOS) && defined(MACOSX)
    #pragma message Multiple platforms???
#endif

#if !defined(WIN32) && !defined(UNIX) && !defined(MACOS) && !defined(MACOSX)
    #pragma message No known platforms, assume default
#endif

/*-------------------------------------------------------------------------*/
/*8-bit int                                                                */
#if UCHAR_MAX==0xff
    #undef  MAXTYPE_INT
    #define MAXTYPE_INT 8
    typedef signed   char       MediaInfo_int8s;
    typedef unsigned char       MediaInfo_int8u;
#else
    #pragma message This machine has no 8-bit integertype?
#endif

/*-------------------------------------------------------------------------*/
/*16-bit int                                                               */
#if UINT_MAX == 0xffff
    #undef  MAXTYPE_INT
    #define MAXTYPE_INT 16
    typedef signed   int        MediaInfo_int16s;
    typedef unsigned int        MediaInfo_int16u;
#elif USHRT_MAX == 0xffff
    #undef  MAXTYPE_INT
    #define MAXTYPE_INT 16
    typedef signed   short      MediaInfo_int16s;
    typedef unsigned short      MediaInfo_int16u;
#else
    #pragma message This machine has no 16-bit integertype?
#endif

/*-------------------------------------------------------------------------*/
/*32-bit int                                                               */
#if UINT_MAX == 0xfffffffful
    #undef  MAXTYPE_INT
    #define MAXTYPE_INT 32
    typedef signed   int        MediaInfo_int32s;
    typedef unsigned int        MediaInfo_int32u;
#elif ULONG_MAX == 0xfffffffful
    #undef  MAXTYPE_INT
    #define MAXTYPE_INT 32
    typedef signed   long       MediaInfo_int32s;
    typedef unsigned long       MediaInfo_int32u;
#elif USHRT_MAX == 0xfffffffful
    #undef  MAXTYPE_INT
    #define MAXTYPE_INT 32
    typedef signed   short      MediaInfo_int32s;
    typedef unsigned short      MediaInfo_int32u;
#else
    #pragma message This machine has no 32-bit integer type?
#endif

/*-------------------------------------------------------------------------*/
/*64-bit int                                                               */
#if defined(__MINGW32__) || defined(__CYGWIN32__) || defined(__UNIX__) || defined(__MACOSX__)
    #undef  MAXTYPE_INT
    #define MAXTYPE_INT 64
    typedef unsigned long long  MediaInfo_int64u;
    typedef   signed long long  MediaInfo_int64s;
#elif defined(__WIN32__) || defined(_WIN32)
    #undef  MAXTYPE_INT
    #define MAXTYPE_INT 64
    typedef unsigned __int64    MediaInfo_int64u;
    typedef   signed __int64    MediaInfo_int64s;
#else
    #pragma message This machine has no 64-bit integer type?
#endif
/*-------------------------------------------------------------------------*/


/***************************************************************************/
/* The callback function                                                   */
/***************************************************************************/

#if !defined(__WINDOWS__) && !defined(__stdcall)
    #define __stdcall
#endif //!defined(__WINDOWS__)

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    typedef void (__stdcall MediaInfo_Event_CallBackFunction)(unsigned char* Data_Content, size_t Data_Size, void* UserHandler);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/***************************************************************************/
/* EventCode management                                                    */
/***************************************************************************/

#define MediaInfo_EventCode_Create(ParserID, EventID, EventVersion) \
  (  ((MediaInfo_int32u)ParserID    )<<24 \
   | ((MediaInfo_int32u)EventID     )<< 8 \
   | ((MediaInfo_int32u)EventVersion)    )

/***************************************************************************/
/* Global                                                                  */
/***************************************************************************/

/*-------------------------------------------------------------------------*/
/* Time code                                                               */
typedef struct MediaInfo_time_code
{
    MediaInfo_int8u     Hours;
    MediaInfo_int8u     Minutes;
    MediaInfo_int8u     Seconds;
    MediaInfo_int8u     Frames;
    MediaInfo_int8u     FramesPerSecond;
    MediaInfo_int8u     DropFrame; //0= No, 1=Yes
    MediaInfo_int8u     Reserved[2];
} MediaInfo_time_code;

/*-------------------------------------------------------------------------*/
/* Generic                                                                 */
#define MEDIAINFO_EVENT_GENERIC \
    MediaInfo_int32u        EventCode; \
    MediaInfo_int32u        ReservedI32; \
    size_t                  EventSize; \
    size_t                  StreamIDs_Size; \
    MediaInfo_int64u        StreamIDs[16]; \
    MediaInfo_int8u         StreamIDs_Width[16]; \
    MediaInfo_int8u         ParserIDs[16]; \
    MediaInfo_int64u        StreamOffset; \
    MediaInfo_int64u        FrameNumber; \
    MediaInfo_int64u        PCR; \
    MediaInfo_int64u        PTS; \
    MediaInfo_int64u        DTS; \
    MediaInfo_int64u        DUR; \
    MediaInfo_int64u        FrameNumber_PresentationOrder; \
    MediaInfo_int64u        ReservedI64[1]; \
    MediaInfo_time_code     TimeCode_Container; \
    MediaInfo_time_code     TimeCode_SDTI; \
    MediaInfo_time_code     TimeCode_RawStream; \
    MediaInfo_time_code     ReservedT[5]; \

typedef struct MediaInfo_Event_Generic
{
    MEDIAINFO_EVENT_GENERIC
} MediaInfo_Event_Generic;

/*-------------------------------------------------------------------------*/
/* MediaInfo_Event_Log_0                                                   */
#define MediaInfo_Event_Log 0x0F00
struct MediaInfo_Event_Log_0
{
    MEDIAINFO_EVENT_GENERIC
    MediaInfo_int8u         Type;
    MediaInfo_int8u         Severity;
    MediaInfo_int8u         Reserved2;
    MediaInfo_int8u         Reserved3;
    MediaInfo_int32u        MessageCode;
    MediaInfo_int32u        Reserved4;
    const wchar_t*          MessageString;
    const wchar_t*          MessageStringU;
    const char*             MessageStringA;
};

/*-------------------------------------------------------------------------*/
/* Demux                                                                   */
#define MediaInfo_Event_Global_Demux 0xAF00
enum MediaInfo_Event_Global_Demux_0_contenttype
{
    MediaInfo_Event_Global_Demux_0_ContentType_MainStream,
    MediaInfo_Event_Global_Demux_0_ContentType_SubStream,
    MediaInfo_Event_Global_Demux_0_ContentType_Header,
    MediaInfo_Event_Global_Demux_0_ContentType_Synchro
};

struct MediaInfo_Event_Global_Demux_4
{
    MEDIAINFO_EVENT_GENERIC
    MediaInfo_int8u         Content_Type; /*MediaInfo_Event_Global_Demux_0_contenttype*/
    size_t                  Content_Size;
    const MediaInfo_int8u*  Content;
    MediaInfo_int64u        Flags; /*bit0=random_access*/
    size_t                  Offsets_Size;
    const MediaInfo_int64u* Offsets_Stream; /* From the begin of the stream */
    const MediaInfo_int64u* Offsets_Content; /* From the begin of the demuxed content */
    size_t                  OriginalContent_Size; /* In case of decoded content inside MediaInfo, OriginalContent contain the not-decoded stream */
    const MediaInfo_int8u*  OriginalContent; /* In case of decoded content inside MediaInfo, OriginalContent contain the not-decoded stream */
};

/*-------------------------------------------------------------------------*/
/* Simple text                                                             */
#define MediaInfo_Event_Global_SimpleText 0xAF01
struct MediaInfo_Event_Global_SimpleText_0
{
    MEDIAINFO_EVENT_GENERIC
    const wchar_t*      Content;
    MediaInfo_int8u     Flags;
    MediaInfo_int8u     MuxingMode;
    MediaInfo_int8u     Service;
    MediaInfo_int32u    Row_Max;
    MediaInfo_int32u    Column_Max;
    wchar_t**           Row_Values; //First indice is the row number, second indice is the column number. Row ends with \0
    MediaInfo_int8u**   Row_Attributes;
};

/*-------------------------------------------------------------------------*/
/* BytesRead                                                               */
#define MediaInfo_Event_Global_BytesRead 0xAF02
struct MediaInfo_Event_Global_BytesRead_0
{
    MEDIAINFO_EVENT_GENERIC
    size_t                  Content_Size;
    const MediaInfo_int8u*  Content;
};

/*-------------------------------------------------------------------------*/
/* Decoded                                                                 */
#define MediaInfo_Event_Global_Decoded 0xAF03

struct MediaInfo_Event_Global_Decoded_0
{
    MEDIAINFO_EVENT_GENERIC
    size_t                  Content_Size;
    const MediaInfo_int8u*  Content;
    MediaInfo_int64u        Flags;
};

/*-------------------------------------------------------------------------*/
/* AttachedFile                                                            */
#define MediaInfo_Event_Global_AttachedFile 0xAF04

struct MediaInfo_Event_Global_AttachedFile_0
{
    MEDIAINFO_EVENT_GENERIC
    size_t                  Content_Size;
    const MediaInfo_int8u*  Content;
    MediaInfo_int64u        Flags;
    const char*             Name;
    const char*             MimeType;
    const char*             Description;
};

/*-------------------------------------------------------------------------*/
/* MediaInfo_Event_Video_SliceInfo_0                                       */
#define MediaInfo_Event_Video_SliceInfo 0x7801
struct MediaInfo_Event_Video_SliceInfo_0
{
    MEDIAINFO_EVENT_GENERIC
    MediaInfo_int64u        FieldPosition;
    MediaInfo_int64u        SlicePosition;
    MediaInfo_int8u         SliceType;
    MediaInfo_int64u        Flags;
};

/***************************************************************************/
/* General                                                                 */
/***************************************************************************/

#define MediaInfo_Parser_None           0x00
#define MediaInfo_Parser_General        0x00
#define MediaInfo_Parser_Global         0x00
#define MediaInfo_Parser_Video          0x01

/*-------------------------------------------------------------------------*/
/* SubFile_Missing                                                         */
#define MediaInfo_Event_General_SubFile_Missing 0x1F01
struct MediaInfo_Event_General_SubFile_Missing_0
{
    MEDIAINFO_EVENT_GENERIC
    const char*             FileName_Relative;
    const wchar_t*          FileName_Relative_Unicode;
    const char*             FileName_Absolute;
    const wchar_t*          FileName_Absolute_Unicode;
};

/*-------------------------------------------------------------------------*/
/* Start                                                                   */
#define MediaInfo_Event_General_Start 0x7001
struct MediaInfo_Event_General_Start_0
{
    MEDIAINFO_EVENT_GENERIC
    MediaInfo_int64u        Stream_Size;
    const char*             FileName;
    const wchar_t*          FileName_Unicode;
};

/*-------------------------------------------------------------------------*/
/* End                                                                     */
#define MediaInfo_Event_General_End 0x7002
struct MediaInfo_Event_General_End_0
{
    MEDIAINFO_EVENT_GENERIC
    MediaInfo_int64u        Stream_Bytes_Analyzed;
    MediaInfo_int64u        Stream_Size;
    MediaInfo_int64u        Stream_Bytes_Padding;
    MediaInfo_int64u        Stream_Bytes_Junk;
};

/*-------------------------------------------------------------------------*/
/* Parser_Selected                                                         */
#define MediaInfo_Event_General_Parser_Selected 0x7003
struct MediaInfo_Event_General_Parser_Selected_0
{
    MEDIAINFO_EVENT_GENERIC
    char                    Name[16];
};

/*-------------------------------------------------------------------------*/
/* Move request                                                            */
#define MediaInfo_Event_General_Move_Request 0x7004
struct MediaInfo_Event_General_Move_Request_0
{
    MEDIAINFO_EVENT_GENERIC
};

/*-------------------------------------------------------------------------*/
/* Move done                                                               */
#define MediaInfo_Event_General_Move_Done 0x7005
struct MediaInfo_Event_General_Move_Done_0
{
    MEDIAINFO_EVENT_GENERIC
};

/*-------------------------------------------------------------------------*/
/* SubFile_Start                                                           */
#define MediaInfo_Event_General_SubFile_Start 0x7006
struct MediaInfo_Event_General_SubFile_Start_0
{
    MEDIAINFO_EVENT_GENERIC
    const char*             FileName_Relative;
    const wchar_t*          FileName_Relative_Unicode;
    const char*             FileName_Absolute;
    const wchar_t*          FileName_Absolute_Unicode;
};

/*-------------------------------------------------------------------------*/
/* SubFile_End                                                             */
#define MediaInfo_Event_General_SubFile_End 0x7007
struct MediaInfo_Event_General_SubFile_End_0
{
    MEDIAINFO_EVENT_GENERIC
};

/***************************************************************************/
/* Matroska                                                                */
/***************************************************************************/

#define MediaInfo_Parser_Matroska       0x08

#endif //MediaInfo_EventsH
