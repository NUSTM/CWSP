/********************************************************************
* cwsp: Chinese Word Segmentation Toolkit used multi-perceptron.
* Copyright (C) 2016 Shenghuan He
* Contact: heshenghuan328@gmail.com

* Permission to use, copy, modify, and distribute this software for
* any non-commercial purpose is hereby granted without fee, provided
* that the above copyright notice appear in all copies and that both
* that copyright notice. 
* It is provided "as is" without express or implied warranty.
*********************************************************************/

#include "MultiPerceptron.h"

namespace cwsp
{
	MultiPerceptron::MultiPerceptron(){}
	MultiPerceptron::~MultiPerceptron(){}

	bool MultiPerceptron::save_model(string model_file)
	{
		if(feat_set_size==0 || class_set_size==0)
		{
			cerr << "\nMultiPerceptron ERROR"<<endl;
			cerr << "\nDoes not have any data to save.\n" << endl;
			return false;
		}
		cout << "Saving model..." << endl;
    	ofstream fout(model_file.c_str());
    	// Write class_set_szie and feat_set_size
    	// Also is the row and cloumn num of the weigths matrix
    	fout << class_set_size << " " << feat_set_size << endl;
    	// Write weights matrix.
		for (int k = 0; k < feat_set_size; k++) {
			for (int j = 0; j < class_set_size; j++) {
				fout << omega[k][j] << " ";
			}
			fout << endl;
		}
    	fout.close();
    	return true;
	}

	bool MultiPerceptron::save_bin_model(string model_file)
	{
		// clock_t begin, finish;
    	// begin = clock();
		if(feat_set_size==0 || class_set_size==0)
		{
			cerr << "\nMultiPerceptron ERROR"<<endl;
			cerr << "\nDoes not have any data to save.\n" << endl;
			return false;
		}
		cout << "Saving model..." << endl;
		FILE* bin_model_file = fopen(model_file.c_str(), "wb");
		fwrite(g_Model_Header.data(), g_Header_Len, 1, bin_model_file);
    	fwrite(&class_set_size, sizeof(int), 1, bin_model_file);
    	fwrite(&feat_set_size, sizeof(int), 1, bin_model_file);
    	for(int i=0; i<feat_set_size; i++) {
      	  for(int j=0; j<class_set_size; j++) {
      	      fwrite(&omega[i][j], sizeof(float), 1, bin_model_file);
      	  }
   	 	}
    	fclose(bin_model_file);
    	// finish = clock();
    	// cout << "Finished! Cost " << (double) (finish - begin) / CLOCKS_PER_SEC << "s" <<endl;
    	return true;
	}

	bool MultiPerceptron::read_model(string model_file)
	{
		FILE *infile;
        infile = fopen(model_file.c_str(), "rb");
        if(!infile)
        {
            cerr << "\nMultiPerceptron Model File ERROR" << endl;
            cerr << "Can not open the MultiPerceptron Model File: "<< model_file <<endl;
            return false;
        }
        char headBuf[UNIGRAM_LEN_MAX];
        fread(&headBuf, g_Header_Len, 1, infile);
        fclose(infile);
        string header = string(headBuf, g_Header_Len);
        if (header == g_Model_Header)
        {
            return load_bin_model(model_file);
        }
        else
        {
            return load_model(model_file);
        }
	}

	bool MultiPerceptron::load_model(string model_file)
	{
		cout << "Loading model..." << endl;
		omega.clear();
    	ifstream fin(model_file.c_str());
    	if(!fin) {
    		cerr << "\nMultiPerceptron Model File ERROR" << endl;
            cerr << "Can not open the MultiPerceptron Model File: "<< model_file <<endl;
            return false;
    	}
    	string line_str;
    	// Read feat_set_size
    	// This line is useless for c/c++ program, but useful for python program
    	// So just read and ignore it. 
    	getline(fin, line_str);

	    // Read weights matrix
    	while (getline(fin, line_str)) {
        	vector<string> line_vec = string_split(line_str, " ");
        	vector<float>  line_omega;
        	for (vector<string>::iterator it = line_vec.begin(); it != line_vec.end(); it++) {
				float weight = (float)atof(it->c_str());
				line_omega.push_back(weight);
			}
			omega.push_back(line_omega);
		}
		fin.close();
		// the size of omega is feature dimension.
		feat_set_size = (int)omega.size();
		// the size of a element of omega is the num of label.
		class_set_size = (int)omega[0].size();
		std::cout << "Loaded a "<<class_set_size<<"*"<<feat_set_size<<" omega matrix."<<endl;
		return true;
	}

