#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

const char DELIMITER_CHAR = ' ';

struct family_node {
   string man;
   string woman;
   vector<family_node> offspring;   
   string genealogic;
   family_node* parents;
};


family_node* find_family_node(family_node* family, const string* member)
{
   family_node* searcher = family;

   if (searcher->man.compare(*member) == 0 || searcher->woman.compare(*member) == 0)
   {
      return searcher;
   }
   else
   {
      if (searcher->offspring.empty())
      {
         return nullptr;
      }
      else 
      {
         for (unsigned int pos = 0; pos < searcher->offspring.size(); ++pos)
         {
            auto finder = find_family_node(&searcher->offspring[pos], member);
            if (finder != nullptr) return finder;
         }
      }
   }
}

void set_tree_parenthood(family_node* genealogy_tree)
{
   for (unsigned int pos = 0; pos < genealogy_tree->offspring.size(); ++pos)
   {
      genealogy_tree->offspring[pos].parents = genealogy_tree;
      set_tree_parenthood(&genealogy_tree->offspring[pos]);
   }
}


void build_family_tree(ifstream* file, family_node* genealogy_tree) 
{
   string fileName, line, st_member, nd_member, relation;

   getline(*file, line);
   int n_relations = stoi(line);

   for (int i = 0; i < n_relations; ++i) 
   {
      getline(*file, line);
      stringstream line_stream(line);
      getline(line_stream, st_member, DELIMITER_CHAR);
      getline(line_stream, nd_member, DELIMITER_CHAR);
      getline(line_stream, relation, DELIMITER_CHAR);
      relation.erase(relation.size() - 1); // removing '\r' character

      family_node* node = find_family_node(genealogy_tree, &nd_member);

      if (relation.compare("husband") == 0) 
      {
         if (node == nullptr) 
         {
            genealogy_tree->parents = nullptr;
            genealogy_tree->man = st_member;
            genealogy_tree->woman = nd_member;
         }
         else 
         {
            node->man = st_member;
         }
      }
      else if (relation.compare("son") == 0) 
      {
         family_node member = { st_member, "", vector<family_node>(), st_member };
         node->offspring.push_back(member);
      }
      else if (relation.compare("daughter") == 0) 
      {
         family_node member = { "", st_member, vector<family_node>(), st_member };
         node->offspring.push_back(member);
      }
      else if (relation.compare("wife") == 0) 
      {
         node->woman = st_member;
      }
   }

   set_tree_parenthood(genealogy_tree);
}


void print_family_tree(const family_node* genealogy_tree, int level)
{
   if (genealogy_tree != nullptr)
   {
      string mark = "-";
      for (int l = 0; l < level; ++l) mark += "-";
      cout << genealogy_tree->man << mark << genealogy_tree->woman << endl;
      ++level;

      for (family_node node : genealogy_tree->offspring)
      {
         print_family_tree(&node, level);            
      }
   }
}

bool inline is_woman_of_node(family_node* node, string member)
{
   return node->woman.compare(member) == 0;
}

bool inline is_blood_child(family_node* node, string member)
{
   return node->genealogic.compare(member) == 0;
}

bool inline has_same_parent_node(family_node* st_node, family_node* nd_node)
{
   return st_node->parents != nullptr && nd_node->parents != nullptr && 
      st_node->parents->man.compare(nd_node->parents->man) == 0;
}

bool inline has_same_grandparent_node(family_node* st_node, family_node* nd_node)
{ 
   return st_node->parents != nullptr && st_node->parents->parents != nullptr &&
      nd_node->parents != nullptr && nd_node->parents->parents != nullptr &&
      st_node->parents->parents->man.compare(nd_node->parents->parents->man) == 0;
}

bool inline is_cousing(family_node* st_node, family_node* nd_node, string st_name, string nd_name)
{
   return has_same_grandparent_node(st_node, nd_node) && 
      is_blood_child(st_node, st_name) && is_blood_child(nd_node, nd_name);
}

bool inline is_aunt(family_node* st_node, family_node* nd_node, string st_name, string nd_name)
{
   return has_same_parent_node(st_node, nd_node->parents) && is_blood_child(st_node, st_name) &&
      is_blood_child(nd_node, nd_name);
}

void process_queries(ifstream* file, family_node* genealogy_tree)
{
   string line, st_member, nd_member;

   getline(*file, line);
   int n_queries = stoi(line);

   for (int query = 0; query < n_queries; ++query) 
   {
      getline(*file, line);
      stringstream line_stream(line);
      getline(line_stream, st_member, DELIMITER_CHAR);
      getline(line_stream, nd_member, DELIMITER_CHAR);
      if (query < n_queries - 1) nd_member.erase(nd_member.size() - 1); // removing '\r' character

      auto st_node = find_family_node(genealogy_tree, &st_member);
      auto nd_node = find_family_node(genealogy_tree, &nd_member);

      if (is_cousing(st_node, nd_node, st_member, nd_member))
      {
         cout << st_member << " and " << nd_member << " are 1st cousins" << endl;
      }
      else if (is_cousing(st_node, nd_node->parents, st_member, nd_node->parents->man))
      {
         cout << st_member << " and " << nd_member << " are 1st cousins 1-time removed" << endl;
      }
      else if (has_same_grandparent_node(st_node, nd_node) && 
         is_blood_child(st_node, st_member) && !is_blood_child(nd_node, nd_member))
      {
         cout << st_member << " and " << nd_member << " are 1st cousins-in-law" << endl;
      }
      else if (has_same_parent_node(st_node, nd_node) && 
         !is_blood_child(st_node, st_member) && is_blood_child(nd_node, nd_member) &&
         is_woman_of_node(st_node, st_member))
      {
         cout << st_member << " is the sister-in-law of " << nd_member << endl;
      }
      else if (st_node->woman.compare(nd_node->parents->woman) == 0 && !is_blood_child(st_node, st_member) &&
         !is_blood_child(nd_node, nd_member))
      {
         cout << st_member << " is the mother-in-law of " << nd_member << endl;
      }
      else if (st_node->woman.compare(nd_node->parents->woman) == 0 && !is_blood_child(st_node, st_member) &&
         is_blood_child(nd_node, nd_member))
      {
         cout << st_member << " is the mother of " << nd_member << endl;
      }
      else if (is_aunt(st_node, nd_node, st_member, nd_member)) 
      {
         cout << st_member << " is the aunt of " << nd_member << endl;
      }
      else if ( has_same_parent_node(st_node, nd_node->parents) && !is_blood_child(st_node, st_member) &&
      is_blood_child(nd_node, nd_member)) 
      {
         cout << st_member << " is the uncle-in-law of " << nd_member << endl;
      }
      else if (is_aunt(nd_node, st_node, nd_member, st_member)) 
      {
         cout << st_member << " is the niece of " << nd_member << endl;
      }
      else if (has_same_parent_node(st_node->parents, nd_node) && is_blood_child(st_node, st_member) &&
         !is_blood_child(nd_node, nd_member)) 
      {
         cout << st_member << " is the niece-in-law of " << nd_member << endl;
      }
   }
}

int main()
{
   string fileName, line;

   cout << "Enter file name: ";
   cin >> fileName;
   cout << endl;

   ifstream relationsFile(fileName);
   family_node genealogy_tree = { "", "", vector<family_node>() };
   build_family_tree(&relationsFile, &genealogy_tree);
   process_queries(&relationsFile, &genealogy_tree);

   relationsFile.close();
}