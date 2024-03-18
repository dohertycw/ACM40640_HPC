#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<math.h>

struct matrix {
	int row;
	int col;
	double *matrix;
};

int getindex(struct matrix w, int i, int j);

int main(void){
	int i, j, k;
	struct matrix w;
	double w_old;
	FILE *fp1, *fp2;
	
	w.row = 100;
	w.col = 100;
	
	w.matrix = (double *)malloc(w.row*w.col*sizeof(double));

	for(i = 0; i<w.row; i++){
		for(j = 0; j <w.col; j++){
                        if(i == 99 || j == 0 || j == 99){
                                w.matrix[getindex(w,i,j)] = 100.0;
                        }
		        else if(i == 0 && 1 < j < 98){
                                w.matrix[getindex(w,i,j)] = 0.0;
			}
			else{
				w.matrix[getindex(w,i,j)] = 75.0;
			}
		}
	}			


	fp1 = fopen("matrix_intial.dat", "w");
	if(fp1 == NULL){
		printf("Cannot open file.\n");
		exit(1);
	}

	for(i = 0; i<w.row; i++){
		for(j=0; j<w.col; j++){
			fprintf(fp1, "%f ",w.matrix[getindex(w,i,j)]);
		}
		fprintf(fp1, "\n");
	}

	for(i = 1; i<w.row-1; i++){
		for(j = 1; j <w.col-1; j++){
			w_old = w.matrix[getindex(w,i,j)];
			w.matrix[getindex(w,i,j)] = (w.matrix[getindex(w,i+1,j)] + w.matrix[getindex(w, i-1, j)] + w.matrix[getindex(w,i,j+1)] + w.matrix[getindex(w, i, j-1)])/4;
			if(abs(w_old - w.matrix[getindex(w,i,j)]) < pow(10.0, -4.0)){
				break;
			}
		}
	}

	fp2 = fopen("matrix_updated.dat", "w");
	if(fp2 == NULL){
		printf("Cannot open file.\n");
		exit(2);
	}

	for(i = 0; i<w.row;i++){
		for(j=0;j<w.col;j++){
			fprintf(fp2, "%f ", w.matrix[getindex(w,i,j)]);
		}
		fprintf(fp2, "\n");
	}


	fclose(fp1);
	fclose(fp2);
	free(w.matrix);
	return 0;
}

int getindex(struct matrix w, int i, int j){
	return w.col*i + j;
}

