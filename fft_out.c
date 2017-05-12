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
    int i,npts;
    float *data, peak = 0., high_f;
    SACHEAD hd;
    fftw_complex *in, *out;
    fftw_plan p;
    FILE *f, *fp;

    if( argc != 4 ) {
        fprintf(stderr,"Usage: fft_out sacfile high_freq-limit out_result_file\n");
        fprintf(stderr,"       return sacfile in frequency-amplitude domain\n");
        exit(1);
    }

    high_f = atof(argv[2]);
    f = fopen(argv[3],"w");
    fp = fopen("plot.sh","w");
    data = read_sac(argv[1],&hd);
    npts = near_pow2(hd.npts);
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * npts);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * npts);
    for ( i = 0; i < npts; i ++ ) {
        if( i < hd.npts ) in[i][0] = data[i];
        else in[i][0]= 0.;
        in[i][1] = 0.;
    }

    p = fftw_plan_dft_1d(npts,in,out,FFTW_FORWARD,FFTW_ESTIMATE);
    fftw_execute(p);

    for ( i = 0; i < hd.npts; i ++ ) {
        fprintf(f,"%6.3f %6.3f\n", i/hd.delta/npts, sqrt(pow(out[i][0],2.)+pow(out[i][1],2.)));
        if ( peak < sqrt(pow(out[i][0],2.)+pow(out[i][1],2.)) ) peak = sqrt(pow(out[i][0],2.)+pow(out[i][1],2.));
    }

    fclose(f);

    fprintf(fp,"gmt psxy %s -R0/%f/0/%f -JX7i/4i -Bx%f+l\"Frequency(Hz)\" -By%.1f+l\"Amplitude\" -BWSen+t\"%s\" -W0.8p,red>plot.ps\n", argv[3], high_f, peak, high_f/10., peak/10.,argv[1]);
    fprintf(fp,"ps2pdf plot.ps plot.pdf\n");
    fclose(fp);
    //printf("gmt psxy %s -R0/%f/0/%f -JX9i/6i -Bx%f+l\"Frequency(Hz)\" -By%f+l\"Amplitude\" -W1.2p,red>plot.ps\n", argv[1], 1./hd.delta, peak, 1./hd.delta/10., peak/10.);
    system("sh plot.sh");

    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out); free(data);
    return 0;
}
