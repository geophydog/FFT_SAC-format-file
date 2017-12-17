OBJ = fft_out.o sacio.o

fft_out : $(OBJ)
	cc -o fft_out $(OBJ) -lm -lfftw3

$(OBJ) : sacio.h

clean : 
	rm -f fft_out $(OBJ)

