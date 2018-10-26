#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmm.h"

char data[10010][201];
double alpha[MAX_SEQ][MAX_STATE];
double beta[MAX_SEQ][MAX_STATE];
double gam[MAX_SEQ][MAX_STATE];
double epsilon[MAX_SEQ][MAX_STATE][MAX_STATE];
int cnt;

void forward(const HMM *hmm)
{
	printf("%d\n",cnt );
	for(int ind = 0; ind < cnt; ind++){
		int length = strlen(data[ind]);
		for(int j = 0; j < hmm->state_num; j++){
			alpha[0][j] = hmm->initial[j] * hmm->observation[data[ind][0]-'A'][j];
			//printf("%lf\n",alpha[0][j] );
		}

		for(int t = 0; t < length - 1; t++){
			for(int j = 0; j < hmm->state_num; j++){
				for(int i = 0; i < hmm->state_num; i++){
					alpha[t+1][j] += alpha[t][i] * hmm->transition[i][j] * hmm->observation[data[ind][t+1]-'A'][j];
				}
			}
		}

	}
	//printf("%lf\n",alpha[0][0] );
	return;
}
void backward(const HMM *hmm)
{
	
		
	//initialize beta
	for (int ind = 0; ind < cnt; ++ind)
	{	
		int length = strlen(data[ind]);
		for(int i = 0; i < hmm->state_num; i++){
			beta[length-1][i] = 1.0;
		}
		for(int t = length - 2; t >= 0; t--){
			for(int i = 0 ; i < hmm->state_num; i++){
				for(int j = 0 ; j < hmm->state_num; j++){
					beta[t][i] += hmm->transition[i][j] * hmm->observation[data[ind][t+1]-'A'][j] * beta[t+1][j];
				}
			}
		}		
	}
	printf("%lf\n",beta[0][0] );
	
	return;
}
// void forward_backward(const HMM* hmm, const char* seq, 
// 					double al[MAX_SEQ][MAX_STATE], double be[MAX_SEQ][MAX_STATE], P* p)
// {
// 	//double ga[MAX_SEQ][MAX_STATE] = {{0.0}}; // gamma
// 	//double ep[MAX_SEQ][MAX_STATE][MAX_STATE] = {{{0.0}}}; // epsilon

// 	int length = strlen(seq);
	
// 	// calculate gamma
// 	for(int t = 0 ; t < length; t++){
// 		double gd = 0.0;
// 		for(int i = 0; i < hmm->state_num; i++){
// 			double gn = al[t][i] * be[t][i];
// 			ga[t][i] = gn;
// 			gd += gn;
// 		}

// 		for(int i = 0; i < hmm->state_num; i++)
// 			ga[t][i] /= gd;
// 	}

// 	// calculate epsilon
// 	for(int t = 0; t < length - 1; t++){
// 		double ed = 0.0;
// 		for(int i = 0; i < hmm->state_num; i++)
// 			for(int j = 0; j < hmm->state_num; j++){
// 				double en = al[t][i] * hmm->transition[i][j] * hmm->observation[seq[t+1]-'A'][j] * be[t+1][j];
// 				ep[t][i][j] = en;
// 				ed += en;
// 			}
		
// 		for(int i = 0; i < hmm->state_num; i++)
// 			for(int j = 0; j < hmm->state_num; j++)
// 				ep[t][i][j] /= ed;
// 	}
	
// 	p->sample_num++;
// 	// acculmulate
// 	for(int i = 0; i < hmm->state_num; i++)
// 		p->init[i] += ga[0][i];

// 	for(int i = 0; i < hmm->state_num; i++)
// 		for(int j = 0; j < hmm->state_num; j++)
// 			for(int t = 0; t < length - 1; t++){
// 				p->tn[i][j] += ep[t][i][j];
// 				p->td[i][j] += ga[t][i];
// 			}

// 	for(int k = 0; k < hmm->observ_num; k++)
// 		for(int i = 0; i < hmm->state_num; i++)
// 			for(int t = 0; t < length; t++){
// 				if(seq[t] - 'A' == k)
// 					p->on[k][i] += ga[t][i];
// 				p->od[k][i] += ga[t][i];
// 			}
	
// 	return;
// }

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
	printf("%d %s\n",cnt,data[cnt-1] );
	fclose(seq_model);
	return;
}

void reset()
{	
	for (int i = 0; i < MAX_SEQ; ++i){
		for (int j = 0; j < MAX_STATE; ++j){
			alpha[i][j] = 0.0;
			beta[i][j] = 0.0;
			gam[i][j] = 0.0;
			for (int k = 0; k < MAX_STATE; ++k){
				epsilon[i][j][k] = 0.0;
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
		reset();
		forward(&hmm);
		backward(&hmm);
	}
	FILE* model = open_or_die(argv[4], "w");
	dumpHMM(model, &hmm);
	fclose(model);

	return 0;
}