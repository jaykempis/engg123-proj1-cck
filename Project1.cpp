#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;
//--------------------------------------------------------------------

string cmd;
int R1, R2, RD;

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

void fileopen (string filename, int n)
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
}

void ConvertHexToBin(string hex_input)
{
  long int i = 0;

  while (hex_input[i])
  {
    switch (hex_input[i])
    {
      case '0':
        cout << "0000";
          break;
      case '1':
        cout << "0001";
          break;
      case '2':
        cout << "0010";
          break;
      case '3':
        cout << "0011";
          break;
      case '4':
        cout << "0100";
          break;
      case '5':
        cout << "0101";
          break;
      case '6':
        cout << "0110";
          break;
      case '7':
        cout << "0111";
          break;
      case '8':
        cout << "1000";
          break;
      case '9':
        cout << "1001";
          break;
      case 'A':
      case 'a':
        cout << "1010";
          break;
      case 'B':
      case 'b':
        cout << "1011";
          break;
      case 'C':
      case 'c':
        cout << "1100";
          break;
      case 'D':
      case 'd':
        cout << "1101";
          break;
      case 'E':
      case 'e':
        cout << "1110";
          break;
      case 'F':
      case 'f':
        cout << "1111";
          break;
      default:
        cout << "\nInvalid hexadecimal value " << hex_input[i];
    }
    i++;
  }
}

int main()
{
  int n = 0;
  string filename;

  do{
    cout<<"[CMD] : ";
    getline(cin, cmd);
    //cout<<cmd<<endl;
    size_t ffound = cmd.find("file");
    size_t nxfound = cmd.find("next");
    size_t r1found = cmd.find("R1");
    size_t r2found = cmd.find("R2");
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
      fileopen(filename, n);
    }

    else if (nxfound == 0)
    {
      n++;
      fileopen(filename, n);
    }

    else if (r1found == 0)
    {
      cout<<"Changing register 1 value"<<endl;
    }

    else if (r2found == 0)
    {
      cout<<"Changing register 2 value"<<endl;
    }
  }
  while (cmd != "exit");
  return 0;
}