	bool MultiPerceptron::load_bin_model(string model_file)
	{
		cout << "Loading binary model..." << endl;
		omega.clear();
		FILE* bin_model_file = fopen(model_file.c_str(), "rb");
		if(bin_model_file == NULL) {
			cerr << "\nMultiPerceptron Model File ERROR" << endl;
            cerr << "Can not open the MultiPerceptron Model File: "<<model_file<<endl;
            return false;
		}
    	char headBuf[UNIGRAM_LEN_MAX];
    	fread(&headBuf, g_Header_Len, 1, bin_model_file);
    	string header = string(headBuf, g_Header_Len);
    	cout << "Header of file: " << header << endl; 
    	class_set_size = 0;
    	feat_set_size = 0;
    	fread(&class_set_size, sizeof(int), 1, bin_model_file);
    	fread(&feat_set_size, sizeof(int), 1, bin_model_file);
    	// cout<<"class size: "<<class_set_size<<", feature size: "<<feat_set_size<<endl;
    	for(int i=0; i<feat_set_size; i++) {
        	vector<float> line_omega;
        	for(int j=0; j<class_set_size; j++) {
            	float tmp;
            	fread(&tmp, sizeof(float), 1, bin_model_file);
            	line_omega.push_back(tmp);
        	}
        	omega.push_back(line_omega);
    	}
    	fclose(bin_model_file);
    	std::cout << "Loaded a "<<class_set_size<<"*"<<feat_set_size<<" omega matrix."<<endl;
    	return true;
	}

	bool MultiPerceptron::read_samp_file(string samp_file, vector<feature> &samp_feat_vec, vector<int> &samp_class_vec)
	{
    	ifstream fin(samp_file.c_str());
    	if(!fin) {
    		cerr << "\nMultiPerceptron ERROR"<<endl;
        	cerr << "Error opening file: " << samp_file << endl;
        	return false;
    	}
    	string line_str;
    	while (getline(fin, line_str)) {
        	size_t class_pos = line_str.find_first_of("\t");
        	int class_id = atoi(line_str.substr(0, class_pos).c_str());
        	samp_class_vec.push_back(class_id);
        	string terms_str = line_str.substr(class_pos+1);
        	feature samp_feat;
        	// samp_feat.id_vec.push_back(0); // bias
        	// samp_feat.value_vec.push_back(1); // bias
        	if (terms_str != "") {
				vector<string> fv_vec = string_split(terms_str, " ");
				for (vector<string>::iterator it = fv_vec.begin(); it != fv_vec.end(); it++) {
					size_t feat_pos = it->find_first_of(":");
					int feat_id = atoi(it->substr(0, feat_pos).c_str());
					float feat_value = (float)atof(it->substr(feat_pos+1).c_str());
					if (feat_value != 0) {
						samp_feat.id_vec.push_back(feat_id);
						samp_feat.value_vec.push_back(feat_value);				
					}
				}
        	}
        	samp_feat_vec.push_back(samp_feat);
    	}
    	fin.close();
    	return true;
	}

	bool MultiPerceptron::read_samps_vec(vector<vector<string> >featsVec, vector<feature> &samp_feat_vec)
	{
		if (featsVec.size()==0)
		{
			cerr << "\nMultiPerceptron ERROR"<<endl;
			cerr << "No feature vector. Parameter featsVec has no element." <<endl;
			return false;
		}
		for(size_t i=0;i<featsVec.size();++i)
		{
			feature samp_feat;
			vector<string>::iterator it;
			for (it=featsVec.at(i).begin();it!=featsVec.at(i).end();++it)
			{
				size_t feat_pos = it->find_first_of(":");
				int feat_id = atoi(it->substr(0, feat_pos).c_str());
				float feat_value = (float)atof(it->substr(feat_pos+1).c_str());
				if (feat_value != 0) {
					samp_feat.id_vec.push_back(feat_id);
					samp_feat.value_vec.push_back(feat_value);				
				}
			}
			samp_feat_vec.push_back(samp_feat);
		}
		return true;
	}

