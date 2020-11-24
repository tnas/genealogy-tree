#include <iostream>
#include <fstream>
#include <map>
#include <string>

using namespace std;

void build_family_tree(ifstream* file) 
{
   string fileName, line;
   getline(*file, line);
   
   int n_relations = stoi(line);
   cout << "Number of relations: " << n_relations << endl;

   for (int i = 0; i < n_relations; ++i) 
   {
      getline(*file, line);
      cout << line << endl;
   }
}



int main()
{
   string fileName, line;

   cout << "Enter file name: ";
   cin >> fileName;
   cout << endl;

   ifstream relationsFile(fileName);
   build_family_tree(&relationsFile);

   getline(relationsFile, line);
   int n_queries = stoi(line);
   cout << "Number of relations: " << n_queries << endl;

   for (int i = 0; i < n_queries; ++i) 
   {
      getline(relationsFile, line);
      cout << line << endl;
   }  

   relationsFile.close();
}