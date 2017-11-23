//  Jesse Zamazanuk
//  Lookup.cpp
//  Outputs the phone number corresponding to the name given in the phonebook text file given
//  Usage:  lookup <filename> <key name>
//          The passed file should be a text file and the key name a string

#include <map>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
int main(int argc, char* argv[]){
  if(argc!=3){
    cout<<"usage: lookup <filename> <key name>\n";
  }
  else{
    ifstream in;
    map<string, string> phonebook;
    string name, number;
    //Open given file and check that it's open
    do{
      in.clear();
      in.open(argv[1]);
    } while (!in);
    //Read in names and numbers and assign them to pairs in phonebook
    while(in>>name){
      in.ignore(500,' ');
      getline(in,number);
      phonebook[name] = number;
    }
    in.close();
    if(phonebook.count(argv[2])>0)
      cout << phonebook[argv[2]]<<"\n";
    else
      cout << argv[2]<<" not found in the given file\n";
  }
}
