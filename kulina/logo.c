#include <kulina.h>
void *logo(int l,int w){
  void *fid;
  void *logoimg=NULL;
  fid = kgInitImage(l,w,8);
  kgUserFrame(fid,0.,0.,(float)l,(float)w);
  kgTextAngle(fid,90.);
  kgTextSize(fid,l*0.5,l*0.3,0.0);
  kgTextFont(fid,23);
  kgWriteText(fid,(char *)"kgMplayer");
  logoimg = kgGetResizedImage(fid);
  kgCloseImage(fid);
  return logoimg;
}
