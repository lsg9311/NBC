#include "data.h"
#include "classifier.h"

#include <iostream>
#include <list>
#include <fstream>

using namespace std;

int main() {
	DataSet training_set = DataSet();
	DataSet test_set = DataSet();
	DataReader data_reader = DataReader();
	TRC trc = TRC();
	TSC tsc = TSC();
	
	Result train_value;

	int menu = 0;

	while (1) {
		cout << "메뉴를 선택하세요" << endl;
		cout << "1. Training Data" << endl;
		cout << "2. Test Data" << endl;
		cout << "3. Test Model" << endl;
		cin >> menu;
		if (menu == 1) {
			data_reader.save_data("trn.txt", &training_set);
			trc.classify(&training_set);
			trc.eval_mean(0);
			trc.eval_mean(1);

			trc.eval_var(0);
			trc.eval_var(1);

			trc.eval_prob(0);
			trc.eval_prob(1);

			train_value = trc.get_result();
			int x_iter = 0;
			cout << "Training Complete" << endl;
			cout << "Probability : " << train_value.prob[0] << ", " << train_value.prob[1] << endl;
			cout << "Mean : " << endl;
			for (x_iter = 0; x_iter < 13; x_iter++) {
				cout << train_value.mean[0][x_iter] << ", " << train_value.mean[1][x_iter] << endl;
			}
			cout << "Variance : " << endl;
			for (x_iter = 0; x_iter < 13; x_iter++) {
				cout << train_value.var[0][x_iter] << ", " << train_value.var[1][x_iter] << endl;
			}
		}
		if (menu == 2) {
			tsc.init();
			data_reader.save_data("tst.txt", &test_set);
			tsc.eval_rate(&test_set, train_value, -5.0);
			tsc.print_result();
		}
		if (menu == 3) {
			struct pos_val {
				double TP;
				double FP;
			};
			list<pos_val> graph_val = list<pos_val>();
			double threshold = -21;
			for (threshold = -21; threshold <= 25; threshold += 0.01) {
				tsc.init();
				pos_val cur_val;
				//cout <<"Test "<<train_value.prob[0] << ", " << train_value.prob[1] <<" case"<< endl;
				tsc.eval_rate(&test_set, train_value,threshold);
				cur_val.TP = tsc.get_TP();
				cur_val.FP = tsc.get_FP();
				if (1-cur_val.TP == cur_val.FP) {
					cout << "ERR : " << threshold << endl;
					cout << "FN, FP : " << 1 - cur_val.TP << endl;
				}
				graph_val.push_back(cur_val);
			}
			list<pos_val>::iterator node;
			ofstream outFile = ofstream("result.txt");
			outFile << "TP\tFP" << endl;
			for (node = graph_val.begin(); node != graph_val.end(); node++) {
				outFile << node->TP << "\t" << node->FP << endl;
			}
			outFile.close();
		}
	}

	return 0;
}