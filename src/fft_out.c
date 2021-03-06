#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fftw3.h>
#include "sacio.h"

int near_pow2( int n  ){
    int m;
    float f;
    f = log((float)n) / log(2.) + 1.;
    m = (int)pow(2.,(int)f);
    return m;
}


int main(int argc, char *argv[]) {
    int i,npts, low_f_n, high_f_n;
    float *data, peak = 0., high_f, low_f;
    SACHEAD hd;
    fftw_complex *in, *out;
    fftw_plan p;
    FILE *f, *fp;

    if( argc != 5 ) {
        fprintf(stderr,"Usage: fft_out sacfile fre_low fre_high output_file\n");
        fprintf(stderr,"       return sacfile in frequency-amplitude domain between fre_low and fre_high\n");
        exit(1);
    }

    low_f = atof(argv[2]); // low limitation of frequency.
    high_f = atof(argv[3]); // high limitation frequency.
    f = fopen(argv[4],"w"); // pointer of outputing file.
    fp = fopen("plot.sh","w"); // pointer of plot script file.
    data = read_sac(argv[1],&hd); // amplitude values.
    npts = near_pow2(hd.npts); // number of points of data.
    low_f_n = npts * low_f * hd.delta;
    high_f_n = npts * high_f * hd.delta;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * npts);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * npts);
    for ( i = 0; i < npts; i ++ ) {
        if( i < hd.npts ) in[i][0] = data[i];
        else in[i][0]= 0.;
        in[i][1] = 0.;
    }

    p = fftw_plan_dft_1d(npts,in,out,FFTW_FORWARD,FFTW_ESTIMATE);
    fftw_execute(p);

// Outputting results between frequencies of low_f and high_f.
    for ( i = low_f_n; i <= high_f_n; i ++ ) {
        fprintf(f,"%6.3f %6.3f\n", i/hd.delta/npts, sqrt(pow(out[i][0],2.)+pow(out[i][1],2.)));
        if ( peak < sqrt(pow(out[i][0],2.)+pow(out[i][1],2.)) ) peak = sqrt(pow(out[i][0],2.)+pow(out[i][1],2.));
    }

    fclose(f);

    //fprintf(fp,"gmt psxy %s -R0/%f/0/%f -JX7i/4i -Bx%f+l\"Frequency(Hz)\" -By%.1f+l\"Amplitude\" -BWSen+t\"%s\" -W0.8p,red>plot.ps\n", argv[3], high_f, peak, high_f/10., peak/10.,argv[1]);
    fprintf(fp,"awk '{print $1,$2/%f}' %s >tmp.txt\n", peak, argv[4]);
    fprintf(fp,"awk '{print \">\",\"\\n\",$1,0,\"\\n\",$1,$2}' tmp.txt >tmp1.txt\n");
    fprintf(fp,"PS=%s_fftout.ps\nPDF=%s_fftout.pdf\n", argv[1], argv[1]);
    fprintf(fp,"gmt gmtset FONT_TITLE 25p,5,black\ngmt gmtset FONT_LABEL 16p,5,black\n");
    fprintf(fp,"gmt gmtset FONT_ANNOT_PRIMARY 12,5,black\n");
    fprintf(fp,"gmt psxy tmp.txt -R%f/%f/0/1 -JX7i/4i -Bx%f+l\"Frequency(Hz)\" -By0.1+l\"Normalized Amplitude\" -BWSen+t\"%s\" \
            -Sc0.05c -Gred -K >$PS\n", low_f, high_f, high_f/10., argv[1]);
    fprintf(fp,"gmt psxy tmp1.txt -R -J -O -W0.5p>>$PS\n");
    fprintf(fp,"gmt psconvert -Tg -A -P $PS\n");
    fprintf(fp,"ps2pdf $PS $PDF\n");
    fprintf(fp,"rm tmp* gmt.*\n");
    fclose(fp);

    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out); free(data);
    return 0;
}
