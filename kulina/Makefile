include config.mak
#CC	=g++ -pthread
#CC	=cc -pthread -fPIC 
CC	=cc -g -pthread -fPIC  $(X11_CFLAGS) -I$(KULINA)/include
kgMplayer.a	: kgMplayer.o kgMplayerCallbacks.o main.o FullScreen.o \
                  FullScreenCallbacks.o main.o ControlsCallbacks.o \
	        Controls.o AControls.o Rip.o Manage.o ManageCallbacks.o\
	PlayList.o PlayListCallbacks.o Mwin.o MwinCallbacks.o \
	 url.o urlCallbacks.o monitor.o monitorCallbacks.o \
	 tools.o toolsCallbacks.o Video.o VideoCallbacks.o \
	 audiodelay.o audiodelayCallbacks.o \
	 AudioTime.o AudioTimeCallbacks.o ToolBox.o ToolBoxCallbacks.o \
	 AudioConvert.o AudioConvertCallbacks.o \
	 Range.o RangeCallbacks.o \
	 EnVolume.o EnVolumeCallbacks.o \
	 ConvertVideo.o ConvertVideoCallbacks.o \
	 Vasp.o VaspCallbacks.o \
	 Vsize.o VsizeCallbacks.o \
	 Vrange.o VrangeCallbacks.o \
	 VideoJoin.o VideoJoinCallbacks.o \
	 AudioJoin.o AudioJoinCallbacks.o \
	 AddAudio.o AddAudioCallbacks.o \
	 MixAudio.o MixAudioCallbacks.o \
	 MakeKaraoke.o MakeKaraokeCallbacks.o \
	 AddSilence.o AddSilenceCallbacks.o \
	 MakeCuts.o MakeCutsCallbacks.o \
	 MonitorJoin.o MonitorJoinCallbacks.o \
	 kgGetFolder.o kgGetFolderCallbacks.o \
	 subtitle.o subtitleCallbacks.o \
	 HdmiAudio.o HdmiAudioCallbacks.o \
	 Subtitleoff.o SubtitleoffCallbacks.o \
	 DirPlay.o DirPlayCallbacks.o \
	 GetSinfo.o GetSinfoCallbacks.o
	 ar -r  kgMplayer.a  kgMplayer.o kgMplayerCallbacks.o main.o \
	 FullScreen.o FullScreenCallbacks.o  \
	ControlsCallbacks.o Controls.o AControls.o Rip.o \
	Manage.o ManageCallbacks.o \
	PlayList.o PlayListCallbacks.o Mwin.o MwinCallbacks.o \
	 url.o urlCallbacks.o monitor.o monitorCallbacks.o \
	 tools.o toolsCallbacks.o Video.o VideoCallbacks.o  \
	 audiodelay.o audiodelayCallbacks.o \
	 AudioTime.o AudioTimeCallbacks.o \
	 ToolBox.o ToolBoxCallbacks.o \
	 AudioConvert.o AudioConvertCallbacks.o \
	 Range.o RangeCallbacks.o \
	 EnVolume.o EnVolumeCallbacks.o \
	 ConvertVideo.o ConvertVideoCallbacks.o \
	 Vasp.o VaspCallbacks.o \
	 Vsize.o VsizeCallbacks.o \
	 Vrange.o VrangeCallbacks.o \
	 VideoJoin.o VideoJoinCallbacks.o \
	 AudioJoin.o AudioJoinCallbacks.o \
	 AddAudio.o AddAudioCallbacks.o \
	 MixAudio.o MixAudioCallbacks.o \
	 MakeKaraoke.o MakeKaraokeCallbacks.o \
	 AddSilence.o AddSilenceCallbacks.o \
	 MakeCuts.o MakeCutsCallbacks.o \
	 MonitorJoin.o MonitorJoinCallbacks.o \
	 kgGetFolder.o kgGetFolderCallbacks.o \
	 subtitle.o subtitleCallbacks.o \
	 HdmiAudio.o HdmiAudioCallbacks.o \
	 Subtitleoff.o SubtitleoffCallbacks.o \
	 DirPlay.o DirPlayCallbacks.o \
	 GetSinfo.o GetSinfoCallbacks.o \
	../mplayer/ffmpeg/ffmpeg.o ../mplayer/ffmpeg/cmdutils.o \
	../mplayer/ffmpeg/ffmpeg_filter.o ../mplayer/ffmpeg/ffmpeg_opt.o
kgMplayer.o	: kgMplayer.c kulina.h
	 $(CC) -c kgMplayer.c
kgMplayerCallbacks.o	: kgMplayerCallbacks.c 
	 $(CC) -c kgMplayerCallbacks.c
main.o	: main.c  kulina.h ConvertData.h
	 $(CC) -c main.c
FullScreen.o	: FullScreen.c kulina.h
	 $(CC) -c FullScreen.c
FullScreenCallbacks.o	: FullScreenCallbacks.c kulina.h
	 $(CC) -c FullScreenCallbacks.c
Controls.o	: Controls.c kulina.h cimages.c
	 $(CC) -c Controls.c
