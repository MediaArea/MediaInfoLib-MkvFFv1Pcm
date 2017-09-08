/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// http://www.ffmpeg.org/~michael/ffv1.html
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Pre-compilation
#include "MediaInfo/PreComp.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Setup.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_FFV1_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Video/File_Ffv1.h"
#include "MediaInfo/MediaInfo_Config_MediaInfo.h"
#include "ZenLib/BitStream.h"
//---------------------------------------------------------------------------

#include <algorithm>
#include <math.h>
using namespace std;

//---------------------------------------------------------------------------
namespace MediaInfoLib
{

using namespace FFV1;

//***************************************************************************
// Const
//***************************************************************************

const int32u Slice::Context::N0 = 128;
const int32s Slice::Context::Cmax = 127;
const int32s Slice::Context::Cmin = -128;

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
// CRC_32_Table
// A CRC is computed like this:
// Init: int32u CRC_32 = 0xFFFFFFFF;
// for each data byte do
//     CRC_32=(CRC_32<<8) ^ CRC_32_Table[(CRC_32>>24)^(data_byte)];
static const int32u CRC_32_Table[256] =
{
  0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9,
  0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005,
  0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61,
  0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD,
  0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9,
  0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75,
  0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011,
  0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD,
  0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039,
  0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5,
  0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81,
  0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D,
  0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49,
  0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95,
  0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1,
  0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D,
  0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE,
  0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072,
  0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16,
  0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA,
  0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE,
  0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02,
  0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066,
  0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,
  0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E,
  0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692,
  0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6,
  0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A,
  0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E,
  0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2,
  0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686,
  0xD5B88683, 0xD1799B34, 0xDC3ABDED, 0xD8FBA05A,
  0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637,
  0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB,
  0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F,
  0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53,
  0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47,
  0x36194D42, 0x32D850F5, 0x3F9B762C, 0x3B5A6B9B,
  0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF,
  0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623,
  0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7,
  0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B,
  0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F,
  0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3,
  0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7,
  0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B,
  0x9B3660C6, 0x9FF77D71, 0x92B45BA8, 0x9675461F,
  0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3,
  0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640,
  0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C,
  0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8,
  0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24,
  0x119B4BE9, 0x155A565E, 0x18197087, 0x1CD86D30,
  0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC,
  0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088,
  0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654,
  0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0,
  0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C,
  0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18,
  0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4,
  0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0,
  0x9ABC8BD5, 0x9E7D9662, 0x933EB0BB, 0x97FFAD0C,
  0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668,
  0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4
};

//---------------------------------------------------------------------------
static int32u FFv1_CRC_Compute(const int8u* Buffer, size_t Size)
{
    int32u CRC_32 = 0;
    const int8u* CRC_32_Buffer=Buffer;
    const int8u* CRC_32_Buffer_End=CRC_32_Buffer+Size;

    while(CRC_32_Buffer<CRC_32_Buffer_End)
    {
        CRC_32=(CRC_32<<8) ^ CRC_32_Table[(CRC_32>>24)^(*CRC_32_Buffer)];
        CRC_32_Buffer++;
    }
    return CRC_32;
}

//---------------------------------------------------------------------------
#if MEDIAINFO_FIXITY
static size_t Ffv1_TryToFixCRC(const int8u* Buffer, size_t Buffer_Size)
{
    //looking for a bit flip
    int8u* Buffer2=new int8u[Buffer_Size];
    memcpy(Buffer2, Buffer, Buffer_Size);
    vector<size_t> BitPositions;
    size_t BitPosition_Max=Buffer_Size*8;
    for (size_t BitPosition=0; BitPosition<BitPosition_Max; BitPosition++)
    {
        size_t BytePosition=BitPosition>>3;
        size_t BitInBytePosition=BitPosition&0x7;
        Buffer2[BytePosition]^=1<<BitInBytePosition;
        int32u crc_left_New=FFv1_CRC_Compute(Buffer2, Buffer_Size);
        if (!crc_left_New)
        {
            BitPositions.push_back(BitPosition);
        }
        Buffer2[BytePosition]^=1<<BitInBytePosition;
    }
    delete[] Buffer2; //Buffer2=NULL

    return BitPositions.size()==1?BitPositions[0]:(size_t)-1;
}
#endif //MEDIAINFO_FIXITY

//***************************************************************************
// RangeCoder
//***************************************************************************

class RangeCoder
{
public:
    RangeCoder(const int8u* Buffer, size_t Buffer_Size, const state_transitions default_state_transition);

    void AssignStateTransitions(const state_transitions new_state_transition);
    void   ResizeBuffer(size_t Buffer_Size); //Adapt the buffer limit
    size_t BytesUsed();
    bool   Underrun();
    void   ForceUnderrun();

    bool    get_rac(int8u* States);
    int32u  get_symbol_u(int8u* States);
    int32s  get_symbol_s(int8u* States);

