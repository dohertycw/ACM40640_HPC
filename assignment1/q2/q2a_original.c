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

float wnew(struct matrix w, int i, int j);

int main(void){
	int i, j, k;
	struct matrix w;
	struct matrix temp;
	double upd, bcond;
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

	bcond = pow(10.0,-4.0);

	temp.row = w.row;
	temp.col = w.col;
	temp.matrix = (double *)malloc(w.row*w.col*sizeof(double));

	for(i = 0; i<w.row;i++){
		for(j = 0; j<w.col;j++){
			if(i == 0 || j == 0 || i == 99 || j == 99){
				temp.matrix[getindex(temp,i,j)] = w.matrix[getindex(w,i,j)];
			} else if(abs(wnew(w,i,j) - w.matrix[getindex(w,i,j)]) > bcond){
				temp.matrix[getindex(w,i,j)] = wnew(w,i,j);
			} else {
				temp.matrix[getindex(w,i,j)] = w.matrix[getindex(w,i,j)];
			}
		}
	}

	for(i = 0; i<w.row;i++){
		for(j = 0; j<w.row;j++){
			w.matrix[getindex(w,i,j)] = temp.matrix[getindex(w,i,j)];
		}
	}

	free(temp.matrix);

	fp2 = fopen("matrix_updated.dat", "w");

	for(i = 0; i<w.row; i++){
		for(j=0; j<w.col;j++){
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

float wnew(struct matrix w, int i, int j){
	float num = w.matrix[getindex(w,i + 1,j)] + w.matrix[getindex(w, i - 1, j)] + w.matrix[getindex(w, i, j+1)] +  w.matrix[getindex(w, i, j-1)];
	float val = num/4.0;
	return val;
}

