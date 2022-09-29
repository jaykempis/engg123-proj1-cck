#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <bitset>

using namespace std;
//--------------------------------------------------------------------

string cmd;
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

  return line;
}

void opI(unsigned rawLine){
  //extraciton of fields
  unsigned opcode = rawLine & 0x0000007F; //extracted OP-code
  unsigned r_dest = (rawLine & 0x00000F80) >> 7; //extracted rd
  unsigned funct_3 = (rawLine & 0x00007000) >> 12; //extracted f3
  unsigned r_src1 = (rawLine & 0x000F8000) >> 15; //extracted rs1
  unsigned immediate = (rawLine & 0xFFF00000) >> 20; //extracted imm

  //fields
  bitset<7> op(opcode);
  bitset<5> rd(r_dest);
  bitset<3> f3(funct_3);
  bitset<5> rs1(r_src1);
  bitset<12> imm(immediate);

  //outputs
  cout << op.to_string() << endl;
  cout << rd.to_string() << endl;
  cout << f3.to_string() << endl;
  cout << rs1.to_string() << endl;
  cout << imm.to_string() << endl;

  //storing values in memory
  //reg[rs1.to_ulong()] = 46;

  //converting signed 12-bit to long
    int16_t x_imm = (int16_t)(imm.to_ulong() & 0xFFF) << 4; //interprets the unsigned 12-bit bitset into a signed
    x_imm = x_imm >> 4;                                     //16-bit set then converts it from bitset to long
    //bitset<12> test_imm(x_imm);                             //the long is currently 16 bits long therefore we shift-right
    //cout << test_imm.to_string() << endl;                   //to extract the signed 12-bit from the signed 16-bit

    //sample orperation
    switch(f3.to_ulong()){
        case 0b000: //ADD I
            //cout << "Immediate Signed Value: " << x_imm << endl;
            R[rd.to_ulong()] = R[rs1.to_ulong()] + x_imm;
            cout << "PSEUDO addi R" << rd.to_ulong() <<
          " , R" <<rs1.to_ulong() << " , " << x_imm << "   |   R" << rd.to_ulong() << ": ";
            cout << R[rd.to_ulong()] << endl;
        break;

        default:
            cout << "error";
        break;
    }

}


void opR(unsigned rawline){
    //extraction
    unsigned opcode = rawline & 0x0000007F; //extracted OP-code
    unsigned r_dest = (rawline & 0x00000F80) >> 7; //extracted rd
    unsigned funct_3 = (rawline & 0x00007000) >> 12; //extracted f3
    unsigned r_src1 = (rawline & 0x000F8000) >> 15; //extracted rs1
    unsigned r_src2 = (rawline & 0x01F00000) >> 20; //extracted rs1
    unsigned funct_7 = (rawline & 0xFE000000) >> 25; //extracted rs1

    //fields
    bitset<7> op(opcode);
    bitset<5> rd(r_dest);
    bitset<3> f3(funct_3);
    bitset<5> rs1(r_src1);
    bitset<5> rs2(r_src2);
    bitset<7> f7(funct_7);

    //storing values in memory
    //R[rs1.to_ulong()] = 46;
    //R[rs2.to_ulong()] = 23;

    //sample orperation
    switch(f3.to_ulong()){
        case 0b000: //add or sub
            if(f7.to_ulong() == 0b0000000){  //add
              R[rd.to_ulong()] = R[rs1.to_ulong()] + R[rs2.to_ulong()];
              cout << "PSEUDO add R" << rd.to_ulong() <<
              " , R" <<rs1.to_ulong() << " , R" << rs2.to_ulong() << "   |   R" << rd.to_ulong() << ": ";
            }
            else if(f7.to_ulong() == 0b0100000){ //sub
              R[rd.to_ulong()] = R[rs1.to_ulong()] - R[rs2.to_ulong()];
              cout << "PSEUDO sub R" << rd.to_ulong() <<
              " , R" <<rs1.to_ulong() << " , R" << rs2.to_ulong() << "   |   R" << rd.to_ulong() << ": ";
            }
            //cout << rd.to_string() << endl;
            //cout << R[rs1.to_ulong()] << endl;
            //cout << R[rs2.to_ulong()] << endl;
            cout << R[rd.to_ulong()] << endl;
        break;

        case 0b110: //bitwise OR
          R[rd.to_ulong()] = R[rs1.to_ulong()] | R[rs2.to_ulong()];
          cout << "PSEUDO or R" << rd.to_ulong() <<
          " , R" <<rs1.to_ulong() << " , R" << rs2.to_ulong() << "   |   R" << rd.to_ulong() << ": ";
            cout << R[rd.to_ulong()] << endl;
        break;

        case 0b111: //bitwise AND
          R[rd.to_ulong()] = R[rs1.to_ulong()] & R[rs2.to_ulong()];
          cout << "PSEUDO and R" << rd.to_ulong() <<
          " , R" <<rs1.to_ulong() << " , R" << rs2.to_ulong() << "   |   R" << rd.to_ulong() << ": ";
            cout << R[rd.to_ulong()] << endl;
        break;

        case 0b001:
          R[rd.to_ulong()] = R[rs1.to_ulong()] << rs2.to_ulong();
          cout << "PSEUDO Shift Left Logical sll R" << rd.to_ulong() <<
          " , R" << rs1.to_ulong() << " , x" << rs2.to_ulong() << "   |   R" << rd.to_ulong() << ": ";
            cout << R[rd.to_ulong()] << endl;
        break;

        case 0b101:
          R[rd.to_ulong()] = R[rs1.to_ulong()] >> rs2.to_ulong();
          cout << "PSEUDO Shift Right Logical srl R" << rd.to_ulong() <<
          " , R" << rs1.to_ulong() << " , x" << rs2.to_ulong() << "   |   R" << rd.to_ulong() << ": ";
            cout << R[rd.to_ulong()] << endl;
        break;

        default:
            cout << "ERROR: Instruction Not Found in Library.";
        break;
    }


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
      string lineread;
      lineread = fileopen(filename, n);
      n++;

      stringstream ss;
      ss << hex << lineread;
      unsigned raw32;
      ss >> raw32;

      bitset<32> b(raw32); //raw 32-bit line
      cout << "Instruction: " << b.to_string() << endl; //output raw32

      //extraction
      unsigned opcode = raw32 & 0x0000007F; //extracted OP-code
      bitset<7> op(opcode);
      cout << "OP Code: " << op.to_string() << endl;

      switch(op.to_ulong()){
         case 0b0110011:
            opR(raw32);
         break;
         case 0b0010011:
            opI(raw32);
         break;

         default:
          cout << "error";
         break;
      }



    }

    else if (cmd.at(0) == 'R')
    {
      int sfound = cmd.find(" ");
      string reg = cmd.substr(1, sfound);
      string val = cmd.substr(sfound +1);
      cout<<"Changing register value of R"<<reg <<endl;
      R[stoi(reg)] = stoi(val);
      cout<< "R" << reg << ": " << val <<endl;
    }
  }
  while (cmd != "exit");
  return 0;
}
