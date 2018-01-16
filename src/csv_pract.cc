#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sstream>
using namespace std;

int x=-1;

//Make a global
//declaration of a new variable with reactor_size and inventory_size as the inputs

//Return a vector which includes all of the data points
int main(){

  string sdemand;
  int demand[100];

  x++;
  ifstream myfile("CAISO-demand.csv");

  if(!myfile.is_open()) std::cout << "Error File Open" << '\n';

  for(int i=0; i<100; i++){
    getline(myfile, sdemand, ',');

    stringstream convert(sdemand);
    convert >> demand[i];
    //std::cout << demand[i] << std::endl;

  }

  std::cout << demand[x] << std::endl;

  return(demand[1]);
}
