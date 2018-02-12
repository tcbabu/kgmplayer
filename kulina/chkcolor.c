#include <kulina.h>
int gray=192;
int red,green,blue;
void *logo(int l,int w){
  void *fid;
  void *logoimg=NULL;
  fid = kgInitImage(l,w,8);
  kgUserFrame(fid,0.,0.,(float)l,(float)w);
  kgChangeColor(fid,170,red,green,blue);
  kgRoundedRectangleFill(fid,l*0.5,w*0.5,(float)l,(float)w,0,170,0.1);
  logoimg = kgGetResizedImage(fid);
  kgWriteImage(logoimg,"Color.png");
  kgCloseImage(fid);
  return logoimg;
}
int main(int argc,char *argv[]){
  int l=600,w=600;
  sscanf(argv[1],"%d",&red);
  sscanf(argv[2],"%d",&green);
  sscanf(argv[3],"%d",&blue);
  logo(l,w);
}
