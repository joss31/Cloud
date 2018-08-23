#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <vector>
#include <map>

using namespace std;

void countPart(int i, long offset, string filename, map<string, long> * partial_map)
{
	FILE * myfile;
	myfile = fopen(filename.c_str(), "r");
	
	fseek(myfile, i*offset, SEEK_CUR);

	string w = "";
	char c;
	for(long j=i*offset ; j<(offset* (i+1)) ; ++j)
	{
		//w = fgetc(myfile)!=' '?w+c:"";	
		c = fgetc(myfile); 
		if(c != ' ')		
			w += c;
		else
		{
			if((*partial_map)[w])
				(*partial_map)[w]++;
			else
				(*partial_map)[w] = 1;
			w = "";
		}
	}
	cout << partial_map->size() << endl;
	fclose(myfile);
}


int main()
{
	int ThreadNumber = 2;
	vector<thread> vectorThread(ThreadNumber);
	vector<map<string, long> > vectorMap(ThreadNumber);
	string filename = "cuento.txt";

	FILE * myfile;
	myfile = fopen(filename.c_str(), "r");	

	fseek (myfile, 0, SEEK_END);
	long size = ftell(myfile);	
	long offset = size/ThreadNumber;
	fclose(myfile);

	for(int i=0 ; i<ThreadNumber ; ++i)
		vectorThread[i] = thread(countPart, i, offset, filename, &vectorMap[i]);

	for(int i=0 ; i<ThreadNumber ; ++i)
		vectorThread[i].join();

	for(int i=1 ; i<ThreadNumber ; ++i)
	{
		for(auto & it : vectorMap[i])
		{
			if( vectorMap[0][it.first] )
				vectorMap[0][it.first] += it.second;
			else
				vectorMap[0][it.first] = it.second;
		}		
	}

	for(auto &it : vectorMap[0])
		cout << it.first << " : " << it.second << endl;	
	
	cout << "Total words = " << vectorMap[0].size() << endl;

	return 0;
}

