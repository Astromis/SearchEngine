#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <list>
#include <queue>
#include <thread>
#include <mutex>
using namespace std;

// this vector contatins an order number of word in text 
typedef vector<int> position_vector;
// this map contains documents as a key and position vectors as a value
typedef map<string,position_vector> word_position_map;
typedef map <string, word_position_map > inverted_list;

mutex mut;

bool get_word_position_vector(word_position_map *response, inverted_list &index, string &quary)
{
    inverted_list::iterator it = index.find(quary);
    if(it != index.end())
    {
        *response = it->second;
        return true;
    }
    else return false;
}

bool get_position_vector(position_vector *response, word_position_map &data, string &quary)
{
    word_position_map::iterator it = data.find(quary);
    if(it != data.end())
    {
        *response = it->second;
        return true;
    }
    else return false;
}


int getdir (const string ext, const string dir, vector<string> &files, queue<string> &dirs)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL)
    {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        if(dirp->d_type == DT_DIR && string(dirp->d_name) != "." && string(dirp->d_name) != "..")

            dirs.push(dir+"/"+string(dirp->d_name));
        else
        //This condition for the exact file extension. If it is not present, all file names to vector will be append.
        if(ext != "")
        {
            if(string(dirp->d_name).find(ext+"\0") != -1)
                files.push_back(dir+"/"+string(dirp->d_name));
        }
        else
            files.push_back(dir+"/"+string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

bool get_dirs(const string ext, const string start_dir, vector<string> &files)
{
    queue<string> dirs = queue<string>();
    string next_dir;

    getdir(ext, start_dir, files, dirs);
    while(dirs.size() != 0)
    {
        next_dir = dirs.front();
        dirs.pop();
        getdir(ext, next_dir, files, dirs);
    }
    return true;
}

void buildindex(inverted_list &inv, vector<string> &f)
{
    for (unsigned int i = 0;i < f.size();i++)
    {
        ifstream in;
        in.open(f[i].c_str(), ifstream::in);
        string word;

        while (in >> word)
        {
            //add conditions, that cut all
            mut.lock();
            inv[word][f[i].c_str()].push_back((int)(in.tellg()) - (word.length()));
            mut.unlock();
        }
    }
    cout<<"Indexing complete in thread id "<<this_thread::get_id()<<". Size: "<< inv.size()<<endl;   
}

void threadIndexing(vector<string> &files, inverted_list &index)
{
    vector<string> a(files.begin(), files.begin() + files.size()/ 2);
    vector<string> b(files.begin() + files.size()/ 2, files.end());

    thread thread1(buildindex, ref(index), ref(a));
    thread thread2(buildindex, ref(index), ref(b));
    thread1.join();
    thread2.join();
}


int main(int argc, char* argv[])
{

    if(argc < 2 )
    {
        cout<<"Error arguments!"<<endl;
        cout<<"/.test [dir] [string]"<<endl;
        return 0;
    }
    string dir = string(argv[1]);
    vector<string> files = vector<string>();
    vector<string> dirs = vector<string>();
    get_dirs(string(argv[2]), dir, files);

 	inverted_list index;
    /*inverted_list index1;
    vector<string> a(files.begin(), files.begin() + files.size()/ 2);
    vector<string> b(files.begin() + files.size()/ 2, files.end());

    thread thread1(buildindex, ref(index), ref(a));
    thread thread2(buildindex, ref(index1), ref(b));
    thread1.join();
    thread2.join();*/

    for (unsigned int i = 0;i < files.size();i++)
    {
        ifstream in;
        //doesnt open files in other dirs
        in.open(files[i].c_str(), ifstream::in);
        string word;

        while (in >> word)
        {
            //add conditions, that cut all
            index[word][files[i]].push_back((int)(in.tellg()) - (word.length()));
        }
    } 


    cout<<"Indexing complete. Size: "<< index.size()<<endl;//<<" "<< index1.size()<<endl;
    cout<<"Indexing complete. Size: "<< files.size()<<endl;
/*     for(auto& il : index)
        cout<<il.first<<endl; */   


//    inverted_list::iterator it;
//    map<string, list<int> >::iterator it2;
//    word_position::iterator it3;
    //int asd = index.find("волновая")->second.find("_Волновая физика.txt")->second.back();
//    cout<< asd;
    /*for(it2 = asd.begin();it2 != asd.end(); it2++)
    {
        cout<< it2;
        break;
    }*/
    /*
	map <string, map<string, list<int> > >::iterator cur;
	map<string, list<int> >::iterator cur1;
	list<int>::iterator cur2;
	cout << "{\n";
	//General cycle for writing words
	for (cur = index.begin(); cur != index.end(); cur++)
	{
		cout << "\t\"" << (*cur).first << "\" : ";
        cout << "{\n";
		//Cycle for writing filenames, contained as a dictionary
		for (cur1 = (*cur).second.begin(); cur1 != (*cur).second.end(); cur1++)
		{

            cout<< "\t\t\""<<(*cur1).first<<"\"";
            cout << " : [";
			 // Cycle for writing position values, which contained in list
			for (cur2 = (*cur1).second.begin(); cur2 != (*cur1).second.end(); cur2++)
			{
				cout << (*cur2);
				cur2++;
				if (cur2 != (*cur1).second.end())
				{
					cout << ", ";
				}
				else cout << "]";
				cur2--;
			}
			cur1++;
			if (cur1 != (*cur).second.end())
			{
				cout << ",\n";
			}
			cur1--;

		}
		cout << "\n\t}";
		cur++;
		if (cur != index.end())
		{
			cout << ",\n";
		}

		cur--;
		cout <<endl;
	}
	cout<<"}";
    */
    return 0;
}
