#include "classifier.h"

TRC::TRC() {
	this->data[0] = DataSet();
	this->data[1] = DataSet();

	this->result = Result();
}

void TRC::classify(DataSet* data_set){
	int iter = 0;
	Data cur_data;
	for (iter = 0; iter < data_set->size(); iter++) {
		cur_data = data_set->get_data(iter);
		if (cur_data.r == 0) {
			this->data[0].insert_data(cur_data);
		}
		else {
			this->data[1].insert_data(cur_data);
		}
	}
}

void TRC::eval_prob(int r) {
	this->result.prob[r] = (double)this->data[r].size() / ((double)data[0].size() + (double)data[1].size());
	return;
}

int TRC::nofr(int r) {
	return this->data[r].size();
}

void TRC::eval_mean(int r) {
	int data_iter = 0;
	int	x_iter = 0;
	Data temp_data;
	double sum_x[13] = { 0 };
	//sum
	for (data_iter = 0; data_iter < this->data[r].size(); data_iter++) {
		temp_data = this->data[r].get_data(data_iter);
		for (x_iter = 0; x_iter < 13; x_iter++) {
			sum_x[x_iter] += temp_data.x[x_iter];
		}
	}
	//evaluate mean
	for (x_iter = 0; x_iter < 13; x_iter++) {
		this->result.mean[r][x_iter] = sum_x[x_iter] / this->data[r].size();
	}
	return;
}

void TRC::eval_var(int r) {
	int data_iter = 0;
	int x_iter = 0;
	Data temp_data;
	double sum_x[13] = { 0 };
	for (data_iter = 0; data_iter < this->data[r].size(); data_iter++) {
		temp_data = this->data[r].get_data(data_iter);
		for (x_iter = 0; x_iter < 13; x_iter++) {
			sum_x[x_iter] += pow((temp_data.x[x_iter] - this->result.mean[r][x_iter]), 2);
		}
	}
	for (x_iter = 0; x_iter < 13; x_iter++) {
		this->result.var[r][x_iter] = sum_x[x_iter] / this->data[r].size();
	}
	return;
}

Result TRC::get_result() {
	return this->result;
}

//Test data classifier
TSC::TSC() {
	this->s_rate = 0;
	this->e_rate = 0;
	this-> T0 = 0;
	this-> T1 = 0;
	this-> F0 = 0;
	this-> F1 = 0;
}
void TSC::init() {
	this->s_rate = 0;
	this->e_rate = 0;
	this-> T0 = 0;
	this-> T1 = 0;
	this-> F0 = 0;
	this-> F1 = 0;
}

double TSC::get_AC() {
	return (double)((this->T0 + this->T1) / (this->T0 + this->T1 + this->F0 + this->F1));
}

double TSC::get_TP() {
	return (double)((this->T1) / (this->T1 + this->F0)); 
}
double TSC::get_FP() {
	return (double)((this->F1) / (this->T0 + this->F1));
}

void TSC::eval_rate(DataSet* test_set, Result trn_value,double threshold) {
	int success = 0;
	int error = 0;
	int N = test_set->size();

	int answer;
	int x_iter = 0;

	Data cur_data;

	double constant=0;
	double cmp_value = 0;
	double x;
	//eval constant
	for (x_iter = 0; x_iter < 13; x_iter++) {
		constant += (log(trn_value.var[1][x_iter]) - log(trn_value.var[0][x_iter]));
	}
	constant -= log(pow(trn_value.prob[1], 2)) - log(pow(trn_value.prob[0], 2));
	//add some constant
	constant += threshold;
	//testing data
	for (int iter = 0; iter < N; iter++) {
		cur_data = test_set->get_data(iter);
		cmp_value = 0;
		for (x_iter = 0; x_iter < 13; x_iter++) {
			x = cur_data.x[x_iter];
			cmp_value += pow((x - trn_value.mean[0][x_iter]), 2) / trn_value.var[0][x_iter];
			cmp_value -= pow((x - trn_value.mean[1][x_iter]), 2) / trn_value.var[1][x_iter];
		}
		
		//classify
		if (cmp_value < constant)	answer = 0;
		else if (cmp_value > constant)	answer = 1;
		else {
			cout <<"Data "<<iter<<" is Ambiguous" << endl;
			error++;
			continue;
		}
		//test
		if (answer == cur_data.r) {
			success++;
			if (answer == 0) this->T0++;
			else if (answer == 1) this->T1++;
			
		}
		else {
			error++;
			if (answer == 0) this->F0++;
			else if (answer == 1) this->F1++;
		}
	}
	//evaluate
	this->s_rate = (double)success / (double)N;
	this->e_rate = (double)error / (double)N;
	return;
}

void TSC::print_result() {
	cout << "Success : " << this->s_rate << endl;
	cout << "Error : " << this->e_rate << endl;
	cout << "AC : " << (double)((this->T0 + this->T1) / (this->T0 + this->T1 + this->F0 + this->F1)) << endl;
	cout << "TP : " << (double)((this->T1) / (this->T1 + this->F0)) << endl;
	cout << "TN : " << (double)((this->T0) / (this->T0 + this->F1)) << endl;
	cout << "FP : " << (double)((this->F0) / (this->T1 + this->F0)) << endl;
	cout << "FN : " << (double)((this->F1) / (this->T0 + this->F1)) << endl;

	return;
}