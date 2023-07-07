#include "mainwindow.h"

#include "ecgfilter.h"

int main(int argc, char *argv[]) {
	ECGFilter f(argc, argv);
	return f.exec();
}
