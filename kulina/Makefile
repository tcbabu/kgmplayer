include config.mak
#CC	=g++ -pthread
#CC	=cc -pthread -fPIC 
CC	=gcc -pthread -fPIC  $(X11_CFLAGS) -I$(KULINA)/include
FFMPEGOBJS	=$(foreach part, ../mplayer/ffmpeg/fftools, $(wildcard $(addprefix $(part)/,*.[o])))
kgMplayer.a	: kgMplayer.o kgMplayerCallbacks.o main.o FullScreen.o \
                  FullScreenCallbacks.o main.o ControlsCallbacks.o \
	        Controls.o AControls.o Rip.o Manage.o ManageCallbacks.o\
	PlayList.o PlayListCallbacks.o Mwin.o MwinCallbacks.o \
	 url.o urlCallbacks.o monitor.o monitorCallbacks.o \
	 tools.o toolsCallbacks.o Video.o VideoCallbacks.o \
	 audiodelay.o audiodelayCallbacks.o \
	 AudioTime.o AudioTimeCallbacks.o ToolBox.o ToolBoxCallbacks.o \
	 HelperMsg.o \
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
	 MonitorVol.o MonitorVolCallbacks.o \
	 kgGetFolder.o kgGetFolderCallbacks.o \
	 subtitle.o subtitleCallbacks.o \
	 HdmiAudio.o HdmiAudioCallbacks.o \
	 Subtitleoff.o SubtitleoffCallbacks.o \
	 DirPlay.o DirPlayCallbacks.o \
	 GetSinfo.o GetSinfoCallbacks.o \
	 images2videos.o imgs2v.o imgs2vCallbacks.o \
	 kgGetFiles.o kgGetFilesCallbacks.o \
	 normalize.o normalizeCallbacks.o \
	 Normalise.o NormaliseCallbacks.o \
	 Loudnorm.o LoudnormCallbacks.o \
	 AudioSpeed.o AudioSpeedCallbacks.o \
	 EnhanceAudio.o EnhanceAudioCallbacks.o \
	 EnhanceSpeech.o EnhanceSpeechCallbacks.o \
	 RangeSelect.o RangeSelectCallbacks.o \
	 RangeReject.o RangeRejectCallbacks.o \
	 LowPass.o LowPassCallbacks.o \
	 HighPass.o HighPassCallbacks.o \
	 Silence.o SilenceCallbacks.o \
	 acap.o acapCallbacks.o \
	 CaptureAudio.o CaptureAudioCallbacks.o \
	 Vtobw.o VtobwCallbacks.o \
	 Vflip.o VflipCallbacks.o \
	 Hflip.o HflipCallbacks.o \
	 slice.o sliceCallbacks.o \
	 ToolHelp.o ToolHelpCallbacks.o \
	 HelpButn.o HelpButnCallbacks.o \
	 ImageBox.o ImageBoxCallbacks.o \
	 VideoInfo.o VideoInfoCallbacks.o \
	 ChangeSize.o ChangeSizeCallbacks.o \
	 Rotate90.o Rotate90Callbacks.o \
	 SidebySide.o SidebySideCallbacks.o \
	 VideoSpeed.o VideoSpeedCallbacks.o \
	 VideoReverse.o VideoReverseCallbacks.o \
	 AudioReverse.o AudioReverseCallbacks.o \
	 topandbottom.o topandbottomCallbacks.o \
	 Overlay.o OverlayCallbacks.o \
	 Boverlay.o BoverlayCallbacks.o \
	 Crop.o CropCallbacks.o \
	 ArrangeVideo.o ArrangeVideoCallbacks.o \
	 PicFrame.o PicFrameCallbacks.o \
	 VR.o VRCallbacks.o \
	 GetCropArea.o GetCropAreaCallbacks.o \
	 AddStill.o AddStillCallbacks.o \
	 Runbackground.o \
	 utilfunctions.o \
	 Helper.o HelperCallbacks.o createhelpimage.o \
	 ReorderImages.o ReorderImagesCallbacks.o
	 mv ../ffmpeg/fftools/ffmain.o ../ffmpeg/
	 touch ../ffmpeg/fftools/ffmain.o
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
	 HelperMsg.o \
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
	 MonitorVol.o MonitorVolCallbacks.o \
	 kgGetFolder.o kgGetFolderCallbacks.o \
	 subtitle.o subtitleCallbacks.o \
	 HdmiAudio.o HdmiAudioCallbacks.o \
	 Subtitleoff.o SubtitleoffCallbacks.o \
	 DirPlay.o DirPlayCallbacks.o \
	 GetSinfo.o GetSinfoCallbacks.o \
	 images2videos.o imgs2v.o imgs2vCallbacks.o \
	 kgGetFiles.o kgGetFilesCallbacks.o \
	 normalize.o normalizeCallbacks.o \
	 Normalise.o NormaliseCallbacks.o \
	 Loudnorm.o LoudnormCallbacks.o \
	 AudioSpeed.o AudioSpeedCallbacks.o \
	 EnhanceAudio.o EnhanceAudioCallbacks.o \
	 EnhanceSpeech.o EnhanceSpeechCallbacks.o \
	 RangeSelect.o RangeSelectCallbacks.o \
	 RangeReject.o RangeRejectCallbacks.o \
	 LowPass.o LowPassCallbacks.o \
	 HighPass.o HighPassCallbacks.o \
	 Silence.o SilenceCallbacks.o \
	 acap.o acapCallbacks.o \
	 CaptureAudio.o CaptureAudioCallbacks.o \
	 Vtobw.o VtobwCallbacks.o \
	 Vflip.o VflipCallbacks.o \
	 Hflip.o HflipCallbacks.o \
	 slice.o sliceCallbacks.o \
	 ToolHelp.o ToolHelpCallbacks.o \
	 HelpButn.o HelpButnCallbacks.o \
	 ImageBox.o ImageBoxCallbacks.o \
	 Runbackground.o \
	 utilfunctions.o \
	 Helper.o HelperCallbacks.o createhelpimage.o \
	 ReorderImages.o ReorderImagesCallbacks.o \
	 VideoInfo.o VideoInfoCallbacks.o \
	 ChangeSize.o ChangeSizeCallbacks.o \
	 Rotate90.o Rotate90Callbacks.o \
	 SidebySide.o SidebySideCallbacks.o \
	 VideoSpeed.o VideoSpeedCallbacks.o \
	 VideoReverse.o VideoReverseCallbacks.o \
	 AudioReverse.o AudioReverseCallbacks.o \
	 topandbottom.o topandbottomCallbacks.o \
	 Overlay.o OverlayCallbacks.o \
	 Boverlay.o BoverlayCallbacks.o \
	 Crop.o CropCallbacks.o \
	 ArrangeVideo.o ArrangeVideoCallbacks.o \
	 PicFrame.o PicFrameCallbacks.o \
	 VR.o VRCallbacks.o \
	 GetCropArea.o GetCropAreaCallbacks.o \
	 AddStill.o AddStillCallbacks.o \
	 $(FFMPEGOBJS)
	 echo $(FFMPEGOBJS)
	 mv ../ffmpeg/ffmain.o ../ffmpeg/fftools/
