:hotel:[Return to Home Page](https://github.com/geophydog/geophydog.github.io)


***

# NOTICE!!! THIS PROGRAM COMES WITH NO WARRANTY, TO THE EXTEND PERMITED BY LAW!

***

# FFT_SAC-format-file
- To execute FFT of a SAC format file and virtualize the result;

***

# Usage
```
fft_out sacfile fre_low fre_high output_file
```

| parameter | mean |
| --------- | ---- |
|  sacfile  | SAC format file name |
|  fre_low  | low limitation of corner frequency |
|  fre_high | high limitation of corner frequency |

***

# Denpendencies
- Linux or Mac OS platform
- [GMT](http://gmt.soest.hawaii.edu/projects/gmt/wiki/Download) (the Generic Mapping Tools), here version of 5.3.1

***

# Example
```
fft_out BOZ.BHZ.SAC 0.01 0.5 out.txt
```

 - :one: Raw data in time domain.
 
 ![Raw-data](https://github.com/geophydog/FFT_SAC-format-file/blob/master/images/BOZBHZ.SAC-raw.jpg)
 
 - :two: FFT data in frequency domain.
 
 ![FFT](https://github.com/geophydog/FFT_SAC-format-file/blob/master/images/BOZBHZ.SAC-fre.jpg)
 
***

# Contribution
- Author: Xuping Feng 
- Email: geophydogvon@gmail.com