	bool MultiPerceptron::load_training_file(string training_file)
	{
		cout << "Loading training data..." << endl;
		if(!read_samp_file(training_file, samp_feat_vec, samp_class_vec)) return false;
		feat_set_size = 0;
		class_set_size = 0;
		for (size_t i = 0; i < samp_class_vec.size(); i++) {
			if (samp_class_vec[i] > class_set_size) {
				class_set_size = samp_class_vec[i];
			}
			if (samp_feat_vec[i].id_vec.back() > feat_set_size) {
				feat_set_size = samp_feat_vec[i].id_vec.back();
			}
		}
		class_set_size += 1;
		feat_set_size += 1;
		return true;
	}

	bool MultiPerceptron::load_training_data(vector<feature> feat_vec, vector<int> class_vec)
	{
		cout<< "Loading training data..." <<endl;
		samp_feat_vec = feat_vec;
		samp_class_vec = class_vec;
		if( samp_feat_vec.size()==0 || samp_class_vec.size()==0) return false;
		feat_set_size = 0;
		class_set_size = 0;
		for (size_t i = 0; i < samp_class_vec.size(); i++) {
			if (samp_class_vec[i] > class_set_size) {
				class_set_size = samp_class_vec[i];
			}
			if (samp_feat_vec[i].id_vec.back() > feat_set_size) {
				feat_set_size = samp_feat_vec[i].id_vec.back();
			}
		}
		class_set_size += 1;
		feat_set_size += 1;
		return true;
	}

	void MultiPerceptron::init_omega()
	{
		//float int_value = 0.0;
		float int_value = (float)1/class_set_size;
		for (int i = 0; i < feat_set_size; i++)
		{
			vector<float> temp_vec(class_set_size, int_value);
			omega.push_back(temp_vec);
		}
	}

	// Stochastic Gradient Descent (SGD) optimization
	int MultiPerceptron::train_SGD(size_t max_loop, double loss_thrd, float learn_rate, int avg)
	{
		size_t id = 0;
		double loss = 0.0;
		double loss_pre = 0.0;
		vector< vector<float> > omega_sum(omega);
		while (id <= max_loop*samp_class_vec.size())
		{
			// check loss value
			if (id%samp_class_vec.size() == 0)
			{
				size_t loop = id/samp_class_vec.size();
				loss = 0.0;
				float acc = 0.0;
				calc_loss(&loss, &acc);			//perceptron critierion
				cout.setf(ios::left);
				cout << "Iter: " << setw(8) << loop << "Loss: " << setw(18) << loss << "Acc: " << setw(8) << acc << endl;
				if ((loss_pre - loss) < loss_thrd && loss_pre >= loss && id != 0)
				{
					cout << "Reaching the minimal loss value decrease!" << endl;
					break;
				}
				loss_pre = loss;
			}
			// update omega
			int r = (int)(rand()%samp_class_vec.size());   //randomly choose a sample
			//int r = (int)i%samp_class_vec.size();
			feature samp_feat = samp_feat_vec[r];
			int samp_class = samp_class_vec[r];
			update_omega(samp_class, samp_feat, learn_rate);
			if (avg == 1) {
				for (int i = 0; i < feat_set_size; i++)
				{
					for (int j = 0; j < class_set_size; j++)
					{
						omega_sum[i][j] += omega[i][j];
					}
				}			
			}
			id++;
		}
		if (avg == 1) {
			for (int i = 0; i < feat_set_size; i++) {
				for (int j = 0; j < class_set_size; j++) {
					omega[i][j] = (float)omega_sum[i][j] / id;
				}
			}		
		}
		return 1;
	}