kgMplayer.o	: kgMplayer.c 
	 $(CC) -c kgMplayer.c
kgMplayerCallbacks.o	: kgMplayerCallbacks.c 
	 $(CC) -c kgMplayerCallbacks.c
main.o	: main.c   ConvertData.h
	 $(CC) -c main.c
FullScreen.o	: FullScreen.c 
	 $(CC) -c FullScreen.c
FullScreenCallbacks.o	: FullScreenCallbacks.c 
	 $(CC) -c FullScreenCallbacks.c
Controls.o	: Controls.c  cimages.c
	 $(CC) -c Controls.c
AControls.o	: AControls.c  cimages.c
	 $(CC) -c AControls.c
ControlsCallbacks.o	: ControlsCallbacks.c 
	 $(CC) -c ControlsCallbacks.c
Rip.o	: Rip.c 
	 $(CC) -c Rip.c
Manage.o	: Manage.c 
	 $(CC) -c Manage.c
ManageCallbacks.o	: ManageCallbacks.c 
	 $(CC) -c ManageCallbacks.c
PlayList.o	: PlayList.c 
	 $(CC) -c PlayList.c
PlayListCallbacks.o	: PlayListCallbacks.c  ConvertData.h
	 $(CC) -c PlayListCallbacks.c
AddSilence.o	: AddSilence.c  ConvertData.h
	 $(CC) -c AddSilence.c
