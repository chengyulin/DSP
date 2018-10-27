#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmm.h"

double viterbi(const HMM* hmm, const char* seq)
{
	double delta[MAX_SEQ][MAX_STATE] = {{0.0}};	

	for(int i = 0; i < hmm->state_num; i++){
		delta[0][i] = hmm->initial[i] * hmm->observation[seq[0]-'A'][i];
	}

	int length = strlen(seq);
	for(int t = 1; t < length; t++){
		for(int j = 0; j < hmm->state_num; j++){
			double mx = -1.0;
			for(int i = 0; i < hmm->state_num; i++){
				double cur = delta[t-1][i] * hmm->transition[i][j];
				if(mx < cur)
					mx = cur;
			}
			delta[t][j] = mx * hmm->observation[seq[t]-'A'][j];
		}
	}
	
	double mx = -1.0;
	for(int i = 0; i < hmm->state_num; i++){
		if(delta[length-1][i] > mx){
			mx = delta[length-1][i];
		}
	}
	return mx;
}

int main(int argc, char* argv[])
{
	HMM models[MAX_LINE];

	int model_cnt = load_models(argv[1], models, MAX_LINE);
	
	char seq[MAX_SEQ] = "";
	FILE* data = open_or_die(argv[2], "r");
	FILE* result = open_or_die(argv[3], "w");
	
	while(fscanf(data, "%s", seq) != EOF){
		int ind = -1;
		double mx = -1.0;
		for(int i = 0; i < model_cnt; i++){
			double curP = viterbi(&models[i], seq);
			if(mx < curP){
				mx = curP;
				ind = i;
			}
		}	
		fprintf(result, "%s\n", models[ind].model_name);
	}

	fclose(data);
	fclose(result);

	return 0;
}