	int MultiPerceptron::train_batch(int max_loop, double loss_thrd, float learn_rate, int avg)
	{
		int id = 0;
		int err_num = 0;
		double loss = 0.0;
		double loss_pre = 0.0;
		vector< vector<float> > omega_sum(omega);
		vector< vector<float> > delta;
		while (id <= max_loop) {
			loss = 0.0;
			err_num = 0;
			// delta is the accumulator for the gradient of all the sample
			delta.clear();
			for (int k = 0; k < feat_set_size; k++) {
				vector<float> vec;
				for (int j = 0; j < class_set_size; j++) {
					vec.push_back(0.0);
				}
				delta.push_back(vec);
			}
			for (size_t i = 0; i < samp_class_vec.size(); i++) {
				int samp_class = samp_class_vec[i];
				feature samp_feat = samp_feat_vec[i];
				vector<float> score = calc_score(samp_feat);
				int pred_class = score_to_class(score);
				if (pred_class != samp_class) {
					err_num++;
					loss += (score[pred_class] - score[samp_class]);
					for (size_t k = 0; k < samp_feat.id_vec.size(); k++) {
						int feat_id = samp_feat.id_vec[k];
						float feat_value = samp_feat.value_vec[k];
						delta[feat_id][pred_class] -= learn_rate * feat_value;
						delta[feat_id][samp_class] += learn_rate * feat_value;
					}
				}
			}
			loss = loss / samp_class_vec.size();
			float acc = 1 - (float)err_num / samp_class_vec.size();
			cout.setf(ios::left);
			cout << "Iter: " << setw(8) << id << "Loss: " << setw(18) << loss << "Acc: " << setw(8) << acc << endl;
			if ((loss_pre - loss) < loss_thrd && loss_pre >= loss && id != 0) {
				cout << "Reaching the minimal loss value decrease!" << endl;
				break;
			}
			loss_pre = loss;
			// update omega
			for (int i = 0; i < feat_set_size; i++) {
				for (int j = 0; j < class_set_size; j++) {
					omega[i][j] += delta[i][j];
				}
			}
			if (avg == 1) {
				for (int i = 0; i < feat_set_size; i++) {
					for (int j = 0; j < class_set_size; j++) {
						omega_sum[i][j] += omega[i][j];
					}
				}
			}
			id++;
		}
		if (avg == 1) {
			for (int i = 0; i < feat_set_size; i++) {
				for (int j = 0; j < class_set_size; j++) {
					omega[i][j] = (float)omega_sum[i][j] / (id * samp_class_vec.size());
				}
			}		
		}
		return 1;
	}

	void MultiPerceptron::update_omega(int samp_class, feature &samp_feat, float learn_rate)
	{
		vector<float> score = calc_score(samp_feat);
		int pred_class = score_to_class(score);
		if (samp_class != pred_class) {          
			for (size_t k = 0; k < samp_feat.id_vec.size(); k++) {
				int feat_id = samp_feat.id_vec[k];
				float feat_value = samp_feat.value_vec[k];
				omega[feat_id][pred_class] -= learn_rate * feat_value;
				omega[feat_id][samp_class] += learn_rate * feat_value;
			}
		}
	}

	void MultiPerceptron::calc_loss(double *loss, float *acc)
	{
		double loss_value = 0.0;
		int err_num = 0;
		for (size_t i = 0; i < samp_class_vec.size(); i++) {
			int samp_class = samp_class_vec[i];
			feature samp_feat = samp_feat_vec[i];
			vector<float> score = calc_score(samp_feat);
			int pred_class = score_to_class(score);
			if (pred_class != samp_class) {
				err_num++;
				loss_value += (score[pred_class] - score[samp_class]);
			}
		}
		*acc = 1 - (float)err_num / samp_class_vec.size();
		*loss = loss_value / samp_class_vec.size();
	}

	vector<float> MultiPerceptron::calc_score(feature &samp_feat)
	{
		vector<float> score(class_set_size, 0);
		for (int j = 0; j < class_set_size; j++) {
			for (size_t k = 0; k < samp_feat.id_vec.size(); k++) {
				int feat_id = samp_feat.id_vec[k];
				float feat_value = samp_feat.value_vec[k];
				score[j] += omega[feat_id][j] * feat_value;
			}
		}
    	return score;
	}

	vector<float> MultiPerceptron::score_to_prb(vector<float> &score)
	{   
		vector<float> prb(class_set_size, 0);
		for (int i = 0; i < class_set_size; i++) {
			float delta_prb_sum = 0.0;
			for (int j = 0; j < class_set_size; j++) {
				delta_prb_sum += exp(score[j] - score[i]);
			}
			prb[i] = 1 / delta_prb_sum;
		}
		return prb;
	}

	int MultiPerceptron::score_to_class(vector<float> &score)
	{
		int pred_class = 0;	
		float max_score = score[0];
		for (int j = 1; j < class_set_size; j++) {
			if (score[j] > max_score) {
				max_score = score[j];
				pred_class = j;
			}
		}
    	return pred_class;
	}

