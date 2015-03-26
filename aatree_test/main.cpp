#include <stdio.h>
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
	if ((ret = at.save("mytree.bin")) != 0) {
		printf ("Failed to save the tree: %d\n", ret);
		return ret;
	}
	// load tree
	printf ("Load tree\n");
	if ((ret = at.load("mytree.bin", createAADataInt)) != 0) {
		printf ("Failed to load the tree: %d\n", ret);
		return ret;
	}
	// print
	printf ("Print tree\n");
	at.prettyPrint();
	
	return 0;
}