    int32u Current;
    int32u Mask;
    state_transitions zero_state;
    state_transitions one_state;

private:
    const int8u* Buffer_Beg;
    const int8u* Buffer_Cur;
    const int8u* Buffer_End;
};

//---------------------------------------------------------------------------
RangeCoder::RangeCoder (const int8u* Buffer, size_t Buffer_Size, const state_transitions default_state_transition)
{
    //Assign buffer
    Buffer_Beg=Buffer;
    Buffer_Cur=Buffer;
    Buffer_End=Buffer+Buffer_Size;

    //Init
    if (Buffer_Size)
        Current=*Buffer_Cur;
    Mask=0xFF;
    Buffer_Cur++;

    AssignStateTransitions(default_state_transition);
}

void RangeCoder::AssignStateTransitions (const state_transitions new_state_transition)
{
    //Assign StateTransitions
    std::memcpy (one_state, new_state_transition, state_transitions_size);
    zero_state[0]=0;
    for (size_t i=1; i<state_transitions_size; i++)
        zero_state[i]=-one_state[state_transitions_size-i];
}

//---------------------------------------------------------------------------
void RangeCoder::ResizeBuffer(size_t Buffer_Size)
{
    Buffer_End=Buffer_Beg+Buffer_Size;
}

//---------------------------------------------------------------------------
size_t RangeCoder::BytesUsed()
{
    if (Buffer_Cur>Buffer_End)
        return Buffer_End-Buffer_Beg;
    return Buffer_Cur-Buffer_Beg-(Mask<0x100?0:1);
}

//---------------------------------------------------------------------------
bool RangeCoder::Underrun()
{
    return (Buffer_Cur-(Mask<0x100?0:1)>Buffer_End)?true:false;
}

//---------------------------------------------------------------------------
void RangeCoder::ForceUnderrun()
{
    Mask=0;
    Buffer_Cur=Buffer_End+1;
}

//---------------------------------------------------------------------------
bool RangeCoder::get_rac(int8u* States)
{
    // Next byte
    if (Mask<0x100)
    {
        Current<<=8;

        // If less, consume the next byte
        // If equal, last byte assumed to be 0x00
        // If more, underrun, we return 0
        if (Buffer_Cur<Buffer_End)
        {
            Current|=*Buffer_Cur;
        }
        else if (Buffer_Cur>Buffer_End)
        {
            return false;
        }

        Buffer_Cur++;
        Mask<<=8;
    }

    //Range Coder boolean value computing
    int32u Mask2=(Mask*(*States))>>8;
    Mask-=Mask2;
    if (Current<Mask)
    {
        *States=zero_state[*States];
        return false;
    }
    Current-=Mask;
    Mask=Mask2;
    *States=one_state[*States];
    return true;
}

//---------------------------------------------------------------------------
int32u RangeCoder::get_symbol_u(int8u* States)
{
    if (get_rac(States))
        return 0;

    int e = 0;
    while (get_rac(States + 1 + min(e, 9))) // 1..10
    {
        e++;
        if (e > 31)
        {
            ForceUnderrun(); // stream is buggy or unsupported, we disable it completely and we indicate that it is NOK
            return 0;
        }
    }

    int32u a = 1;
    int i = e - 1;
    while (i >= 0)
    {
        a <<= 1;
        if (get_rac(States + 22 + min(i, 9)))  // 22..31
            ++a;
        i--;
    }

    return a;
}

//---------------------------------------------------------------------------
int32s RangeCoder::get_symbol_s(int8u* States)
{
    if (get_rac(States))
        return 0;

    int e = 0;
    while (get_rac(States + 1 + min(e, 9))) // 1..10
    {
        e++;
        if (e > 31)
        {
            ForceUnderrun(); // stream is buggy or unsupported, we disable it completely and we indicate that it is NOK
            return 0;
        }
    }

    int32s a = 1;
    int i = e - 1;
    while (i >= 0)
    {
        a <<= 1;
        if (get_rac(States + 22 + min(i, 9)))  // 22..31
            ++a;
        i--;
    }

    if (get_rac(States + 11 + min(e, 10))) // 11..21
        return -a;
    else
        return a;
}

//***************************************************************************
// Info
//***************************************************************************

static const char* Ffv1_coder_type(int8u coder_type)
{
    switch (coder_type)
    {
        case 0 :
                return "Golomb Rice";
        case 1 :
        case 2 :
                return "Range Coder";
        default:
                return "";
    }
}

static const string Ffv1_colorspace_type(int8u colorspace_type, bool chroma_planes, bool alpha_plane)
{
    string ToReturn;
    switch (colorspace_type)
    {
        case 0 :
                    ToReturn=chroma_planes?"YUV":"Y";
                    break;
        case 1 :    ToReturn="RGB"; break;
        default:    return string();
    }

    if (alpha_plane)
        ToReturn+='A';

    return ToReturn;
}

static const char* Ffv1_picture_structure_ScanType (int8u picture_structure)
{
    switch (picture_structure)
    {
        case 1 :
        case 2 : return "Interlaced";
        case 3 : return "Progressive";
        default: return "";
    }
}

static const char* Ffv1_picture_structure_ScanOrder (int8u picture_structure)
{
    switch (picture_structure)
    {
        case 1 : return "TFF";
        case 2 : return "BFF";
        default: return "";
    }
}

static const state_transitions Ffv1_default_state_transition =
{
      0,  0,  0,  0,  0,  0,  0,  0, 20, 21, 22, 23, 24, 25, 26, 27,
     28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 37, 38, 39, 40, 41, 42,
     43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 56, 57,
     58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73,
     74, 75, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88,
     89, 90, 91, 92, 93, 94, 94, 95, 96, 97, 98, 99,100,101,102,103,
    104,105,106,107,108,109,110,111,112,113,114,114,115,116,117,118,
    119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,133,
    134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,
    150,151,152,152,153,154,155,156,157,158,159,160,161,162,163,164,
    165,166,167,168,169,170,171,171,172,173,174,175,176,177,178,179,
    180,181,182,183,184,185,186,187,188,189,190,190,191,192,194,194,
    195,196,197,198,199,200,201,202,202,204,205,206,207,208,209,209,
    210,211,212,213,215,215,216,217,218,219,220,220,222,223,224,225,
    226,227,227,229,229,230,231,232,234,234,235,236,237,238,239,240,
    241,242,243,244,245,246,247,248,248,  0,  0,  0,  0,  0,  0,  0,
};

// Coming from FFv1 spec.
static const int8u log2_run[41]={
    0 , 0, 0, 0, 1, 1, 1, 1,
    2 , 2, 2, 2, 3, 3, 3, 3,
    4 , 4, 5, 5, 6, 6, 7, 7,
    8 , 9,10,11,12,13,14,15,
    16,17,18,19,20,21,22,23,
    24,
};

static const int32u run[41] =
{
    1 << 0,
    1 << 0,
    1 << 0,
    1 << 0,
    1 << 1,
    1 << 1,
    1 << 1,
    1 << 1,
    1 << 2,
    1 << 2,
    1 << 2,
    1 << 2,
    1 << 3,
    1 << 3,
    1 << 3,
    1 << 3,
    1 << 4,
    1 << 4,
    1 << 5,
    1 << 5,
    1 << 6,
    1 << 6,
    1 << 7,
    1 << 7,
    1 << 8,
    1 << 9,
    1 << 10,
    1 << 11,
    1 << 12,
    1 << 13,
    1 << 14,
    1 << 15,
    1 << 16,
    1 << 17,
    1 << 18,
    1 << 19,
    1 << 20,
    1 << 21,
    1 << 22,
    1 << 23,
    1 << 24,
};

//***************************************************************************
// Slice
//***************************************************************************

//---------------------------------------------------------------------------
void Slice::contexts_clean()
{
    for (size_t i = 0; i < MAX_PLANES; i++)
    {
        if (contexts[i])
            delete[] contexts[i];
    }
}

//---------------------------------------------------------------------------
void Slice::contexts_init(int32u plane_count, int32u quant_table_index[MAX_PLANES], int32u context_count[MAX_QUANT_TABLES])
{
    contexts_clean();

    for (size_t i = 0; i < MAX_PLANES; ++i)
    {
        if (i >= plane_count)
        {
            contexts[i] = NULL;
            continue;
        }
        int32u idx = quant_table_index[i];
        contexts[i] = new Context [context_count[idx]];
    }
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Ffv1::File_Ffv1()
:File__Analyze()
{
    //Configuration
    ParserName="FFV1";
    #if MEDIAINFO_TRACE
        Trace_Layers_Update(8); //Stream
    #endif //MEDIAINFO_TRACE
    IsRawStream=true;

    //use Ffv1_default_state_transition by default
    memcpy(state_transitions_table, Ffv1_default_state_transition,
           sizeof(Ffv1_default_state_transition));

    //Input
    Width = (int32u)-1;
    Height = (int32u)-1;

    //Temp
    for (size_t i=0; i < MAX_QUANT_TABLES; ++i)
    {
        plane_states[i] = NULL;
        plane_states_maxsizes[i] = 0;
    }
    Parameters_IsValid=false;
    ConfigurationRecord_IsPresent=false;
    RC=NULL;
    slices = NULL;
    picture_structure = (int32u)-1;
    sample_aspect_ratio_num = 0;
    sample_aspect_ratio_den = 0;
    KeyFramePassed = false;
    memset(context_count, 0, MAX_QUANT_TABLES*sizeof(int32u));
}

//---------------------------------------------------------------------------
File_Ffv1::~File_Ffv1()
{
    //Temp
    if (slices)
    {
        for (size_t y = 0; y < num_v_slices; ++y)
            for (size_t x = 0; x < num_h_slices; ++x)
                plane_states_clean(slices[x + y * num_h_slices].plane_states);
        delete[] slices;
    }
    for (size_t i = 0; i < MAX_QUANT_TABLES; ++i)
    {
        if (!plane_states[i])
            continue;
        for (size_t j = 0; j < context_count[i]; ++j)
            delete[] plane_states[i][j];
        delete[] plane_states[i];
        plane_states[i] = NULL;
    }
    delete RC; //RC=NULL
}

//***************************************************************************
// Streams management
//***************************************************************************

//---------------------------------------------------------------------------
void File_Ffv1::Streams_Accept()
{
    Stream_Prepare(Stream_Video);
    Fill(Stream_Video, 0, Video_Format, "FFV1");
    Ztring Version=__T("Version ")+Ztring::ToZtring(version);
    if (version>=3 && version<=4)
    {
        Version+=__T('.');
        Version+=Ztring::ToZtring(micro_version);
    }
    Fill(Stream_Video, 0, Video_Format_Version, Version);
}

//***************************************************************************
// RangeCoder
//***************************************************************************

#if MEDIAINFO_TRACE
//---------------------------------------------------------------------------
void File_Ffv1::Get_RB (states &States, bool &Info, const char* Name)
{
    Info=RC->get_rac(States);

    if (Trace_Activated)
    {
        Element_Offset+=RC->BytesUsed();
        Param(Name, Info);
        Element_Offset-=RC->BytesUsed();
    }
}

//---------------------------------------------------------------------------
void File_Ffv1::Get_RU (states &States, int32u &Info, const char* Name)
{
    Info=RC->get_symbol_u(States);

    if (Trace_Activated)
    {
        Element_Offset+=RC->BytesUsed();
        Param(Name, Info);
        Element_Offset-=RC->BytesUsed();
    }
}

//---------------------------------------------------------------------------
void File_Ffv1::Get_RS (states &States, int32s &Info, const char* Name)
{
    Info=RC->get_symbol_s(States);

    if (Trace_Activated)
    {
        Element_Offset+=RC->BytesUsed();
        Param(Name, Info);
        Element_Offset-=RC->BytesUsed();
    }
}

//---------------------------------------------------------------------------
void File_Ffv1::Get_RS (int8u* &States, int32s &Info, const char* Name)
{
    Info=RC->get_symbol_s(States);

    if (Trace_Activated)
    {
        Element_Offset+=RC->BytesUsed();
        Param(Name, Info);
        Element_Offset-=RC->BytesUsed();
    }
}

//---------------------------------------------------------------------------
void File_Ffv1::Skip_RC (states &States, const char* Name)
{
    int8u Info=RC->get_rac(States);

    if (Trace_Activated)
    {
        Element_Offset+=RC->BytesUsed();
        Param(Name, Info);
        Element_Offset-=RC->BytesUsed();
    }
}

//---------------------------------------------------------------------------
void File_Ffv1::Skip_RU (states &States, const char* Name)
{
    if (Trace_Activated)
    {
        Element_Offset+=RC->BytesUsed();
        Param(Name, RC->get_symbol_u(States));
        Element_Offset-=RC->BytesUsed();
    }
    else
        RC->get_symbol_u(States);
}

//---------------------------------------------------------------------------
void File_Ffv1::Skip_RS (states &States, const char* Name)
{
    if (Trace_Activated)
    {
        Element_Offset+=RC->BytesUsed();
        Param(Name, RC->get_symbol_s(States));
        Element_Offset-=RC->BytesUsed();
    }
    else
        RC->get_symbol_s(States);
}

#else //MEDIAINFO_TRACE
//---------------------------------------------------------------------------
void File_Ffv1::Get_RB_ (states &States, bool &Info)
{
    Info=RC->get_rac(States);
}

//---------------------------------------------------------------------------
void File_Ffv1::Get_RU_ (states &States, int32u &Info)
{
    Info=RC->get_symbol_u(States);
}

//---------------------------------------------------------------------------
void File_Ffv1::Get_RS_ (states &States, int32s &Info)
{
    Info=RC->get_symbol_s(States);
}

//---------------------------------------------------------------------------
void File_Ffv1::Get_RS_ (int8u* &States, int32s &Info)
{
    Info=RC->get_symbol_s(States);
}

//---------------------------------------------------------------------------
void File_Ffv1::Skip_RC_ (states &States)
{
    RC->get_rac(States);
}

//---------------------------------------------------------------------------
void File_Ffv1::Skip_RU_ (states &States)
{
    RC->get_symbol_u(States);
}

//---------------------------------------------------------------------------
void File_Ffv1::Skip_RS_ (states &States)
{
    RC->get_symbol_s(States);
}

#endif //MEDIAINFO_TRACE

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_Ffv1::Read_Buffer_OutOfBand()
{
    ConfigurationRecord_IsPresent=true;

    //Coherency tests
    if (Buffer_Size<4)
    {
        Skip_XX(Element_Size,                                   "ConfigurationRecord");
        Param_Error("FFV1-HEADER-END:1");
        return;
    }
    int32u CRC_32=FFv1_CRC_Compute(Buffer+Buffer_Offset, (size_t)Element_Size);

    Element_Begin1("ConfigurationRecord");
    delete RC; RC=new RangeCoder(Buffer, Buffer_Size-4, Ffv1_default_state_transition);
    Parameters();
    delete RC; RC=NULL;
    if (Element_Offset+4<Element_Size)
        Skip_XX(Element_Size-Element_Offset-4,                  "Reserved");
    Skip_B4(                                                    "configuration_record_crc_parity");
    if (CRC_32)
        Param_Error("FFV1-HEADER-configuration_record_crc_parity:1");
    Element_End0();
}

//---------------------------------------------------------------------------
void File_Ffv1::Skip_Frame()
{
    Skip_XX(Element_Size-Element_Offset, "Data");

    Frame_Count++;

    delete RC;
    RC = NULL;

    Fill();
    if (Config->ParseSpeed<1.0)
        Finish();
}

//---------------------------------------------------------------------------
void File_Ffv1::Read_Buffer_Continue()
{
    if (ConfigurationRecord_IsPresent && !Parameters_IsValid)
    {
        Skip_Frame();
        return;
    }

    if (!RC)
        RC = new RangeCoder(Buffer, Buffer_Size, Ffv1_default_state_transition);

    states States;
    memset(States, 128, states_size);

    Element_Begin1("Frame");

    Get_RB (States, keyframe,                                   "keyframe");
    if (intra && !keyframe)
        Param_Error("FFV1-FRAME-key_frame-ISNOTINTRA:1");
    if (keyframe)
        KeyFramePassed=true;

    if (!ConfigurationRecord_IsPresent && keyframe)
    {
        #if MEDIAINFO_TRACE
            bool Trace_Activated_Save=Trace_Activated;
            if (Trace_Activated && Frame_Count)
                Trace_Activated=false; // Trace is relatively huge, temporarary deactivating it. TODO: an option for it
        #endif //MEDIAINFO_TRACE

        Parameters();

        #if MEDIAINFO_TRACE
            Trace_Activated=Trace_Activated_Save; // Trace is too huge, reactivating it.
        #endif //MEDIAINFO_TRACE
    }

    if (!Parameters_IsValid || !KeyFramePassed)
    {
        Skip_Frame();
        return;
    }

    int32u tail = (version >= 3) ? 3 : 0;
    tail += ec == 1 ? 5 : 0;

    vector<int32u> Slices_BufferSizes;
    if (version>=3)
    {
        int64u Slices_BufferPos=Element_Size;
        while (Slices_BufferPos)
        {
            if (Slices_BufferPos<tail)
            {
                //There is a problem
                Slices_BufferSizes.clear();
                break;
            }

            int32u Size=BigEndian2int24u(Buffer+Buffer_Offset+(size_t)Slices_BufferPos-tail);
            Size+=tail;

            if (Size>Slices_BufferPos)
            {
                //There is a problem
                Slices_BufferSizes.clear();
                break;
            }
            Slices_BufferPos-=Size;

            Slices_BufferSizes.insert(Slices_BufferSizes.begin(), Size);
        }
    }

    size_t Pos=0;
    BuggySlices=false;
    while (Element_Offset<Element_Size || (!Pos && coder_type)) // With some v0 RC, content may be in the last byte of the RC which is also in the Parameter() part
    {
        Element_Begin1("Slice");
        int64u Element_Offset_Begin=Element_Offset;
        int64u Element_Size_Save=Element_Size;
        if (Pos<Slices_BufferSizes.size())
            Element_Size=Element_Offset+Slices_BufferSizes[Pos];
        int32u crc_left=0;
        if (ec == 1)
            crc_left=FFv1_CRC_Compute(Buffer+Buffer_Offset+(size_t)Element_Offset, (size_t)(Element_Size-Element_Offset));
        Element_Size-=tail;

        if (Pos)
        {
            delete RC; RC = new RangeCoder(Buffer+Buffer_Offset+(size_t)Element_Offset, Element_Size-Element_Offset, state_transitions_table);
        }
        else
        {
            RC->ResizeBuffer(Element_Size);
            RC->AssignStateTransitions(state_transitions_table);
        }

        //SliceHeader
        bool ParseContent;
        if (version>=3)
        {
            ParseContent=SliceHeader(States);
            if (!ParseContent)
                BuggySlices=true;
        }
        else
            ParseContent=true;

        //SliceContent
        #if MEDIAINFO_TRACE
        if (ParseContent && (!Frame_Count || Trace_Activated)) // Parse slice only if trace feature is activated
        {
            SliceContent(States);
        }
        else
        #endif //MEDIAINFO_TRACE
            Skip_XX(Element_Size-Element_Offset,                "SliceContent");
        if (Element_Offset<Element_Size)
        {
            Skip_XX(Element_Size-Element_Offset,                "Junk");
            Param_Error("FFV1-SLICE-JUNK:1");
        }

        //SliceFooter
        Element_Size=Element_Size_Save;
        if (version>=3)
        {
            Element_Begin1("SliceFooter");
            int32u slice_size;
            Get_B3 (slice_size,                                 "slice_size");
            if (Element_Offset_Begin+slice_size+3!=Element_Offset)
                Param_Error("FFV1-SLICE-slice_size:1");
            if (ec == 1)
            {
                int8u error_status;
                Get_B1 (error_status,                           "error_status");
                if (error_status)
                    Param_Error("FFV1-SLICE-error_status:1");
                Skip_B4(                                        "slice_crc_parity");
                if (crc_left)
                {
                    Param_Error("FFV1-SLICE-slice_crc_parity:1");

                    #if MEDIAINFO_FIXITY
                        if (Config->TryToFix_Get())
                        {
                            size_t BitPosition=Ffv1_TryToFixCRC(Buffer+Buffer_Offset+(size_t)Element_Offset_Begin, Element_Offset-Element_Offset_Begin);
                            if (BitPosition!=(size_t)-1)
                            {
                                size_t BytePosition=BitPosition>>3;
                                size_t BitInBytePosition=BitPosition&0x7;
                                int8u Modified=Buffer[Buffer_Offset+(size_t)Element_Offset_Begin+BytePosition];
                                Modified^=1<<BitInBytePosition;
                                FixFile(File_Offset+Buffer_Offset+(size_t)Element_Offset_Begin+BytePosition, &Modified, 1)?Param_Info1("Fixed"):Param_Info1("Not fixed");
                            }
                        }
                    #endif //MEDIAINFO_FIXITY
                }
                Element_End0();
            }
        }

        Element_End0();
        Pos++;
    }

    //Integrity test
    if (!BuggySlices && version>=3 && slices)
    {
        vector<size_t> SlicesPlaces;
        size_t SlicesPlaces_Size=num_h_slices*num_v_slices;
        SlicesPlaces.resize(num_h_slices*num_v_slices);
        Slice* Slice_Max=slices+SlicesPlaces_Size;
        current_slice=slices;
        while (current_slice<Slice_Max)
        {
            if (current_slice->sample_buffer)
                SlicesPlaces[current_slice->slice_y*num_h_slices+current_slice->slice_x]++;

            current_slice++;
        }
        for (size_t i=0; i<SlicesPlaces_Size; i++)
            if (SlicesPlaces[i]!=1)
            {
                Element_Error("FFV1-FRAME-END:1");
                break;
            }
    }

    Element_End0();

    FILLING_BEGIN();
        if (Frame_Count==0)
        {
            Fill(Stream_Video, 0, Video_ScanType, Ffv1_picture_structure_ScanType(picture_structure));
            Fill(Stream_Video, 0, Video_ScanOrder, Ffv1_picture_structure_ScanOrder(picture_structure));
            if (sample_aspect_ratio_num && sample_aspect_ratio_den)
                Fill(Stream_Video, 0, Video_PixelAspectRatio, ((float64)sample_aspect_ratio_num)/sample_aspect_ratio_den);
        }

        Frame_Count++;
    FILLING_END();

    delete RC;
    RC = NULL;

    Fill();
    if (Config->ParseSpeed<1.0)
        Finish();
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Ffv1::Parameters()
{
    Element_Begin1("Parameters");

    //Parsing
    states States;
    memset(States, 128, states_size);
    Get_RU (States, version,                                    "version");
    if ( ConfigurationRecord_IsPresent && version<=1)
    {
        Param_Error("FFV1-HEADER-version-OUTOFBAND:1");
        Accept(); //TODO: better check without removing error info in trace
        Element_End0();
        return;
    }
    if (!ConfigurationRecord_IsPresent && version> 1)
    {
        Param_Error("FFV1-HEADER-version-OUTOFBAND:1");
        Accept(); //TODO: better check without removing error info in trace
        Element_End0();
        return;
    }
    if (version==2 || version>3)
    {
        Param_Error(version==2?"FFV1-HEADER-version-EXPERIMENTAL:1":"FFV1-HEADER-version-LATERVERSION:1");
        if (version==2 || version>4)
        {
            Accept();
            Element_End0();
            return;
        }
    }
    if (version>=3)
        Get_RU (States, micro_version,                          "micro_version");
    if ((version==3 && micro_version<4))
    {
        Param_Error("FFV1-HEADER-micro_version-EXPERIMENTAL:1");
        Accept();
        Element_End0();
        return;
    }
    if (version==4) // Version 4 is still experimental at the time of writing but contains micro_version so we show it
    {
        Accept();
        Element_End0();
        return;
    }
    if (Frame_Count==0)
        Accept(); //TODO: better check without removing error info in trace
    Get_RU (States, coder_type,                                 "coder_type");
    if (coder_type>2)
    {
        Param_Error("FFV1-HEADER-coder_type:1");
        Element_End0();
        return;
    }
    if (coder_type==2) //Range coder with custom state transition table
    {
        Element_Begin1("state_transition_deltas");
        for (size_t i = 1; i < state_transitions_size; i++)
        {
            int32s state_transition_delta;
            Get_RS (States, state_transition_delta,             "state_transition_delta");
            state_transition_delta+=RC->one_state[i];
            Param_Info1(state_transition_delta);
            if (state_transition_delta<0x00 || state_transition_delta>0xFF)
            {
                Param_Error("FFV1-HEADER-state_transition_delta:1");
                Element_End0();
                Element_End0();
                return;
            }
            state_transitions_table[i]=(int8u)state_transition_delta;
        }
        Element_End0();
    }
    Get_RU (States, colorspace_type,                            "colorspace_type");
    if (colorspace_type>1)
    {
        Param_Error("FFV1-HEADER-colorspace_type:1");
        Element_End0();
        Element_End0();
        return;
    }
    if (version)
    {
        Get_RU (States, bits_per_raw_sample,                    "bits_per_raw_sample");
        if (bits_per_raw_sample>64)
        {
            Param_Error("FFV1-HEADER-bits_per_raw_sample:1");
            Element_End0();
            return;
        }
        if (bits_per_raw_sample==0)
            bits_per_raw_sample=8; // Spec decision due to old encoders
    }
    else
    {
        bits_per_raw_sample=8;
    }
    
    Get_RB (States, chroma_planes,                              "chroma_planes");
    Get_RU (States, h_chroma_subsample_log2,                    "log2(h_chroma_subsample)");
    Get_RU (States, v_chroma_subsample_log2,                    "log2(h_chroma_subsample)");
    Get_RB (States, alpha_plane,                                "alpha_plane");
    if (version>1)
    {
        Get_RU (States, num_h_slices,                           "num_h_slices_minus1");
        if (num_h_slices>=Width)
        {
            Param_Error("FFV1-HEADER-num_h_slices:1");
            Element_End0();
            return;
        }
        Get_RU (States, num_v_slices,                           "num_v_slices_minus1");
        if (num_v_slices>=Height)
        {
            Param_Error("FFV1-HEADER-num_v_slices:1");
            Element_End0();
            return;
        }
        num_h_slices++;
        num_v_slices++;
        Get_RU (States, quant_table_count,                      "quant_table_count");
        if (quant_table_count>8)
        {
            Param_Error("FFV1-HEADER-quant_table_count:1");
            Element_End0();
            return;
        }
    }
    else
    {
        num_h_slices=1;
        num_v_slices=1;
        quant_table_count=1;
    }
    for (size_t i=0; i<quant_table_count; i++)
        if (!QuantizationTable(i))
        {
            Element_End0();
            return;
        }
    for (size_t i=0; i<quant_table_count; i++)
    {
        if (version>=2)
            Element_Begin1("initial_state");
        bool present=false;
        if (version>=2)
            Get_RB (States, present,                                "states_coded");

        if (coder_type && context_count[i]>plane_states_maxsizes[i])
        {
            states_context_plane plane_state_old = plane_states[i];
            plane_states[i] = new int8u*[context_count[i]];
            if (plane_state_old)
            {
                memcpy(plane_states[i], plane_state_old, plane_states_maxsizes[i] * sizeof(int8u*));
                delete[] plane_state_old;
            }
            for (size_t j = plane_states_maxsizes[i]; j < context_count[i]; j++)
                plane_states[i][j] = new int8u[states_size];
            plane_states_maxsizes[i] = context_count[i];
        }

        for (size_t j = 0; j < context_count[i]; j++)
        {
            if (present)
            {
                Element_Begin1("initial_state_deltas");
                for (size_t k = 0; k < states_size; k++)
                {
                    int32s value;
                    Get_RS (States, value,                  "initial_state_delta");
                    if (coder_type)
                        plane_states[i][j][k] = value;
                }
                Element_End0();
            }
            else if (coder_type)
            {
                for (size_t k = 0; k < states_size; k++)
                    plane_states[i][j][k] = 128;
            }
        }
        if (version>=2)
            Element_End0();
    }

    if (version>=3)
    {
        Get_RU (States, ec,                                     "ec");
        if (ec>1)
        {
            Param_Error("FFV1-HEADER-ec:1");
            Element_End0();
            return;
        }
        if (micro_version)
        {
            Get_RU (States, intra,                              "intra");
            if (intra>1)
            {
                Param_Error("FFV1-HEADER-intra:1");
                Element_End0();
                return;
            }
        }
        else
            intra=0;
    }
    else
    {
        ec=0;
        intra=0;
    }

    if (!coder_type && version<=1)
    {
        states States;
        memset(States, 129, states_size);
        Skip_RC(States,                                         "end");
    }

    Element_End0();

    FILLING_BEGIN();
        //Marking handling of 16-bit overflow computing
        is_overflow_16bit=(colorspace_type==0 && bits_per_raw_sample==16 && (coder_type==1 || coder_type==2))?true:false; //TODO: check in FFmpeg the version when the stream is fixed. Note: only with YUV colorspace

        //quant_table_index_Count
        quant_table_index_count=1+(alpha_plane?1:0);
        if (version < 4 || chroma_planes) // Warning: chroma is considered as 1 plane
            quant_table_index_count++;

        //Slices
        if (!slices)
        {
            slices=new Slice[num_h_slices*num_v_slices];
            current_slice=&slices[0];
        }
        if (version<=1)
        {
            current_slice->x=0;
            current_slice->y=0;
            current_slice->w=Width;
            current_slice->h=Height;
            quant_table_index[0]=0;
            for (size_t i=1; i<quant_table_index_count; i++)
            {
                quant_table_index[i]=0;
                context_count[i]=context_count[0];
            }
        }

        //Filling
        if (Frame_Count==0)
        {
            Fill(Stream_Video, 0, "coder_type", Ffv1_coder_type(coder_type));
            Fill(Stream_Video, 0, Video_BitDepth, bits_per_raw_sample);
            if (version>1)
            {
                Fill(Stream_Video, 0, "MaxSlicesCount", num_h_slices*num_v_slices);
                if (version>2)
                {
                    if (ec)
                        Fill(Stream_Video, 0, "ErrorDetectionType", "Per slice");
                    if (micro_version && intra)
                        Fill(Stream_Video, 0, Video_Format_Settings_GOP, "N=1");
                }
            }
            Fill(Stream_Video, 0, Video_ColorSpace, Ffv1_colorspace_type(colorspace_type, chroma_planes, alpha_plane));
            if (colorspace_type==0 && chroma_planes)
            {
                string ChromaSubsampling;
                switch (h_chroma_subsample_log2)
                {
                    case 0 :
                            switch (v_chroma_subsample_log2)
                            {
                                case 0 : ChromaSubsampling="4:4:4"; break;
                                default: ;
                            }
                            break;
                    case 1 :
                            switch (v_chroma_subsample_log2)
                            {
                                case 0 : ChromaSubsampling="4:2:2"; break;
                                case 1 : ChromaSubsampling="4:2:0"; break;
                                default: ;
                            }
                            break;
                    case 2 :
                            switch (v_chroma_subsample_log2)
                            {
                                case 0 : ChromaSubsampling="4:1:1"; break;
                                case 1 : ChromaSubsampling="4:1:0"; break;
                                case 2 : ChromaSubsampling="4:1:0 (4x4)"; break;
                                default: ;
                            }
                            break;
                    default: ;
                }
                if (!ChromaSubsampling.empty() && alpha_plane)
                    ChromaSubsampling+=":4";
                Fill(Stream_Video, 0, Video_ChromaSubsampling, ChromaSubsampling);
            }

            Parameters_IsValid=true;
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Ffv1::SliceContent(states &States)
{
    Element_Begin1("SliceContent");

    #if MEDIAINFO_TRACE
        bool Trace_Activated_Save=Trace_Activated;
        if (Trace_Activated)
            Trace_Activated=false; // Trace is too huge, deactivating it during pixel decoding
    #endif //MEDIAINFO_TRACE

    if (!coder_type)
    {
        if ((version == 3 && micro_version > 1) || version > 3)
        {
            states States;
            memset(States, 129, states_size);
            Skip_RC(States,                                     "?");
        }
        if ((version > 2 || (!current_slice->x && !current_slice->y)))
            Element_Offset+=RC->BytesUsed(); // Computing how many bytes where consumed by the range coder
        else
            Element_Offset=0;
        BS_Begin();
    }

    if (keyframe)
    {
        int8u plane_count=1+(alpha_plane?1:0);
        if (version < 4 || chroma_planes) // Warning: chroma is considered as 1 plane
            plane_count+=1;
        if (!coder_type)
            current_slice->contexts_init(plane_count, quant_table_index, context_count);
        else
            copy_plane_states_to_slice(plane_count);
    }
    current_slice->sample_buffer_new((current_slice->w + 6) * 3 * MAX_PLANES);

    if (colorspace_type == 0)
    {
        // YCbCr
        plane(0); // Y
        if (chroma_planes)
        {
            int32u w = current_slice->w;
            int32u h = current_slice->h;

            current_slice->w = w >> h_chroma_subsample_log2;
            if (w & ((1 << h_chroma_subsample_log2) - 1))
                current_slice->w++; //Is ceil
            current_slice->h = h >> v_chroma_subsample_log2;
            if (h & ((1 << v_chroma_subsample_log2) - 1))
                current_slice->h++; //Is ceil
            plane(1); // Cb
            plane(1); // Cr
            current_slice->w = w;
            current_slice->h = h;
        }
        if (alpha_plane)
            plane(2); // Alpha
    }
    else if (colorspace_type == 1)
        rgb();

    if (BS->BufferUnderRun || RC->Underrun())
        Element_Error("FFV1-SLICE-SliceContent:1");

    if (!coder_type)
        BS_End();

    if (coder_type)
    {
        //if (version > 2)
        {
            int8u s = 129;
            RC->get_rac(&s);
        }
        Skip_XX(RC->BytesUsed(),                                "slice_data");
    }

    #if MEDIAINFO_DECODE
        //Decode(Buffer, Buffer_Size);
    #endif //MEDIAINFO_DECODE

    #if MEDIAINFO_TRACE
        Trace_Activated=Trace_Activated_Save; // Trace is too huge, reactivating after during pixel decoding
    #endif //MEDIAINFO_TRACE
    Element_End0();
}

//---------------------------------------------------------------------------
bool File_Ffv1::SliceHeader(states &States)
{
    Element_Begin1("SliceHeader");

    memset(States, 128, states_size);

    int32u slice_x, slice_y, slice_width_minus1, slice_height_minus1;
    Get_RU (States, slice_x,                                "slice_x");
    if (slice_x >= num_h_slices)
    {
        Param_Error("FFV1-SLICE-slice_xywh:1");
        Element_End0();
        return false;
    }

    Get_RU (States, slice_y,                                "slice_y");
    if (slice_y >= num_h_slices)
    {
        Param_Error("FFV1-SLICE-slice_xywh:1");
        Element_End0();
        return false;
    }

    Get_RU (States, slice_width_minus1,                     "slice_width_minus1");
    int32u slice_x2 = slice_x + slice_width_minus1 + 1; //right boundary
    if (slice_x2 > num_h_slices)
    {
        Param_Error("FFV1-SLICE-slice_xywh:1");
        Element_End0();
        return false;
    }

    Get_RU (States, slice_height_minus1,                    "slice_height_minus1");
    int32u slice_y2 = slice_y + slice_height_minus1 + 1; //bottom boundary
    if (slice_y2 > num_v_slices)
    {
        Param_Error("FFV1-SLICE-slice_xywh:1");
        Element_End0();
        return false;
    }

    current_slice = &slices[slice_x + slice_y * num_h_slices];
    current_slice->slice_x = slice_x;
    current_slice->slice_y = slice_y;
    current_slice->slice_w = slice_x2;
    current_slice->slice_h = slice_y2;

    //Computing boundaries, being careful about how are computed boundaries when there is not an integral number for Width  / num_h_slices or Height / num_v_slices (the last slice has more pixels)
    current_slice->x = slice_x  * Width  / num_h_slices;
    current_slice->y = slice_y  * Height / num_v_slices;
    current_slice->w = slice_x2 * Width  / num_h_slices - current_slice->x;
    current_slice->h = slice_y2 * Height / num_v_slices - current_slice->y;


    for (int8u i = 0; i < quant_table_index_count; i++)
    {
        Get_RU (States, quant_table_index[i],               "quant_table_index");
        if (quant_table_index[i]>=quant_table_count)
        {
            Param_Error("FFV1-SLICE-quant_table_index:1");
            Element_End0();
            return false;
        }
    }
    Get_RU (States, picture_structure,                      "picture_structure");
    if (picture_structure>3)
        Param_Error("FFV1-SLICE-picture_structure:1");
    Get_RU (States, sample_aspect_ratio_num,                "sar_num");
    Get_RU (States, sample_aspect_ratio_den,                "sar_den");
    if ((sample_aspect_ratio_num && !sample_aspect_ratio_den) || (!sample_aspect_ratio_num && sample_aspect_ratio_den))
        Param_Error("FFV1-SLICE-sar_den:1");
    if (version > 3)
    {
        //TODO
    }

    RC->AssignStateTransitions(state_transitions_table);

    if (!coder_type)
    {
        states States;
        memset(States, 129, states_size);
        Skip_RC(States,                                     "end");
    }
    //Element_Offset+=RC->BytesUsed(); // Computing how many bytes where consumed by the range coder

    Element_End0();
    return true;
}

//---------------------------------------------------------------------------
void File_Ffv1::plane(int32u pos)
{
    #if MEDIAINFO_TRACE_FFV1CONTENT
        Element_Begin1("Plane");
    #endif //MEDIAINFO_TRACE_FFV1CONTENT

    if (bits_per_raw_sample <= 8)
        bits_max = 8;
    else
        bits_max = bits_per_raw_sample;
    bits_mask1 = ((1 << bits_max) - 1);
    bits_mask2 = 1 << (bits_max - 1);
    bits_mask3 = bits_mask2 - 1;

    pixel_t *sample[2];
    sample[0] = current_slice->sample_buffer + 3;
    sample[1] = sample[0] + current_slice->w + 6;

    memset(current_slice->sample_buffer, 0, 2 * (current_slice->w + 6) * sizeof(*current_slice->sample_buffer));

    current_slice->run_index = 0;

    for (size_t y = 0; y < current_slice->h; y++)
    {
        #if MEDIAINFO_TRACE_FFV1CONTENT
            Element_Begin1("Line");
            Element_Info1(y);
        #endif //MEDIAINFO_TRACE_FFV1CONTENT

        swap(sample[0], sample[1]);

        sample[1][-1] = sample[0][0];
        sample[0][current_slice->w]  = sample[0][current_slice->w - 1];

        line(pos, sample);

        #if MEDIAINFO_TRACE_FFV1CONTENT
            Element_End0();
        #endif //MEDIAINFO_TRACE_FFV1CONTENT
    }

    #if MEDIAINFO_TRACE_FFV1CONTENT
        Element_End0();
    #endif //MEDIAINFO_TRACE_FFV1CONTENT
}

//---------------------------------------------------------------------------
void File_Ffv1::rgb()
{
    #if MEDIAINFO_TRACE_FFV1CONTENT
        Element_Begin1("rgb");
    #endif //MEDIAINFO_TRACE_FFV1CONTENT

    bits_max = bits_per_raw_sample + 1;
    bits_mask1 = (1 << bits_max) - 1;
    bits_mask2 = 1 << (bits_max - 1);
    bits_mask3 = bits_mask2-1;

    size_t c_max = alpha_plane ? 4 : 3;

    pixel_t *sample[4][2];

    current_slice->run_index = 0;

    for (size_t x = 0; x < c_max; x++) {
        sample[x][0] = current_slice->sample_buffer +  x * 2      * (current_slice->w + 6) + 3;
        sample[x][1] = sample[x][0] + current_slice->w + 6;
    }
    memset(current_slice->sample_buffer, 0, 8 * (current_slice->w + 6) * sizeof(*current_slice->sample_buffer));

    for (size_t y = 0; y < current_slice->h; y++)
    {
        #if MEDIAINFO_TRACE_FFV1CONTENT
            Element_Begin1("Line");
            Element_Info1(y);
        #endif //MEDIAINFO_TRACE_FFV1CONTENT

        for (size_t c = 0; c < c_max; c++)
        {
            // Copy for next lines: 4.3 context
            swap(sample[c][0], sample[c][1]);

            sample[c][1][-1]= sample[c][0][0  ];
            sample[c][0][current_slice->w]= sample[c][0][current_slice->w - 1];

            line((c + 1) / 2, sample[c]);
        }

        #if MEDIAINFO_TRACE_FFV1CONTENT
            Element_End0();
        #endif //MEDIAINFO_TRACE_FFV1CONTENT
    }

    #if MEDIAINFO_TRACE_FFV1CONTENT
        Element_End0();
    #endif //MEDIAINFO_TRACE_FFV1CONTENT
}

//---------------------------------------------------------------------------
static inline int32s get_median_number(int32s one, int32s two, int32s three)
{
    if (one > two)
    {
        // one > two > three
        if (two > three)
            return two;

        // three > one > two
        if (three > one)
            return one;
        // one > three > two
        return three;
    }

    // three > two > one
    if (three > two)
        return two;

    // two > one && two > three

    // two > three > one
    if (three > one)
        return three;
    return one;
}

//---------------------------------------------------------------------------
static inline int32s predict(pixel_t *current, pixel_t *current_top, bool is_overflow_16bit)
{
    int32s LeftTop, Top, Left;
    if (is_overflow_16bit)
    {
        LeftTop = (int16s)current_top[-1];
        Top = (int16s)current_top[0];
        Left = (int16s)current[-1];
    }
    else
    {
        LeftTop = current_top[-1];
        Top = current_top[0];
        Left = current[-1];
    }

    return get_median_number(Left, Left + Top - LeftTop, Top);
}

//---------------------------------------------------------------------------
static inline int get_context_3(pixel_t quant_table[MAX_CONTEXT_INPUTS][256], pixel_t *src, pixel_t *last)
{
    const int LT = last[-1];
    const int T  = last[0];
    const int RT = last[1];
    const int L  = src[-1];

    return quant_table[0][(L - LT) & 0xFF]
        + quant_table[1][(LT - T) & 0xFF]
        + quant_table[2][(T - RT) & 0xFF];
}
static inline int get_context_5(pixel_t quant_table[MAX_CONTEXT_INPUTS][256], pixel_t *src, pixel_t *last)
{
    const int LT = last[-1];
    const int T  = last[0];
    const int RT = last[1];
    const int L  = src[-1];
    const int TT = src[0];
    const int LL = src[-2];
    return quant_table[0][(L - LT) & 0xFF]
        + quant_table[1][(LT - T) & 0xFF]
        + quant_table[2][(T - RT) & 0xFF]
        + quant_table[3][(LL - L) & 0xFF]
        + quant_table[4][(TT - T) & 0xFF];
}

//---------------------------------------------------------------------------
int32s File_Ffv1::pixel_RC(int32s context)
{
    int32s u;

    Get_RS(Context_RC[context], u, "symbol");
    return u;
}


//---------------------------------------------------------------------------
int32s File_Ffv1::pixel_GR(int32s context)
{
#if MEDIAINFO_TRACE_FFV1CONTENT
    int32s u;

    // New symbol, go to "run mode"
    if (context == 0 && current_slice->run_mode == RUN_MODE_STOP)
        current_slice->run_mode = RUN_MODE_PROCESSING;

    // If not running, get the symbol
    if (current_slice->run_mode == RUN_MODE_STOP)
    {
        u = get_symbol_with_bias_correlation(&Context_GR[context]);
        #if MEDIAINFO_TRACE
        Param("symbol", u);
        #endif //MEDIAINFO_TRACE
        return u;
    }

    if (current_slice->run_segment_length == 0 && current_slice->run_mode == RUN_MODE_PROCESSING) // Same symbol length
    {
        bool hits;
        Get_SB (hits,                                           "hits/miss");
        //if (bsf.GetB()) // "hits"
        if (hits) // "hits"
        {
            current_slice->run_segment_length = run[current_slice->run_index];
            if (x + current_slice->run_segment_length <= current_slice->w) //Do not go further as the end of line
                ++current_slice->run_index;
        }
        else // "miss"
        {
            //current_slice->run_segment_length = bsf.Get4(log2_run[current_slice->run_index]);
            int32u run_segment_length;
            Get_S4 (log2_run[current_slice->run_index], run_segment_length,  "run_segment_length");
            current_slice->run_segment_length=(int32s)run_segment_length;
            if (current_slice->run_index)
                --current_slice->run_index;
            current_slice->run_mode = RUN_MODE_INTERRUPTED;
        }
    }

    current_slice->run_segment_length--;
    if (current_slice->run_segment_length < 0) // we passed the length of same symbol, time to get the new symbol
    {
        u = get_symbol_with_bias_correlation(&Context_GR[context]);
        #if MEDIAINFO_TRACE
        Param("symbol", u);
        #endif //MEDIAINFO_TRACE
        if (u >= 0) // GR(u - 1, ...)
            u++;

        // Time for the new symbol length run
        current_slice->run_mode_init();
        current_slice->run_segment_length = 0;
    } else // same symbol as previous pixel, no difference, waiting
        u = 0;
    return u;
#else //MEDIAINFO_TRACE_FFV1CONTENT
    if (current_slice->run_mode == RUN_MODE_STOP)
    {
        if (context)
            return get_symbol_with_bias_correlation(&Context_GR[context]); // If not running, get the symbol

        current_slice->run_mode = RUN_MODE_PROCESSING; // New symbol, go to "run mode"
    }

    if (current_slice->run_segment_length == 0 && current_slice->run_mode == RUN_MODE_PROCESSING) // Same symbol length
    {
        if (BS->GetB()) // "hits"
        {
            current_slice->run_segment_length = run[current_slice->run_index];
            if (x + current_slice->run_segment_length <= current_slice->w) //Do not go further as the end of line
                ++current_slice->run_index;
            if (--current_slice->run_segment_length >= 0)
                return 0;
        }
        else // "miss"
        {
            current_slice->run_mode = RUN_MODE_INTERRUPTED;

            if (current_slice->run_index)
            {
                int8u count = log2_run[current_slice->run_index--];
                if (count)
                {
                    current_slice->run_segment_length = ((int32s)BS->Get4(count)) - 1;
                    if (current_slice->run_segment_length >= 0)
                        return 0;
                }
                else
                    current_slice->run_segment_length = -1;
            }
            else
                current_slice->run_segment_length = -1;
        }
    }
    else if (--current_slice->run_segment_length >= 0)
        return 0;

    // Time for the new symbol length run
    current_slice->run_mode_init();

    int32s u = get_symbol_with_bias_correlation(&Context_GR[context]);
    if (u >= 0) // GR(u - 1, ...)
        u++;
    return u;
#endif //MEDIAINFO_TRACE_FFV1CONTENT
}

//---------------------------------------------------------------------------
void File_Ffv1::line(int pos, pixel_t *sample[2])
{
    // TODO: slice_coding_mode (version 4)

    quant_table_struct& quant_table = quant_tables[quant_table_index[pos]];
    bool Is5 = quant_table[3][127] ? true : false;
    pixel_t* s0c = sample[0];
    pixel_t* s0e = s0c + current_slice->w;
    pixel_t* s1c = sample[1];

    if (coder_type)
    {
        Context_RC = current_slice->plane_states[pos];

        while (s0c<s0e)
        {
            int32s context = Is5 ? get_context_5(quant_table, s1c, s0c) : get_context_3(quant_table, s1c, s0c);

            int32s Value = predict(s1c, s0c, is_overflow_16bit);
            #if MEDIAINFO_TRACE_FFV1CONTENT
                if (context >= 0)
                    Value += pixel_RC(context);
                else
                    Value -= pixel_RC(-context);
            #else //MEDIAINFO_TRACE_FFV1CONTENT
                if (context >= 0)
                    Value += RC->get_symbol_s(Context_RC[context]);
                else
                    Value -= RC->get_symbol_s(Context_RC[-context]);
            #endif //MEDIAINFO_TRACE_FFV1CONTENT;
            *s1c = Value & bits_mask1;

            s0c++;
            s1c++;
        }
    }
    else
    {
        current_slice->run_mode_init();

        Context_GR = current_slice->contexts[pos];
        x = 0;

        while (s0c < s0e)
        {
            int32s context = Is5 ? get_context_5(quant_table, s1c, s0c) : get_context_3(quant_table, s1c, s0c);

            *s1c = (predict(s1c, s0c, is_overflow_16bit) + (context >= 0 ? pixel_GR(context) : -pixel_GR(-context))) & bits_mask1;

            s0c++;
            s1c++;
            x++;
        }
    }
}

//---------------------------------------------------------------------------
bool File_Ffv1::QuantizationTable(size_t i)
{
    Element_Begin1("QuantizationTableSet");

    FFV1::pixel_t scale = 1;

    for (size_t j = 0; j < 5; j++)
    {
        if (!QuantizationTablePerContext(i, j, scale))
        {
            Element_End0();
            return false;
        }
    }
    context_count[i] = (scale + 1) / 2;

    Element_End0();
    return true;
}

//---------------------------------------------------------------------------
bool File_Ffv1::QuantizationTablePerContext(size_t i, size_t j, FFV1::pixel_t &scale)
{
    Element_Begin1("QuantizationTable");

    int8u States[states_size];
    memset(States, 128, sizeof(States));

    FFV1::pixel_t v = 0;
    for (size_t k=0; k < 128;)
    {
        int32u len_minus1;
        Get_RU (States, len_minus1,                             "len_minus1");

        if (k+len_minus1 >= 128)
        {
            Param_Error("FFV1-HEADER-QuantizationTable-len:1");
            Element_End0();
            return false;
        }

        for (int32u a=0; a<=len_minus1; a++)
        {
            quant_tables[i][j][k] = scale * v;
            k++;
        }

        v++;
    }

    for (int k = 1; k < 128; k++)
        quant_tables[i][j][256 - k] = -quant_tables[i][j][k];
    quant_tables[i][j][128] = -quant_tables[i][j][127];

    scale *= 2 * v - 1;
    if (scale > 32768U)
    {
        Element_Error("FFV1-HEADER-QuantizationTable-scale:1");
        Element_End0();
        return false;
    }

    Element_End0();
    return true;
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
int32s File_Ffv1::golomb_rice_decode(int k)
{
#if MEDIAINFO_TRACE_FFV1CONTENT
    int32u q = 0;
    int32u v;

    //while (bsf.Remain() > 0 && q < PREFIX_MAX && !bsf.GetB())
    while (Data_BS_Remain() > 0 && q < PREFIX_MAX)
    {
        bool Temp;
        Get_SB(Temp,                                            "golomb_rice_prefix_0");
        if (Temp)
            break;

        ++q;
    }

    if (q == PREFIX_MAX) // ESC
    {
        //v = bsf.Get4(bits_max) + 11;
        Get_S4(bits_max, v,                                     "escaped_value_minus_11");
        v+=11;
    }
    else
    {
        //int32u remain = bsf.Get8(k); // Read k bits
        int32u remain;
        Get_S4(k, remain,                                       "golomb_rice_remain");
        int32u mul = q << k; // q * pow(2, k)

        v = mul | remain;
    }

    // unsigned to signed
    int32s code = (v >> 1) ^ -(v & 1);
    return code;
#else //MEDIAINFO_TRACE_FFV1CONTENT
    int8u q = 0;
    while (BS->Remain() && !BS->GetB())
        if (++q >= PREFIX_MAX)
        {
            int32s v = 11 + BS->Get4(bits_max);

            // unsigned to signed
            return (v >> 1) ^ -(v & 1);
        }

    int32s v = (q << k) | BS->Get4(k);

    // unsigned to signed
    return (v >> 1) ^ -(v & 1);
#endif //MEDIAINFO_TRACE_FFV1CONTENT
}

//---------------------------------------------------------------------------
int32s File_Ffv1::get_symbol_with_bias_correlation(Slice::ContextPtr context)
{
    int k = 0;
    // Step 8: compute the Golomb parameter k
    for (k = 0; (context->N << k) < context->A; ++k);

    // Step 10: Decode Golomb code (using limitation PREFIX_MAX == 12)
    int32s code = golomb_rice_decode(k);

    // Step 9: Mapping
    int32s M = 2 * context->B + context->N;
    code = code ^ (M >> 31);

    // Step 11
    context->B += code;
    context->A += code >= 0 ? code : -code;

    code += context->C;

    context->update_correlation_value_and_shift();

    // Step 7 (TODO better way)
    bool neg = code & bits_mask2; // check if the number is negative
    code = code & bits_mask3; // Keep only the n bits
    if (neg)
        code = - 1 - (~code & bits_mask3); // 0xFFFFFFFF - positive value on n bits

    return code;
}

//---------------------------------------------------------------------------
void File_Ffv1::copy_plane_states_to_slice(int8u plane_count)
{
    if (!coder_type)
        return;

    for (size_t i = 0; i < plane_count; i++)
    {
        int32u idx = quant_table_index[i];
        if (!current_slice->plane_states[i])
        {
            current_slice->plane_states[i] = new int8u*[context_count[idx] + 1];
            current_slice->plane_states_maxsizes[i] = context_count[idx] + 1;
            memset(current_slice->plane_states[i], 0, (context_count[idx] + 1) * sizeof(int8u*));
        }

        for (size_t j = 0; j < context_count[idx]; j++)
        {
            if (!current_slice->plane_states[i][j])
                current_slice->plane_states[i][j] = new int8u [states_size];
            for (size_t k = 0; k < states_size; k++)
            {
                current_slice->plane_states[i][j][k] = plane_states[idx][j][k];
            }
        }
    }
}

//---------------------------------------------------------------------------
void File_Ffv1::plane_states_clean(states_context_plane states[MAX_QUANT_TABLES])
{
    if (!coder_type)
        return;

    for (size_t i = 0; i < MAX_QUANT_TABLES && states[i]; ++i)
    {
        for (size_t j = 0; states[i][j]; ++j)
            delete[] states[i][j];

        delete[] states[i];
        states[i] = NULL;
    }
}

} //NameSpace

#endif //MEDIAINFO_FFV1_YES