AddSilenceCallbacks.o	: AddSilenceCallbacks.c  ConvertData.h
	 $(CC) -c AddSilenceCallbacks.c
MakeCuts.o	: MakeCuts.c  ConvertData.h
	 $(CC) -c MakeCuts.c
MakeCutsCallbacks.o	: MakeCutsCallbacks.c  ConvertData.h
	 $(CC) -c MakeCutsCallbacks.c
MonitorJoin.o   : MonitorJoin.c
	 $(CC) -c MonitorJoin.c
MonitorJoinCallbacks.o  : MonitorJoinCallbacks.c
	 $(CC) -c MonitorJoinCallbacks.c
MonitorVol.o   : MonitorVol.c
	 $(CC) -c MonitorVol.c
MonitorVolCallbacks.o  : MonitorVolCallbacks.c
	 $(CC) -c MonitorVolCallbacks.c
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
ToolBox.o	: ToolBox.c ConvertData.h ToolGroup.h
	 $(CC) -c ToolBox.c
ToolBoxCallbacks.o	: ToolBoxCallbacks.c ConvertData.h ToolGroup.h
	 $(CC) -c ToolBoxCallbacks.c
Runbackground.o	: Runbackground.c 
	 $(CC) -c Runbackground.c
utilfunctions.o	: utilfunctions.c 
	 $(CC) -c utilfunctions.c
HelperMag.o	: HelperMag.c
	 $(CC) -c HelperMag.c
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
imgs2v.o        : imgs2v.c
		$(CC) -c imgs2v.c
imgs2vCallbacks.o       : imgs2vCallbacks.c  images2videos.h
		$(CC) -c imgs2vCallbacks.c
image2video.o   : images2videos.c  images2videos.h
		$(CC) -c images2videos.c
kgGetFiles.o	: kgGetFiles.c
	$(CC) -c kgGetFiles.c
kgGetFilesCallbacks.o	: kgGetFilesCallbacks.c
	$(CC) -c kgGetFilesCallbacks.c
normalize.o	: normalize.c
	 	$(CC) -c normalize.c
normalizeCallbacks.o	: normalizeCallbacks.c
		$(CC) -c normalizeCallbacks.c
Normalise.o		: Normalise.c
	 	$(CC) -c Normalise.c
NormaliseCallbacks.o		: NormaliseCallbacks.c
		$(CC) -c NormaliseCallbacks.c
Loudnorm.o		: Loudnorm.c
	 	$(CC) -c Loudnorm.c
LoudnormCallbacks.o		: LoudnormCallbacks.c
		$(CC) -c LoudnormCallbacks.c
AudioSpeed.o		: AudioSpeed.c
	 	$(CC) -c AudioSpeed.c
AudioSpeedCallbacks.o		: AudioSpeedCallbacks.c
		$(CC) -c AudioSpeedCallbacks.c
VideoSpeed.o		: VideoSpeed.c
	 	$(CC) -c VideoSpeed.c
VideoSpeedCallbacks.o		: VideoSpeedCallbacks.c
		$(CC) -c VideoSpeedCallbacks.c
VideoReverse.o		: VideoReverse.c
	 	$(CC) -c VideoReverse.c
VideoReverseCallbacks.o		: VideoReverseCallbacks.c
		$(CC) -c VideoReverseCallbacks.c
AudioReverse.o		: AudioReverse.c
	 	$(CC) -c AudioReverse.c
AudioReverseCallbacks.o		: AudioReverseCallbacks.c
		$(CC) -c AudioReverseCallbacks.c
EnhanceAudio.o		: EnhanceAudio.c
	 	$(CC) -c EnhanceAudio.c
EnhanceAudioCallbacks.o		: EnhanceAudioCallbacks.c
		$(CC) -c EnhanceAudioCallbacks.c
EnhanceSpeech.o		: EnhanceSpeech.c
	 	$(CC) -c EnhanceSpeech.c
EnhanceSpeechCallbacks.o		: EnhanceSpeechCallbacks.c
		$(CC) -c EnhanceSpeechCallbacks.c
RangeSelect.o		: RangeSelect.c
	 	$(CC) -c RangeSelect.c
RangeSelectCallbacks.o		: RangeSelectCallbacks.c
		$(CC) -c RangeSelectCallbacks.c
RangeReject.o		: RangeReject.c
	 	$(CC) -c RangeReject.c
