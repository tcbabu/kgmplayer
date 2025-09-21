
void _img_initialise(DIG *G)
{
  int i;
  int EVGAX,EVGAY;
  kgDC *dc;
  dc=G->dc;
  EVGAX=G->x2 -G->x1+1;
  EVGAY=G->y2 - G->y1+1;
  for(i=0;i<10;i++)st_ptr[i]=0;
  dc->ln_style=0;
  dc->m_style=0;
  dc->ln_color = 0x0F;
  dc->c_bound =4;
  dc->t_color = 14;
  dc->t_pattern=15;
  dc->t_fill = 1;
  dc->t_font = 0;
  dc->t_bkgr = 0;
  dc->t_bodr = 0;
  dc->fil_color=0;
  dc->bod_width=11;
  dc->bod_color=1;
  dc->icpos = icposf0;dc->icxv=icxvf0;dc->icyv=icyvf0;dc->m_f=m_f0;
  dc->ln_width=1;
  dc->pr_txt=1;
  dc->cost = 1.0;
  dc->sint = 0.0;
  dc->txt_ht=20;
  dc->txt_wt=20;
  dc->txt_htx=20;
  dc->txt_wty=20;
  dc->txt_hty=20;
  dc->txt_wtx=20;
  dc->txt_w42 = dc->txt_wt/CFact;
  dc->txt_h42 = dc->txt_ht/CFact;
  dc->txt_sp=2;
  dc->txt_spy=2;
  dc->txt_spx=2;
  dc->v_x1=0;
  dc->v_y1=0;
  dc->v_x2=EVGAX;
  dc->v_y2=EVGAY;
  dc->w_x1=0;
  dc->w_y1=0;
  dc->w_x2=1.0;
  dc->w_y2=1.0;
  dc->u_x=EVGAX;
  dc->u_y=EVGAY;
  dc->V_x=EVGAX;
  dc->V_y=EVGAY;
  dc->D_x=0;
  dc->D_y=0;
  dc->ZBUFF=dc->SET3D=dc->TIFF=dc->PROJ=1;
  dc->DBL_BUF=0;
  dc->D_ON=0;
  wc->Clip = Dopen();
  wc->SBlist=Dopen();
  dc->EVGAX=EVGAX;
  dc->EVGAY=EVGAY;
  uiset_sup_clip_limits(wc,0,0,dc->EVGAX-1,dc->EVGAY-1);
  uiset_clip_limits(wc,0,0,dc->EVGAX-1,dc->EVGAY-1);
  for(i=0;i<10;i++) {
    dc->ln_ptn[i]=ln_ptn[i];
    dc->st_ptr[i]=st_ptr[i];
  }
}
void *kgInitImage(float width,float height,int mag) {
  int l;
  static entry=0;
  char flname[200],reviewfile[200];

  DIG *G;
  kgDC *dc;
  G = (DIG *) malloc(sizeof(DIG));
  dc = (kgDC *) malloc(sizeof(kgDC));
  G->wc = (kgWC *)malloc(sizeof(kgWC));
  G->dc = dc;
  G->x1=G->y1=0;
  G->x2 = (int)(width+0.5);
  G->y2 = (int)(height+0.5);
  _img_initialise(G);
  for(l=0;l<10;l++) dc->st_ptr[l]=0;
  G->B_min = 10400;
  G->BACK_UP=0;
  G->OPEN=1;
  G->Byte=0;
  G->R_Byte=0;
  G->R_max=0;
  G->D_ON=1;
  G->MAG=mag;
  G->img=NULL;
  G->rzimg=NULL;
  G->pixels=NULL;
  kgUserFrame(G,0.,0.,(float)(width),(float)(height));
  dc->ls_list=NULL;
  dc->No_of_lights=0;
  return G;
}
