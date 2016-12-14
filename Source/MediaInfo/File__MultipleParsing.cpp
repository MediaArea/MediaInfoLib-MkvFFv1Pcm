/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

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
#include "MediaInfo/File__MultipleParsing.h"
//---------------------------------------------------------------------------
// Multiple
#if defined(MEDIAINFO_MK_YES)
    #include "MediaInfo/Multiple/File_Mk.h"
#endif

#include "MediaInfo/File_Unknown.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Out
//***************************************************************************

//---------------------------------------------------------------------------
File__Analyze* File__MultipleParsing::Parser_Get()
{
    if (Parser.size()!=1)
        return NULL;

    File__Analyze* ToReturn=Parser[0]; //The first parser
    Parser.clear();
    return ToReturn;
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File__MultipleParsing::File__MultipleParsing()
:File__Analyze()
{
    #if MEDIAINFO_TRACE
        Trace_DoNotSave=true;
    #endif //MEDIAINFO_TRACE

    // Multiple
    #if defined(MEDIAINFO_MK_YES)
        Parser.push_back(new File_Mk());
    #endif
}

//---------------------------------------------------------------------------
File__MultipleParsing::~File__MultipleParsing()
{
    for (size_t Pos=0; Pos<Parser.size(); Pos++)
        delete Parser[Pos]; //Parser[Pos]=NULL
}

//***************************************************************************
// Streams management
//***************************************************************************

//---------------------------------------------------------------------------
void File__MultipleParsing::Streams_Finish()
{
    if (Parser.size()!=1)
        return;

    Parser[0]->Open_Buffer_Finalize();
    #if MEDIAINFO_TRACE
        Details=Parser[0]->Details;
    #endif //MEDIAINFO_TRACE
}

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File__MultipleParsing::Read_Buffer_Init()
{
    //Parsing
    for (size_t Pos=0; Pos<Parser.size(); Pos++)
    {
        //Parsing
        #if MEDIAINFO_TRACE
            Parser[Pos]->Init(Config, Details, Stream, Stream_More);
        #else //MEDIAINFO_TRACE
            Parser[Pos]->Init(Config, Stream, Stream_More);
        #endif //MEDIAINFO_TRACE
        Parser[Pos]->File_Name=File_Name;
        Parser[Pos]->Open_Buffer_Init(File_Size);
    }
}

//---------------------------------------------------------------------------
void File__MultipleParsing::Read_Buffer_Unsynched()
{
    //Parsing
    for (size_t Pos=0; Pos<Parser.size(); Pos++)
        Parser[Pos]->Open_Buffer_Unsynch();
}

//---------------------------------------------------------------------------
void File__MultipleParsing::Read_Buffer_Continue()
{
    //Parsing
    for (size_t Pos=0; Pos<Parser.size(); Pos++)
    {
        //Parsing
        Parser[Pos]->Open_Buffer_Continue(Buffer+Buffer_Offset, (size_t)Element_Size);
        if (File_Offset+Buffer_Size==File_Size)
            Parser[Pos]->Open_Buffer_Finalize();

        //Testing if the parser failed
        if (Parser[Pos]->Status[IsFinished] && !Parser[Pos]->Status[IsAccepted])
        {
            delete Parser[Pos];
            Parser.erase(Parser.begin()+Pos);
            Pos--; //for the next position

            if (Parser.empty())
            {
                File__Analyze* Temp=new File_Unknown(); Parser.push_back(Temp);
                Read_Buffer_Init();
            }
        }
        else
        {
            //If Parser is found, erasing all the other parsers
            if (Parser.size()>1 && Parser[Pos]->Status[IsAccepted])
            {
                File__Analyze* Temp=Parser[Pos];
                for (size_t To_Delete_Pos=0; To_Delete_Pos<Parser.size(); To_Delete_Pos++)
                    if (To_Delete_Pos!=Pos)
                        delete Parser[To_Delete_Pos]; //Parser[Pos]=NULL
                Parser.clear();
                Parser.push_back(Temp);
                Pos=0;
            }

            if (Parser.size()==1)
            {
                //Status
                if (!Status[IsAccepted] && Parser[Pos]->Status[IsAccepted])
                    Status[IsAccepted]=true;
                if (!Status[IsFilled] && Parser[Pos]->Status[IsFilled])
                    Status[IsFilled]=true;
                if (!Status[IsUpdated] && Parser[Pos]->Status[IsUpdated])
                    Status[IsUpdated]=true;
                if (!Status[IsFinished] && Parser[Pos]->Status[IsFinished])
                    Status[IsFinished]=true;

                //Positionning if requested
                if (Parser[0]->File_GoTo!=(int64u)-1)
                   File_GoTo=Parser[0]->File_GoTo;
            }
        }
    }
}

} //NameSpace