	float MultiPerceptron::classify_testing_file(string testing_file, string output_file, int output_format)
	{
		cout << "Classifying testing file..." << endl;
		vector<feature> test_feat_vec;
		vector<int> test_class_vec;
		vector<int> pred_class_vec;
		read_samp_file(testing_file, test_feat_vec, test_class_vec);
		ofstream fout(output_file.c_str());
		for (size_t i = 0; i < test_class_vec.size(); i++) {
			// int samp_class = test_class_vec[i];
			feature samp_feat = test_feat_vec[i];
			vector<float> pred_score = calc_score(samp_feat);			
			int pred_class = score_to_class(pred_score);
			pred_class_vec.push_back(pred_class);
			fout << pred_class << "\t";
			if (output_format == 1) {
				for (int j = 0; j < class_set_size; j++) {
					fout << j << ":" << pred_score[j] << ' '; 
				}		
			}
			else if (output_format == 2) {
				vector<float> pred_prb = score_to_prb(pred_score);
				for (int j = 0; j < class_set_size; j++) {
					fout << j << ":" << pred_prb[j] << ' '; 
				}
			}
			fout << endl;		
		}
		fout.close();
		float acc = calc_acc(test_class_vec, pred_class_vec);
		return acc;
	}

	bool MultiPerceptron::classify_samps_withprb(vector<vector<string> >featsVec, vector<vector<double> > &probs)
	{
		vector<feature> test_feat_vec;
		if(!read_samps_vec(featsVec, test_feat_vec)) return false;
		for (size_t i = 0; i < test_feat_vec.size(); i++) {
			feature samp_feat = test_feat_vec[i];
			vector<float> score = calc_score(samp_feat);
			vector<float> pred_prb = score_to_prb(score);
			vector<double> prob;
			for(size_t j=0;j<pred_prb.size();j++)
			{
				prob.push_back(log((double)pred_prb.at(j)));
			}
			probs.push_back(prob);
		}
		return true;
	}

	float MultiPerceptron::calc_acc(vector<int> &test_class_vec, vector<int> &pred_class_vec)
	{
		size_t len = test_class_vec.size();
		if (len != pred_class_vec.size()) {
			cerr << "\nMultiPerceptron ERROR"<<endl;
			cerr << "Error: two vectors should have the same lenght." << endl;
			exit(0);
		}
		int err_num = 0;
		for (size_t id = 0; id != len; id++) {
			if (test_class_vec[id] != pred_class_vec[id]) {
				err_num++;
			}
		}
		return 1 - ((float)err_num) / len;
	}

	float MultiPerceptron::sigmoid(float x) 
	{
		double sgm = 1 / (1+exp(-(double)x));
		return (float)sgm;
	}

	bool MultiPerceptron::ConvertToBinaryFile(string InputFileName, string OutputFileName)
	{
		if (!load_model(InputFileName)) return false;
		std::cout << "Load text MultiPerceptron model file finished." <<endl;
		// FILE *str_file;
		// FILE *bin_file;
		// str_file = fopen(InputFileName.c_str(), "r");
		// if(!str_file) {
		// 	cerr << "\nMultiPerceptron ERROR" <<endl;
		// 	cerr << "Cannot open the MultiPerceptron model file:" << InputFileName << endl;
		// 	return false;
		// }
		// bin_file = fopen(OutputFileName.c_str(), "wb");
		return save_bin_model(OutputFileName);

	}

	vector<string> MultiPerceptron::string_split(string terms_str, string spliting_tag)
	{
		vector<string> feat_vec;
	    size_t term_beg_pos = 0;
    	size_t term_end_pos = 0;
	    while ((term_end_pos = terms_str.find_first_of(spliting_tag, term_beg_pos)) != string::npos) {
    	    if (term_end_pos > term_beg_pos) {
        	    string term_str = terms_str.substr(term_beg_pos, term_end_pos - term_beg_pos);
            	feat_vec.push_back(term_str);
        	}
        	term_beg_pos = term_end_pos + 1;
    	}
		if (term_beg_pos < terms_str.size()) {
			string end_str = terms_str.substr(term_beg_pos);
			feat_vec.push_back(end_str);
		}
    	return feat_vec;
	}
}
