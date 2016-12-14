AUTOMAKE_OPTIONS = foreign subdir-objects

lib_LTLIBRARIES = lib@MediaInfoLib_LibName@.la
lib@MediaInfoLib_LibName@_la_SOURCES = \
                       ../../../Source/MediaInfo/File__Analyze.cpp \
                       ../../../Source/MediaInfo/File__Analyze_Buffer.cpp \
                       ../../../Source/MediaInfo/File__Analyze_Buffer_MinimizeSize.cpp \
                       ../../../Source/MediaInfo/File__Analyze_Streams.cpp \
                       ../../../Source/MediaInfo/File__Analyze_Streams_Finish.cpp \
                       ../../../Source/MediaInfo/File__Analyze_Element.cpp \
                       ../../../Source/MediaInfo/File__Base.cpp \
                       ../../../Source/MediaInfo/File__MultipleParsing.cpp \
                       ../../../Source/MediaInfo/File_Unknown.cpp \
                       ../../../Source/MediaInfo/MediaInfo.cpp \
                       ../../../Source/MediaInfo/MediaInfo_Config.cpp \
                       ../../../Source/MediaInfo/MediaInfo_Config_Automatic.cpp \
                       ../../../Source/MediaInfo/MediaInfo_Config_MediaInfo.cpp \
                       ../../../Source/MediaInfo/MediaInfo_File.cpp \
                       ../../../Source/MediaInfo/MediaInfo_Inform.cpp \
                       ../../../Source/MediaInfo/MediaInfo_Internal.cpp \
                       ../../../Source/MediaInfo/MediaInfoList.cpp \
                       ../../../Source/MediaInfo/MediaInfoList_Internal.cpp \
                       ../../../Source/MediaInfo/TimeCode.cpp \
                       ../../../Source/MediaInfo/Audio/File_Pcm.cpp \
                       ../../../Source/MediaInfo/Multiple/File_Mk.cpp \
                       ../../../Source/MediaInfo/Reader/Reader_Directory.cpp \
                       ../../../Source/MediaInfo/Reader/Reader_File.cpp \
                       ../../../Source/MediaInfo/Reader/Reader_libcurl.cpp \
                       ../../../Source/MediaInfo/Video/File_Ffv1.cpp \
                       ../../../Source/MediaInfoDLL/MediaInfoDLL.cpp

@MediaInfoLib_LibName@includedir = $(includedir)/MediaInfo
@MediaInfoLib_LibName@include_HEADERS = \
 ../../../Source/MediaInfo/MediaInfo.h \
 ../../../Source/MediaInfo/MediaInfoList.h \
 ../../../Source/MediaInfo/MediaInfo_Const.h \
 ../../../Source/MediaInfo/MediaInfo_Events.h

@MediaInfoLib_LibName@DLLincludedir = $(includedir)/MediaInfoDLL
@MediaInfoLib_LibName@DLLinclude_HEADERS = \
 ../../../Source/MediaInfoDLL/MediaInfoDLL.h \
 ../../../Source/MediaInfoDLL/MediaInfoDLL_Static.h

if COMPILE_AES
lib@MediaInfoLib_LibName@_la_SOURCES += \
                       ../../../Source/ThirdParty/aes-gladman/aes_modes.c \
                       ../../../Source/ThirdParty/aes-gladman/aescrypt.c \
                       ../../../Source/ThirdParty/aes-gladman/aeskey.c \
                       ../../../Source/ThirdParty/aes-gladman/aestab.c
endif

if COMPILE_MD5
lib@MediaInfoLib_LibName@_la_SOURCES += \
                       ../../../Source/ThirdParty/md5/md5.c
endif

if COMPILE_SHA1
lib@MediaInfoLib_LibName@_la_SOURCES += \
                       ../../../Source/ThirdParty/sha1-gladman/sha1.c
endif

if COMPILE_SHA2
lib@MediaInfoLib_LibName@_la_SOURCES += \
                       ../../../Source/ThirdParty/sha2-gladman/sha2.c
endif

if COMPILE_HMAC
lib@MediaInfoLib_LibName@_la_SOURCES += \
                       ../../../Source/ThirdParty/hmac-gladman/hmac.c \
                       ../../../Source/ThirdParty/hmac-gladman/pwd2key.c
endif

if COMPILE_TINYXML2
lib@MediaInfoLib_LibName@_la_SOURCES += \
                       ../../../Source/ThirdParty/tinyxml2/tinyxml2.cpp
endif

lib@MediaInfoLib_LibName@_la_LDFLAGS     = -no-undefined -version-info 0:0:0

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libmediainfo.pc

AM_CPPFLAGS = -I../../../Source -I../../../../ZenLib/Source

TESTS_DIR = test

SUBDIRS = . $(TESTS_DIR)

REGRESSION_FILES = $(TESTS_DIR)/Files
clone_regression_files: $(REGRESSION_FILES)

$(REGRESSION_FILES):
	git clone https://github.com/MediaArea/MediaArea-RegressionTestingFiles $(TESTS_DIR)/Files