RangeRejectCallbacks.o		: RangeRejectCallbacks.c
		$(CC) -c RangeRejectCallbacks.c
LowPass.o		: LowPass.c
	 	$(CC) -c LowPass.c
LowPassCallbacks.o		: LowPassCallbacks.c
		$(CC) -c LowPassCallbacks.c
HighPass.o		: HighPass.c
	 	$(CC) -c HighPass.c
HighPassCallbacks.o		: HighPassCallbacks.c
		$(CC) -c HighPassCallbacks.c
Silence.o		: Silence.c
	 	$(CC) -c Silence.c
SilenceCallbacks.o		: SilenceCallbacks.c
		$(CC) -c SilenceCallbacks.c
acap.o		: acap.c
	 	$(CC) -c acap.c
acapCallbacks.o		: acapCallbacks.c
	 	$(CC) -c acapCallbacks.c
CaptureAudio.o		: CaptureAudio.c
	 	$(CC) -c CaptureAudio.c
CaptureAudoCallbacks.o		: CaptureAudoCallbacks.c
	 	$(CC) -c CaptureAudoCallbacks.c
Vtobw.o		: Vtobw.c
		 $(CC) -c Vtobw.c
VtobwCallbacks.o	: VtobwCallbacks.c
		$(CC) -c VtobwCallbacks.c
Vflip.o		: Vflip.c
		 $(CC) -c Vflip.c
VflipCallbacks.o	: VflipCallbacks.c
		$(CC) -c VflipCallbacks.c
Hflip.o		: Hflip.c
		 $(CC) -c Hflip.c
HflipCallbacks.o	: HflipCallbacks.c
		$(CC) -c HflipCallbacks.c
slice.o		: slice.c
		 $(CC) -c slice.c
sliceCallbacks.o	: sliceCallbacks.c
		$(CC) -c sliceCallbacks.c
Helper.o		: Helper.c
		 $(CC) -c Helper.c
ToolHelp.o		: ToolHelp.c
		 $(CC) -c ToolHelp.c
ToolHelpCallbacks.o	: ToolHelpCallbacks.c
		$(CC) -c ToolHelpCallbacks.c
HelpButn.o		: HelpButn.c
		 $(CC) -c HelpButn.c
HelpButnCallbacks.o	: HelpButnCallbacks.c
		$(CC) -c HelpButnCallbacks.c
ImageBox.o		: ImageBox.c
		 $(CC) -c ImageBox.c
ImageBoxCallbacks.o	: ImageBoxCallbacks.c
		$(CC) -c ImageBoxCallbacks.c
HelperCallbacks.o	: HelperCallbacks.c
		$(CC) -c HelperCallbacks.c
createhelpimage.o	: createhelpimage.c
		$(CC) -c createhelpimage.c
ReorderImages.o	: ReorderImages.c
		$(CC) -c ReorderImages.c
ReorderImagesCallbacks.o	: ReorderImagesCallbacks.c
		$(CC) -c ReorderImagesCallbacks.c
VideoInfo.o		: VideoInfo.c
		 $(CC) -c VideoInfo.c
ChangeSize.o		: ChangeSize.c
		 $(CC) -c ChangeSize.c
Rotate90.o		: Rotate90.c
		 $(CC) -c Rotate90.c
SidebySide.o		: SidebySide.c
		 $(CC) -c SidebySide.c
topandbottom.o		: topandbottom.c
		 $(CC) -c topandbottom.c
Overlay.o		: Overlay.c
		 $(CC) -c Overlay.c
Boverlay.o		: Boverlay.c
		 $(CC) -c Boverlay.c
BoverlayCallbacks.o		: BoverlayCallbacks.c
		 $(CC) -c BoverlayCallbacks.c
Crop.o		: Crop.c
		 $(CC) -c Crop.c
ArrangeVideo.o		: ArrangeVideo.c
		 $(CC) -c ArrangeVideo.c
PicFrame.o		: PicFrame.c
		 $(CC) -c PicFrame.c
VR.o		: VR.c
		 $(CC) -c VR.c
GetCropArea.o		: GetCropArea.c
		 $(CC) -c GetCropArea.c
AddStill.o		: AddStill.c
		 $(CC) -c AddStill.c

clean	:  
	   rm -f *.o kgMplayer.a
install	: kgMplayer.a
	 cp kgMplayer.a $(KULINA)/lib/kgmplayer.a
