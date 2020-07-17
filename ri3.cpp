/*
Copyright (c) 2014 by Rosalba Giugno

This library contains portions of other open source products covered by separate
licenses. Please see the corresponding source files for specific terms.

RI is provided under the terms of The MIT License (MIT):

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

#include <stdio.h>
#include <stdlib.h>
#include "c_textdb_driver.h"
#include "timer.h"

#include <sys/resource.h>



#include "AttributeComparator.h"
#include "AttributeDeconstructor.h"
#include "Graph.h"

// Centrality measures
#include "Centrality.h"
#include "FreqCentrality.h"
#include "InfoCentrality.h"
#include "SubCentrality.h"
#include "EigCentrality.h"
#include "LacCentrality.h"
#include "NetCentrality.h"
#include "DegCentrality.h"
#include "ClCentrality.h"
#include "BetCentrality.h"
// -----------------------


#include "MatchingMachine.h"
#include "MaMaConstrFirst.h"
#include "Match.h"
#include "Solver.h"
#include "IsoGISolver.h"
#include "SubGISolver.h"
#include "InducedSubGISolver.h"


#include "LabelMap.h"

//#define PRINT_MATCHES
//#define CSV_FORMAT


using namespace rilib;


void usage(char* args0);
int match(MATCH_TYPE matchtype, GRAPH_FILE_TYPE filetype,	std::string& referencefile,	std::string& queryfile, int version);

int main(int argc, char* argv[]){

	if(argc!=6){
		usage(argv[0]);
		return -1;
	}

	MATCH_TYPE matchtype;
	GRAPH_FILE_TYPE filetype;
	std::string reference;
	std::string query;
	int version = -1;

	std::string par = argv[1];

	if(par == "ri")
		version = 0;
	else if(par == "info")
		version = 1;
	else if(par == "freq")	
	 	version = 2;
	else if(par == "sub")
		version = 3;
	else if(par == "eig")
		version = 4;
	else if(par == "lac")
		version = 5;
	else if(par == "net")
		version = 6;
	else if(par == "deg")	
		version = 7;
	else if(par == "cl")
		version = 8;
	else if(par == "bet")
		version = 9;
	else{
		usage(argv[0]);
		return -1;
	}

	par = argv[2];
	if(par=="iso"){
		matchtype = MT_ISO;
	}
	else if(par=="ind"){
		matchtype = MT_INDSUB;
	}
	else if(par=="mono"){
		matchtype = MT_MONO;
	}
	else{
		usage(argv[0]);
		return -1;
	}

	par = argv[3];

	if(par=="syn"){
		filetype = GFT_SYN;
	}
	else if(par=="gfu"){
		filetype = GFT_GFU;
	}
	else if(par=="gfd"){
		filetype = GFT_GFD;
	}
	else if(par=="geu"){
		filetype = GFT_EGFU;
	}
	else if(par=="ged"){
		filetype = GFT_EGFD;
	}
	else if(par=="vfu"){
		filetype = GFT_VFU;
	}
	else{
		usage(argv[0]);
		return -1;
	}

	if(filetype == GFT_SYN && (version == 2 || version == 7)){
		cout << "Error!You can not use freq/deg version with synthetic dataset." << endl;
		return -1;
	}	

	reference = argv[4];
	query = argv[5];

	return match(matchtype, filetype, reference, query, version);
};





void usage(char* args0){
	std::cout<<"usage "<<args0<<" [ri info freq sub eig lac net deg cl bet] [iso ind mono] [syn gfu gfd geu ged vfu] reference query\n";
	std::cout<<"\tmatch type:\n";
	std::cout<<"\t\tiso = isomorphism\n";
	std::cout<<"\t\tind = induced subisomorphism\n";
	std::cout<<"\t\tmono = monomorphism\n";
	std::cout<<"\tgraph input format:\n";
	std::cout<<"\t\tsyn = synthetic dataset\n";	
	std::cout<<"\t\tgfu = undirect graphs with labels on nodes\n";
	std::cout<<"\t\tgfd = direct graphs with labels on nodes\n";
	std::cout<<"\t\tgeu = undirect graphs with labels both on nodes and edges\n";
	std::cout<<"\t\tged = direct graphs with labels both on nodes and edges\n";
	std::cout<<"\t\tvfu = VF2Lib undirect unlabeled format\n";
	std::cout<<"\treference file contains one or more reference graphs\n";
	std::cout<<"\tquery contains the query graph (just one)\n";
};


int match(
		MATCH_TYPE 			matchtype,
		GRAPH_FILE_TYPE 	filetype,
		std::string& 		referencefile,
		std::string& 	queryfile,
		int version){



	TIMEHANDLE load_s, load_s_q, make_mama_s, match_s, total_s;
	double load_t=0;double load_t_q=0; double make_mama_t=0; double match_t=0; double total_t=0;
	total_s=start_time();

	bool takeNodeLabels = false;
	bool takeEdgesLabels = false;
	int rret;

	AttributeComparator* nodeComparator;			//to compare node labels
	AttributeComparator* edgeComparator;			//to compare edge labels
	AttributeDeconstructor* nodeAttrDeco;			//to free node labels
	AttributeDeconstructor* edgeAttrDeco;			//to free edge labels
	switch(filetype){
		case GFT_SYN:
		case GFT_GFU:
		case GFT_GFD:
			// only nodes have labels and they are int
			//nodeComparator = new StringAttrComparator();
			nodeComparator = new IntAttrComparator();
			edgeComparator = new DefaultAttrComparator();
			//nodeAttrDeco = new StringAttrDeCo();
			nodeAttrDeco = new IntAttrDeCo();
			edgeAttrDeco = new VoidAttrDeCo();
			takeNodeLabels = true;
			break;
		//case GFT_EGFU:
		/*case GFT_EGFD:
			//labels on nodes and edges, both of them are strings
			nodeComparator = new StringAttrComparator();
			edgeComparator = new StringAttrComparator();
			nodeAttrDeco = new StringAttrDeCo();
			edgeAttrDeco = new StringAttrDeCo();
			takeNodeLabels = true;
			takeEdgesLabels = true;
			break;
		case GFT_VFU:
			//no labels
			nodeComparator = new DefaultAttrComparator();
			edgeComparator = new DefaultAttrComparator();
			nodeAttrDeco = new VoidAttrDeCo();
			edgeAttrDeco = new VoidAttrDeCo();
			break;*/
	}

	TIMEHANDLE tt_start;
	double tt_end;


	// map label to int
	LabelMap *lm;
	lm = new LabelMap();


	//read the query graph
	load_s_q=start_time();
	Graph *query = new Graph();
	rret = read_graph(queryfile.c_str(), query, filetype, *lm, false, 0, query->label_adj_matrix, version);
	load_t_q+=end_time(load_s_q);
	if(rret !=0){
		std::cout<<"Error on reading query graph!\n";
		exit(1);
	}

	//mama->print();

	long 	steps = 0,				//total number of steps of the backtracking phase
			triedcouples = 0, 		//nof tried pair (query node, reference node)
			matchcount = 0, 		//nof found matches
			matchedcouples = 0;		//nof mathed pair (during partial solutions)
	long tsteps = 0, ttriedcouples = 0, tmatchedcouples = 0;

	int flag = 0;

	std::ifstream in;
	FILE* fd;


	if(filetype != GFT_SYN){
		fd = open_file(referencefile.c_str(), filetype);
		if(fd != NULL){
			flag = 1;
		}
	}
	else{
		open_ifstream(in, referencefile.c_str());
		if(in.good()){
			flag = 1;
		}
	}

	if(flag){
#ifdef PRINT_MATCHES
		//to print found matches on screen
		MatchListener* matchListener=new ConsoleMatchListener();
#else
		//do not print matches, just count them
		MatchListener* matchListener=new EmptyMatchListener();
#endif
		int i=0, rret = -1;
		bool rreaded = true;


		do{//for each reference graph inside the input file
#ifdef PRINT_MATCHES
			std::cout<<"#"<<i<<"\n";
#endif

			//read the next reference graph
			load_s=start_time();

			Graph *rrg = new Graph();

			if(filetype != GFT_SYN){
				rret = read_dbgraph(referencefile.c_str(), fd, rrg, filetype, *lm, true, query->labels, query->label_adj_matrix, version);			
			}
			else{
				rret = read_dbgraph(referencefile.c_str(), in, rrg, filetype, *lm, true, query->labels, query->label_adj_matrix, version);			
			}

			rreaded = (rret == 0);
			load_t+=end_time(load_s);
			if(rreaded){
					make_mama_s=start_time();

					Centrality *m;
					switch(version){
						case 0: m = NULL;break;
						case 1: m = new InfoCentrality(*query, *rrg);break;
						case 2: m = new FreqCentrality(*query, *rrg);break;
						case 3: m = new SubCentrality(*query, *rrg);break;
						case 4: m = new EigCentrality(*query, *rrg);break;
						case 5: m = new LacCentrality(*query, *rrg);break;
						case 6: m = new NetCentrality(*query, *rrg);break;
						case 7: m = new DegCentrality(*query, *rrg);break;
						case 8: m = new ClCentrality(*query, *rrg);break;
						case 9: m = new BetCentrality(*query, *rrg);break;
					}

					// Change to use different centrality measures					
					//Centrality *m = new InfoCentrality(*query, *rrg);
					
					MaMaConstrFirst* mama = new MaMaConstrFirst(*query);
					mama->build(*query, m);					

					make_mama_t+=end_time(make_mama_s);

					//run the matching

					match_s=start_time();
					match(	*rrg,
							*query,
							*mama,
							*matchListener,
							matchtype,
							*nodeComparator,
							*edgeComparator,
							&tsteps,
							&ttriedcouples,
							&tmatchedcouples);
					match_t+=end_time(match_s);

					//see rilib/Solver.h
//					steps += tsteps;
					triedcouples += ttriedcouples;
					matchedcouples += tmatchedcouples;

				}
//				delete rrg;
				//remember that destroyer are not defined
			i++;

			if(filetype == GFT_SYN) break;

		}while(rreaded);

		matchcount = matchListener->matchcount;

		delete matchListener;

		if(filetype != GFT_SYN)
			fclose(fd);
	}
	else{
		std::cout<<"unable to open reference file\n";
		return -1;
	}

	total_t=end_time(total_s);

#ifdef CSV_FORMAT

	std::cout<<referencefile<<"\t"<<queryfile<<"\t";
	std:cout<<load_t_q<<"\t"<<make_mama_t<<"\t"<<load_t<<"\t"<<match_t<<"\t"<<total_t<<"\t"<<steps<<"\t"<<triedcouples<<"\t"<<matchedcouples<<"\t"<<matchcount;
#else

	std::cout<<"reference file: "<<referencefile<<"\n";
	std::cout<<"query file: "<<queryfile<<"\n";
	std::cout<<"total time: "<<total_t<<"\n";
	std::cout<<"matching time: "<<match_t<<"\n";
	std::cout<<"number of found matches: "<<matchcount<<"\n";
	std::cout<<"search space size: "<<matchedcouples<<"\n";

	std::cout<<"tried couples: "<<triedcouples<<"\n";
	
#endif

//	delete mama;
//	delete query;

	delete nodeComparator;
	delete edgeComparator;

	return 0;
};





