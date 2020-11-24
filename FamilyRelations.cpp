#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main()
{
   string fileName, line;
   int n_relations;
   cout << "Enter file name: ";
   cin >> fileName;
   cout << endl;

   ifstream relationsFile(fileName);

   while (getline(relationsFile, line))
   {
      n_relations = stoi(line);
      cout << "Number of relations: " << n_relations << endl;
      
      for (int i = 0; i < n_relations; ++i) 
      {
         getline(relationsFile, line);
         cout << line << endl;
      }  
   }

   relationsFile.close();
}