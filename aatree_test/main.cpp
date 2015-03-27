#include <stdio.h>
#include <fstream>
#include "../aatree.h"

int main(int argc, char **argv)
{
	AATree at;
	
	for (int i=1; i<=13; i++) {
		printf ("Insert %d\n", i);
		AADataInt *d = new AADataInt(i);
		at.insert(d);
	}
	at.prettyPrint();
	
	int ret;
	// save tree
	printf ("Save tree\n");
	
	ofstream of;
	of.open("mytree.bin", ios_base::out | ios_base::binary);
	if (!of.is_open()) {
		return errno;
	}
	if ((ret = at.save(of)) != 0) {
		printf ("Failed to save the tree: %d\n", ret);
		return ret;
	}
	of.close();
	
	// load tree
	ifstream f;
	f.open("mytree.bin", ios_base::in | ios_base::binary);
	if (!f.is_open()) {
		return errno;
	}
	printf ("Load tree\n");
	if ((ret = at.load(f, createAADataInt)) != 0) {
		printf ("Failed to load the tree: %d\n", ret);
		return ret;
	}
	f.close();
	// print
	printf ("Print tree\n");
	at.prettyPrint();
	
	return 0;
}