AControls.o	: AControls.c kulina.h cimages.c
	 $(CC) -c AControls.c
ControlsCallbacks.o	: ControlsCallbacks.c kulina.h
	 $(CC) -c ControlsCallbacks.c
Rip.o	: Rip.c 
	 $(CC) -c Rip.c
Manage.o	: Manage.c kulina.h
	 $(CC) -c Manage.c
ManageCallbacks.o	: ManageCallbacks.c kulina.h
	 $(CC) -c ManageCallbacks.c
PlayList.o	: PlayList.c kulina.h
	 $(CC) -c PlayList.c
PlayListCallbacks.o	: PlayListCallbacks.c kulina.h ConvertData.h
	 $(CC) -c PlayListCallbacks.c
AddSilence.o	: AddSilence.c kulina.h ConvertData.h
	 $(CC) -c AddSilence.c
AddSilenceCallbacks.o	: AddSilenceCallbacks.c kulina.h ConvertData.h
	 $(CC) -c AddSilenceCallbacks.c
MakeCuts.o	: MakeCuts.c kulina.h ConvertData.h
	 $(CC) -c MakeCuts.c
MakeCutsCallbacks.o	: MakeCutsCallbacks.c kulina.h ConvertData.h
	 $(CC) -c MakeCutsCallbacks.c
MonitorJoin.o   : MonitorJoin.c
	 $(CC) -c MonitorJoin.c
MonitorJoinCallbacks.o  : MonitorJoinCallbacks.c
	 $(CC) -c MonitorJoinCallbacks.c
AddAudio.o	: AddAudio.c ConvertData.h
	 $(CC) -c AddAudio.c
AddAudioCallbacks.o	: AddAudioCallbacks.c ConvertData.h
	 $(CC) -c AddAudioCallbacks.c
MixAudio.o	: MixAudio.c ConvertData.h
	 $(CC) -c MixAudio.c
MixAudioCallbacks.o	: MixAudioCallbacks.c ConvertData.h
	 $(CC) -c MixAudioCallbacks.c
AudioConvert.o	: AudioConvert.c ConvertData.h
	 $(CC) -c AudioConvert.c
AudioConvertCallbacks.o	: AudioConvertCallbacks.c ConvertData.h
	 $(CC) -c AudioConvertCallbacks.c
AudioJoin.o	: AudioJoin.c ConvertData.h
	 $(CC) -c AudioJoin.c
AudioJoinCallbacks.o	: AudioJoinCallbacks.c ConvertData.h
	 $(CC) -c AudioJoinCallbacks.c
ConvertVideo.o	: ConvertVideo.c ConvertData.h
	 $(CC) -c ConvertVideo.c
ConvertVideoCallbacks.o	: ConvertVideoCallbacks.c ConvertData.h
	 $(CC) -c ConvertVideoCallbacks.c
EnVolume.o	: EnVolume.c ConvertData.h
	 $(CC) -c EnVolume.c
EnVolumeCallbacks.o	: EnVolumeCallbacks.c ConvertData.h
	 $(CC) -c EnVolumeCallbacks.c
MakeKaraoke.o	: MakeKaraoke.c ConvertData.h
	 $(CC) -c MakeKaraoke.c
MakeKaraokeCallbacks.o	: MakeKaraokeCallbacks.c ConvertData.h
	 $(CC) -c MakeKaraokeCallbacks.c
Range.o	: Range.c ConvertData.h
	 $(CC) -c Range.c
RangeCallbacks.o	: RangeCallbacks.c ConvertData.h
	 $(CC) -c RangeCallbacks.c
ToolBox.o	: ToolBox.c ConvertData.h
	 $(CC) -c ToolBox.c
ToolBoxCallbacks.o	: ToolBoxCallbacks.c ConvertData.h
	 $(CC) -c ToolBoxCallbacks.c
Vasp.o	: Vasp.c ConvertData.h
	 $(CC) -c Vasp.c
VaspCallbacks.o	: VaspCallbacks.c ConvertData.h
	 $(CC) -c VaspCallbacks.c
Vrange.o	: Vrange.c ConvertData.h
	 $(CC) -c Vrange.c
VrangeCallbacks.o	: VrangeCallbacks.c ConvertData.h
	 $(CC) -c VrangeCallbacks.c
Vsize.o	: Vsize.c ConvertData.h
	 $(CC) -c Vsize.c
Vsizellbacks.o	: VsizeCallbacks.c ConvertData.h
	 $(CC) -c VsizeCallbacks.c
VideoJoin.o	: VideoJoin.c ConvertData.h
	 $(CC) -c VideoJoin.c
VideoJoinllbacks.o	: VideoJoinCallbacks.c ConvertData.h
	 $(CC) -c VideoJoinCallbacks.c
GetSinfo.o	: GetSinfo.c 
	 $(CC) -c GetSinfo.c
GetSinfollbacks.o	: GetSinfoCallbacks.c
	 $(CC) -c GetSinfoCallbacks.c


clean	:  
	   rm -f *.o kgMplayer.a
install	: kgMplayer.a
	 cp kgMplayer.a $(KULINA)/lib/kgmplayer.a