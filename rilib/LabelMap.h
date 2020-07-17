/*
 * LabelMap.h
 *
 *  Created on: Nov 2, 2010
 */

#ifndef LABELMAP_H_
#define LABELMAP_H_

#include <map>
#include <string>
#include <iostream>
#include <fstream>

class LabelMap: public std::map<std::string, int>{
public:
	int getLabel(std::string s){

		LabelMap::iterator IT = this->find(s);

		if(IT == this->end()){
			int label = (int)this->size();
			this->insert(std::pair<std::string,int>(s,label));
			return label;
		}
		else{
			return IT->second;
		}

	}

	friend std::istream& operator>>(std::istream& is, LabelMap& m){
		size_t size;
		is.read((char*)&size,sizeof(std::size_t));
		for(int i=0;i<size;i++){
			std::string s_label;
			int n_label=0;
			is>> s_label;
			is.seekg(((int)is.tellg())+1);;
			is.read((char*)&n_label, sizeof(int));
			m.insert(std::pair<std::string, int>(s_label,n_label));
		}
		return is;
	}
	friend std::ostream& operator<<(std::ostream& os, LabelMap& m){
		std::size_t size = m.size();
		os.write((char*)&(size), sizeof(std::size_t));
		for(LabelMap::iterator IT = m.begin(); IT!=m.end(); IT++){
			os<< IT->first;
			os<<std::endl;
			os.write((char*)&(IT->second), sizeof(int));
		}
		return os;
	}

};

#endif /* LABELMAP_H_ */
