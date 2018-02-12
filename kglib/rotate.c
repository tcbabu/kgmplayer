
static float TI[16] = {1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.,0.,0.,0.,0.,1.};
static float FM[16]=
  {
     1,0,0,0,
     0,1,0,0,
     0,0,1,0,
     0,0,0,1
  };
static float TX[16],TY[16],TZ[16];
static float TR[16],TM,T1;
static xstr,ystr,zstr;

static void init_matrix(void) {
     for(i=0;i<16;i++)
     {
      TR[i] = TI[i];
      TX[i] = TI[i];
      TY[i] = TI[i];
      TZ[i] = TI[i];
      TM[i] = TI[i];
      FM[i] = TI[i];
     }
}

static void mul_matrix(float *A,float *B,float *C)
 {

   C[0] = A[0]* B[0] + A[1]*B[4] + A[2]*B[8] + A[3]*B[12];
   C[1] = A[0]*B[1] + A[1]*B[5] + A[2]*B[9] + A[3]*B[13];
   C[2] = A[0]*B[2] + A[1]*B[6] + A[2]*B[10] + A[3]*B[14];
   C[3] = A[0]*B[3] + A[1]*B[7] + A[2]*B[11] + A[3]*B[15];
   C[4] = A[4]*B[0] + A[5]*B[4] + A[6]*B[8] + A[7] *B[12];
   C[5] = A[4]*B[1] + A[5]*B[5] + A[6]*B[9] + A[7]*B[13];
   C[6] = A[4]*B[2] + A[5]*B[6] + A[6]*B[10] + A[7]*B[14];
   C[7] = A[4]*B[3] + A[5]*B[7] + A[6]*B[11] + A[7]*B[15];
   C[8] = A[8]*B[0] + A[9]*B[4] + A[10]*B[8] + A[11]*B[12];
   C[9] = A[8]*B[1] + A[9]*B[5] + A[10]*B[9] + A[11]*B[13];
   C[10] = A[8]*B[2] + A[9]*B[6] + A[10]*B[10] + A[11]*B[14];
   C[11] = A[8]*B[3] + A[9]*B[7] + A[10]*B[11] + A[11]*B[15];
   C[12] = A[12]*B[0] + A[13]*B[4] + A[14]*B[8] + A[15]*B[12];
   C[13] = A[12]*B[1] + A[13]*B[5] + A[14]*B[9] + A[15]*B[13];
   C[14] = A[12]*B[2] + A[13]*B[6] + A[14]*B[10] + A[15]*B[14];
   C[15] = A[12]*B[3] + A[13]*B[7] + A[14]*B[11] + A[15]*B[15];
 }
  
static void transfrm(float *FM,float xp, float yp, float zp)
 {

   xstr = (xp * FM[0]) + (yp * FM[4]) + (zp * FM[8]) + FM[12];
   ystr = (xp * FM[1]) + (yp * FM[5]) + (zp * FM[9]) + FM[13];
   zstr = (xp * FM[2]) + (yp * FM[6]) + (zp * FM[10]) + FM[14];
   return;

 }

static void translate(float tx, float ty, float tz)
 {
    int i;
    TR[12] = -tx; TR[13] = -ty; TR[14] = -tz;
    mul_matrix(TM,TR,FM);
    for(i=0;i<16;i++) TM[i] = FM[i];
 }

static void rotatx(float ang)
 {
    int i;
    TX[5] = cos(ang); TX[6] = sin(ang); TX[9] = -(TX[6]);
    TX[10] = TX[5];
    mul_matrix(TM,TX,FM);
    for(i=0;i<16;i++) TM[i] = FM[i]; 
 }

static void rotaty(float ang)
 {
    int i;
   
    TY[0] = cos(ang); TY[2] = -(sin(ang)); TY[8] = sin(ang);
    TY[10] = TY[0];
    mul_matrix(TM,TY,FM); 
    for(i=0;i<16;i++) TM[i] = FM[i]; 
 }

static void rotatz(float ang)
 {
    int i;

    TZ[0] = cos(ang); TZ[1] = sin(ang); TZ[4] = -TZ[1];
    TZ[5] = TZ[0];
    mul_matrix(TM,TZ,FM); 
    for(i=0;i<16;i++) TM[i] = FM[i]; 
 }

static void rotate(unsigned char ch, float ang)
 {
    float rdg; 
    
    rdg = (M_PI * ang)/180;
    if (ch == 'X') rotatx(rdg);
    if (ch == 'Y') rotaty(rdg);
    if (ch == 'Z') rotatz(rdg);
 }
