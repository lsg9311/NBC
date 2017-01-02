#include "data.h"

///DATA SET///
DataSet::DataSet() {
	this->data_list = vector<Data>();
}

void DataSet::insert_data(Data new_data) {
	this->data_list.push_back(new_data);
}

Data DataSet::get_data(int id) {
	return this->data_list.at(id);
}

int DataSet::size() {
	return this->data_list.size();
}

///DATA READER//
void DataReader::save_data(string filepath, DataSet* data_set) {
	ifstream trFile = ifstream(filepath);
	Data new_data;
	string trLine;
	
	string temp_x;
	string temp_r;

	double new_x;
	int new_r;
	
	int x_iter = 0;

	while (getline(trFile, trLine)) {
		if (trLine.compare(" 0") == 0) continue;
		new_data = Data();
		for (x_iter = 0; x_iter < 13; x_iter++) {
			if (trLine[0] == ' ') trLine = trLine.substr(1);
			temp_x = trLine.substr(0,trLine.find_first_of(" "));
			new_x = stod(temp_x);
			trLine= trLine.substr(trLine.find_first_of(" ")+1);
			new_data.x[x_iter] = new_x;
		}
		temp_r = trLine;
		new_r = stoi(temp_r);
		new_data.r = new_r;

		data_set->insert_data(new_data);
	}
	trFile.close();
	return;
}