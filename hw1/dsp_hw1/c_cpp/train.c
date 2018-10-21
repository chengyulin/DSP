#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmm.h"

// void train_model(HMM* hmm, const char* seq_model_name)
// {
// 	FILE* seq_model = open_or_die(seq_model_name, "r");
// 	char seq[MAX_SEQ] = "";
// 	P p;
	
// 	initialP(&p);

// 	while(fscanf(seq_model, "%s", seq) != EOF){
// 		double al[MAX_SEQ][MAX_STATE] = {{0.0}}; // alpha for forward algorithm
// 		double be[MAX_SEQ][MAX_STATE] = {{0.0}}; // beta for backward algorithm

// 		forward(hmm, seq, al);
// 		backward(hmm, seq, be);
// 		forward_backward(hmm, seq, al, be, &p);
// 	}

// 	updateHMM(hmm, &p);
	
// 	fclose(seq_model);

// 	return;
// }

void forward()
{

}
void backward()
{
	
}
int main(int argc, char* argv[])
{
	int iteration = atoi(argv[1]);

	HMM hmm;
	loadHMM(&hmm, argv[2]);

	// for(int i = 0; i < iteration; i++)
	// 	train_model(&hmm, argv[3]);

	FILE* model = open_or_die(argv[4], "w");
	dumpHMM(model, &hmm);
	fclose(model);

	return 0;
}