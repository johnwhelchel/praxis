/* Written by Jens Kruse Andersen */
/* Search vampire numbers (not prime) */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#if 0
typedef unsigned __int64 int64; /* Borland C++ 64 bit int - note two underscores*/
#else
typedef unsigned long long int64; /* 64 bit int = long long in 1999 standard C? */
#endif

/* intn should be unsigned at least n-bit integer */
typedef unsigned char int8;
typedef unsigned int  int16;
typedef unsigned long int32;
typedef unsigned int intf; /* intf = fastest int at least 16 bit */

typedef int32 digtype;

#define million  1000000
#define digitmax 1000000
#define digitsmax 6
digtype dig[digitmax+1];
int64 dig64[digitmax+1];

void initdigit(void) {
  int32 d,i,d0,digit;
  int64 dsquare;

  fprintf(stderr, "Initdigit..");
  for (d=0;d<digitmax;d++) {
    dig[d]=0;
    d0=d;
    for (i=0;i<digitsmax;i++) {
      digit=d0%10;
      dig64[d]+=((int64)1<<(digit*4));
      dig[d]+=((int64)1<<(digit*3)); /* was *4 with 64-bit dig */
      d0/=10;
    }
  }
  fprintf(stderr," OK\n");
}

#define maxd 1000000
int32 maxdi=maxd,mine;
int64 minde=(int64)maxd*maxd/10;
int32 dby10;
int32 vampcount=0;

int32 step,step0,step1;
int32 d,e;
int32 emod9[]={0,10,2,6,10,8,3,10,5}; /* 10 means no solution */
int64 de;
digtype digd0,digd,digd_e1,digde2,dige1,
        digd_e1_sub_digde2,digde3,digd_e1_sub_digde2_sub_digde3;
int32 de0,de1,de2,de3;
int32 e0,e1,maxdtest=maxd/10+(maxd-maxd/10)/20;
#define power10 10000

void searchvampadd12(void) { /* search 12-digit vampire numbers */

  for (d=maxd/10; d<maxd; d++) {
/*printf(".");*/
    dby10=(d%10==0);
    if ((int64)d*d>=(int64)maxd*maxd/10) mine=d;
    else mine=(int64)maxd*maxd/10/d;
    if (emod9[d%9]==10) continue;
    mine+= ((emod9[d%9])-(mine%9)+9)%9 -9;

    step=9*d;
    step0=step%power10;
    step1=step/power10;

    de=(int64)d*mine;
    de0=de%power10;
    de/=power10;
    de1=de%power10;
    de2=de/power10;

    e0=mine%power10;
    e1=mine/power10;

    digd=dig[d];
    dige1=dig[e1];
    digd_e1=digd+dige1;
    digde2=dig[de2];
    digd_e1_sub_digde2=digd_e1-digde2;

    for (e=mine+9; e<maxd; e+=9) {
      if ( (e0+=9) >=power10) {
        e0-=power10;
        digd_e1=digd+dig[++e1];
        digd_e1_sub_digde2=digd_e1-digde2;
      }
      if ( (de0+=step0) >=power10) {
        de0-=power10;
        de1++;
      }
      if ( (de1+=step1) >=power10) {
        de1-=power10;
        digde2=dig[++de2];
        digd_e1_sub_digde2=digd_e1-digde2;
      }

      if (dig[de0]+dig[de1]-dig[e0] == digd_e1_sub_digde2) {
        if (dby10 && (e%10==0)) continue; /* Not true vampire */
        if (dig64[d]+dig64[e1]+dig64[e0] == dig64[de0]+dig64[de1]+dig64[de2]) {
          printf("%Lu = %lu*%lu\n", (int64) d*e,d,e);
          vampcount++;
        }
        else {
          fprintf(stderr,"dig64<>dig for: ");
          fprintf(stderr,"%Lu = %lu*%lu\n", (int64) d*e,d,e);
        }
      }

    }
  }
  fprintf(stderr,"%d vampire numbers found.\n",vampcount);
}

int main (void) {
  initdigit();
  fprintf(stderr,"Searching..\n");
  searchvampadd12();
  fprintf(stderr,"Exiting after searchvampadd12.\n");
  return 0;
}
