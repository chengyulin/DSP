#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmm.h"

char data[10010][201];
double alpha[10001][MAX_SEQ][MAX_STATE];
double beta[10001][MAX_SEQ][MAX_STATE];
double gam[10001][MAX_SEQ][MAX_STATE];
double epsilon[10001][MAX_SEQ][MAX_STATE][MAX_STATE];
int cnt;

void forward(const HMM *hmm)
{
	// printf("%d\n",cnt );
	for(int ind = 0; ind < cnt; ind++){
		int length = strlen(data[ind]);
		for(int j = 0; j < hmm->state_num; j++){
			alpha[ind][0][j] = hmm->initial[j] * hmm->observation[data[ind][0]-'A'][j];
			//printf("%lf\n",alpha[0][j] );
		}

		for(int t = 0; t < length - 1; t++){
			for(int j = 0; j < hmm->state_num; j++){
				for(int i = 0; i < hmm->state_num; i++){
					alpha[ind][t+1][j] += alpha[ind][t][i] * hmm->transition[i][j] * hmm->observation[data[ind][t+1]-'A'][j];
				}
			}
		}

	}
	//printf("%lf\n",alpha[0][0][0] );
	return;
}
void backward(const HMM *hmm)
{
	//initialize beta
	for (int ind = 0; ind < cnt; ++ind){	
		int length = strlen(data[ind]);
		for(int i = 0; i < hmm->state_num; i++){
			beta[ind][length-1][i] = 1.0;
		}
		for(int t = length - 2; t >= 0; t--){
			for(int i = 0 ; i < hmm->state_num; i++){
				for(int j = 0 ; j < hmm->state_num; j++){
					beta[ind][t][i] += hmm->transition[i][j] * hmm->observation[(data[ind][t+1])-'A'][j] * beta[ind][t+1][j];
				}
			}
		}		
	}
	
	return;
}
void forward_backward(HMM* hmm){

// 	// calculate gamma
 	for (int ind = 0; ind < cnt; ++ind){
 		int length = strlen(data[ind]);
 		for(int t = 0 ; t < length; t++){
			double gd = 0.0;
			for(int i = 0; i < hmm->state_num; i++){
				double gn = alpha[ind][t][i] * beta[ind][t][i];
				gam[ind][t][i] = gn;
				gd += gn;
			}

			for(int i = 0; i < hmm->state_num; i++){
				gam[ind][t][i] /= gd;
			}
		}
 	}
	
    // calculate epsilon
    for (int ind = 0; ind < cnt; ++ind){	
    	int length = strlen(data[ind]);
    	for(int t = 0; t < length - 1; t++){
			double ed = 0.0;
			for(int i = 0; i < hmm->state_num; i++){
				for(int j = 0; j < hmm->state_num; j++){
					double en = alpha[ind][t][i] * hmm->transition[i][j] * hmm->observation[data[ind][t+1]-'A'][j] * beta[ind][t+1][j];
					epsilon[ind][t][i][j] = en;
					ed += en;
				}
			}
			
			for(int i = 0; i < hmm->state_num; i++){
				for(int j = 0; j < hmm->state_num; j++){
					epsilon[ind][t][i][j] /= ed;
				}
			}
		}
    }
    double init[MAX_STATE] = {0.0};
    // printf("cnt %d\n",cnt );
    for (int ind = 0; ind < cnt; ++ind){
    	 for(int i = 0; i < hmm->state_num; i++)
			init[i] += ((gam[ind][0][i])/cnt);
    }
    for(int i = 0; i < hmm->state_num; i++){
			hmm->initial[i] = init[i];
	}
    for(int i = 0; i < hmm->state_num; i++)
		printf("%lf",hmm->initial[i]);

    double tn[MAX_STATE][MAX_STATE] = {};
	double td[MAX_STATE][MAX_STATE] = {};
	double on[MAX_OBSERV][MAX_STATE] = {};
	double od[MAX_OBSERV][MAX_STATE] = {};
    for (int ind = 0; ind < cnt; ++ind){
	    for(int i = 0; i < hmm->state_num; i++){
			for(int j = 0; j < hmm->state_num; j++){
				int length = strlen(data[ind]);
				for(int t = 0; t < length - 1; t++){
					tn[i][j] += epsilon[ind][t][i][j];
					td[i][j] += gam[ind][t][i];
				}
			}
		}
	}
	for(int i = 0; i < hmm->state_num; i++){
		for(int j = 0; j < hmm->state_num; j++){
			hmm->transition[i][j] = tn[i][j] / td[i][j];
		}
	}

	for (int ind = 0; ind < cnt; ++ind){
	   	for(int k = 0; k < hmm->observ_num; k++){
			for(int i = 0; i < hmm->state_num; i++){
				int length = strlen(data[ind]);
				for(int t = 0; t < length; t++){
					if(data[ind][t] - 'A' == k)
						on[k][i] += gam[ind][t][i];
					od[k][i] += gam[ind][t][i];
				}
			}
		}
	}
	for(int i = 0; i < hmm->observ_num; i++){
		for(int j = 0; j < hmm->state_num; j++){
			hmm->observation[i][j] = (on[i][j] / od[i][j]);		
		}
	}
	
	printf("\n");
 	return;
}

void read_data(const char* seq_model_name)
{
	FILE* seq_model = open_or_die(seq_model_name, "r");
	char seq[MAX_SEQ] = "";
	cnt = 0;
	while(fscanf(seq_model, "%s", seq) != EOF){
		for (int i = 0; i < strlen(seq); ++i){
			data[cnt][i] = seq[i];
		}
		cnt += 1;
	}
	// printf("%d %s\n",cnt,data[cnt-1] );
	fclose(seq_model);
	return;
}

void reset()
{	
	for (int h = 0; h < cnt; ++h){
		for (int i = 0; i < MAX_SEQ; ++i){
			for (int j = 0; j < MAX_STATE; ++j){
				alpha[h][i][j] = 0.0;
				beta[h][i][j] = 0.0;
				gam[h][i][j] = 0.0;
				for (int k = 0; k < MAX_STATE; ++k){
					epsilon[h][i][j][k] = 0.0;
				}
			}
		}
	}
	return;
}
int main(int argc, char* argv[])
{
	int iteration = atoi(argv[1]);

	HMM hmm;
	loadHMM(&hmm, argv[2]);

	read_data(argv[3]);

	for (int it = 0; it < iteration; ++it){
		printf("%d\n",it );
		reset();
		forward(&hmm);
		backward(&hmm);
		forward_backward(&hmm);
	}
	FILE* model = open_or_die(argv[4], "w");
	dumpHMM(model, &hmm);
	fclose(model);

	return 0;
}