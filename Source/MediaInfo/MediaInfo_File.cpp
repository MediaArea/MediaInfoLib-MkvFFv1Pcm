/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// How to:
// To add a new format,
// Fill includes, SelectFromExtension, ListFormats and LibraryIsModified
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

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#include "MediaInfo/Reader/Reader_File.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Multiple
#if defined(MEDIAINFO_MK_YES)
    #include "MediaInfo/Multiple/File_Mk.h"
#endif

#if defined(MEDIAINFO_UNKNOWN_YES)
    #include "MediaInfo/File_Unknown.h"
#endif
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
extern MediaInfo_Config Config;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
bool MediaInfo_Internal::SelectFromExtension (const String &Parser)
{
    CriticalSectionLocker CSL(CS);

    //Clear last value
    delete Info; Info=NULL;

    //Searching the right File_*
             if (0) {} //For #defines

    // Multiple
    #if defined(MEDIAINFO_MK_YES)
        else if (Parser==__T("Mk"))          Info=new File_Mk();
    #endif

    //No parser
        else
            return false;

    return true;
}

//---------------------------------------------------------------------------
#if !defined(MEDIAINFO_READER_NO)
int MediaInfo_Internal::ListFormats(const String &File_Name)
{
    // Multiple
    #if defined(MEDIAINFO_MK_YES)
        delete Info; Info=new File_Mk();                 if (((Reader_File*)Reader)->Format_Test_PerParser(this, File_Name)>0) return 1;
    #endif

    // Default (empty)
        delete Info; Info=new File_Unknown();            if (((Reader_File*)Reader)->Format_Test_PerParser(this, File_Name)>0) return 1;
    return 0;
}
#endif //!defined(MEDIAINFO_READER_NO)

//---------------------------------------------------------------------------
bool MediaInfo_Internal::LibraryIsModified ()
{
        return false;
}

//---------------------------------------------------------------------------
void MediaInfo_Internal::CreateDummy (const String&)
{
    #if defined(MEDIAINFO_DUMMY_YES)
        Info=new File_Dummy();
        ((File_Dummy*)Info)->KindOfDummy=Value;
    #endif
}

} //NameSpace
