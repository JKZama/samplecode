//  Jesse Zamazanuk
//  area_codes.cpp
//  Outputs a list of unique area codes given a text file phonebook
//  Usage:  area_codes <filename>
//          The passed file should be a text file phonebook

#include <set>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
int main(int argc, char* argv[]){
  if(argc!=2){
    cout<<"usage: area_codes <filename>\n";
  }
  else{
    ifstream in;
    set<string> areacodes;
    string name, areacode;
    //Open given file and check that it's open
    do{
      in.clear();
      in.open(argv[1]);
    } while (!in);
    //Read in names and numbers and assign them to pairs in phonebook
    while(in>>name){
      in.ignore(500,' ');
      in>>areacode;
      in.ignore(500,'\n');
      areacodes.insert(areacode);
    }
    in.close();
    for(auto i : areacodes){
      cout<<i<<endl;
    }
  }
}
