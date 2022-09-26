#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <bitset>

using namespace std;
//--------------------------------------------------------------------

string cmd;
string lineread;
int R[32]; //registers 1 to 31

void helpCMD ()
{
  cout<<"\nLists of possible commands: \n"
      <<"* help            - displays the lists of commands \n"
      <<"* Rn [value]      - set value of a register\n"
      <<"* file [filename] - opens the RISC-V instruction file\n"
      <<"* next            - instructs the program to read the "
      <<"next command in the file\n"
      <<"* exit            - closes the program"<<endl;
}

string fileopen (string filename, int n)
{
  string line;
  line.clear();
  ifstream file (filename);
  if (file.is_open())
  {
    for (int i = 0; i < n; i++)
    {
      getline(file, line);
    }
    cout << "Reading Line "<<n<<":"<<line<< endl;
    file.close();
    //cout<<"Done\n";
  }
  else cout<<"Unable to open file."<<endl;

  stringstream ss;
    ss << hex << line;
    unsigned raw32;
    ss >> raw32;

    bitset<32> b(raw32); //raw 32-bit line
    string converted = b.to_string();
    //cout << converted << endl;

  return converted;
}


int main()
{
  int n = 0;
  string filename, hextobin;

  do{
    cout<<"\n[CMD] : ";
    getline(cin, cmd);
    //cout<<cmd<<endl;
    size_t ffound = cmd.find("file");
    size_t nxfound = cmd.find("next");
    //cout<<"ffound = "<<ffound<<endl;
    //cout<<"rfound = "<<rfound<<endl;
    if (cmd == "help")
    {
      helpCMD();
    }

    else if (ffound == 0)
    {
      n = 0;
      n++;
      string word;
      filename.clear();
      stringstream name(cmd);
      while (name >> word)
      {
        if (word == "file")
        {
          continue;
        }
        else
        {
          //cout<<word<<endl;
          filename = filename + word + ' ';
          //cout<<filename<<endl;
        }
      }
      cout<<"Opening file: "<<filename<<endl;
      hextobin = fileopen(filename, n);
      cout<<hextobin<<endl;
    }

    else if (nxfound == 0)
    {
      n++;
      hextobin = fileopen(filename, n);
      cout<<hextobin<<endl;
    }

    else if (cmd.at(0) == 'R')
    {
      int sfound = cmd.find(" ");
      string reg = cmd.substr(1, sfound);
      string val = cmd.substr(sfound +1);
      cout<<"Changing register value of "<<reg<<endl;
      cout<<"Value: "<<val<<endl;
    }
  }
  while (cmd != "exit");
  return 0;
}
