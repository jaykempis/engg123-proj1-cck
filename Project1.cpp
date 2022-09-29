//ENGG 123.01
//Project 1: Interactive Basic RISC V Simulator
//by: Iris Carson, Antonio Castro, and Joshua Kempis
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <bitset>

using namespace std;
//--------------------------------------------------------------------

void helpCMD ()
{
  cout<<"\nLists of possible commands: \n"
      <<"* help            - displays the lists of commands \n"
      <<"* show            - displays the current values stored "
      <<"in registers"
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
  }
  else cout<<"Unable to open file."<<endl;

  return line;
}

//Function for I-type Instructions
void opI(unsigned rawLine, long long int R[])
{
  //extraciton of fields
  unsigned opcode=rawLine&0x0000007F; //extracted OP-code
  unsigned r_dest=(rawLine&0x00000F80)>> 7; //extracted rd
  unsigned funct_3=(rawLine&0x00007000)>> 12; //extracted f3
  unsigned r_src1=(rawLine&0x000F8000)>> 15; //extracted rs1
  unsigned immediate=(rawLine&0xFFF00000)>> 20; //extracted imm

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

  //converting signed 12-bit to long
  //interprets the unsigned 12-bit bitset into a signed value
  int16_t x_imm = (int16_t)(imm.to_ulong() & 0xFFF) << 4;

  //16-bit set then converts it from bitset to long
  x_imm = x_imm >> 4;
  cout << "x_imm: " << x_imm << endl;

  //bitset for immediate shift
  bitset<5> shift_imm(x_imm); 

  //sample orperation
  switch(f3.to_ulong())
  {
    case 0b000: //ADD I
      R[rd.to_ulong()] = R[rs1.to_ulong()] + x_imm;
      cout << "PSEUDO addi R" << rd.to_ulong()
           << " , R" <<rs1.to_ulong() << " , "
           << x_imm << "   |   R" << rd.to_ulong() << ": ";
      cout << R[rd.to_ulong()] << endl;
    break;

    case 0b010: //SLTI
      if (R[rs1.to_ulong()] < x_imm)
      {
        R[rd.to_ulong()] = 1;
      }
      else
      {
        R[rd.to_ulong()] = 0;
      }
      cout << "PSEUDO slti R" << rd.to_ulong()
           << " , R" <<rs1.to_ulong() << " , "
           << x_imm << "   |   R" << rd.to_ulong() << ": ";
      cout << R[rd.to_ulong()] << endl;
    break;

    case 0b110: //OR I
      R[rd.to_ulong()] = R[rs1.to_ulong()] | x_imm;
      cout << "PSEUDO ori R" << rd.to_ulong()
           << " , R" <<rs1.to_ulong() << " , "
           << x_imm << "   |   R" << rd.to_ulong() << ": ";
      cout << R[rd.to_ulong()] << endl;
    break;

    case 0b11: //AND I
      R[rd.to_ulong()] = R[rs1.to_ulong()] & x_imm;
      cout << "PSEUDO andi R" << rd.to_ulong()
           << " , R" <<rs1.to_ulong() << " , "
           << x_imm << "   |   R" << rd.to_ulong() << ": ";
      cout << R[rd.to_ulong()] << endl;
    break;

    case 0b001: //SHIFT LEFT LOGICAL IMMEDIATE
      //use only lower 5 bits of imm
      cout << "Shift: " << shift_imm.to_string() << endl;
      R[rd.to_ulong()] = R[rs1.to_ulong()] << shift_imm.to_ulong();
      cout << "PSEUDO slli R" << rd.to_ulong()
           << " , R" <<rs1.to_ulong() << " , "
           << shift_imm << "   |   R" << rd.to_ulong() << ": ";
      cout << R[rd.to_ulong()] << endl;
    break;

    case 0b101: //SHIFT RIGHT LOGICAL  IMMEDIATE
      //use only lower 5 bits of imm
      cout << "Shift: " << shift_imm.to_string() << endl;
      R[rd.to_ulong()] = R[rs1.to_ulong()] >> shift_imm.to_ulong();
      cout << "PSEUDO srli R" << rd.to_ulong()
           << " , R" <<rs1.to_ulong() << " , "
           << shift_imm << "   |   R" << rd.to_ulong() << ": ";
      cout << R[rd.to_ulong()] << endl;
    break;

    default:
      cout << "error";
    break;
  }
}

//Function for R-type Instructions
void opR(unsigned rawline, long long int R[])
{
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

  //sample orperation
  switch(f3.to_ulong())
  {
    case 0b000: //add or sub
      if(f7.to_ulong() == 0b0000000) //ADD
      { 
        R[rd.to_ulong()] = R[rs1.to_ulong()] + R[rs2.to_ulong()];
        cout << "PSEUDO add R" << rd.to_ulong()
             << " , R" <<rs1.to_ulong() << " , R" << rs2.to_ulong()
             << "   |   R" << rd.to_ulong() << ": ";
      }
      else if(f7.to_ulong() == 0b0100000) //SUB
      { 
        R[rd.to_ulong()] = R[rs1.to_ulong()] - R[rs2.to_ulong()];
        cout << "PSEUDO sub R" << rd.to_ulong()
             << " , R" <<rs1.to_ulong() << " , R" << rs2.to_ulong()
             << "   |   R" << rd.to_ulong() << ": ";
      }
      cout << R[rd.to_ulong()] << endl;
    break;

    case 0b110: //bitwise OR
      R[rd.to_ulong()] = R[rs1.to_ulong()] | R[rs2.to_ulong()];
      cout << "PSEUDO or R" << rd.to_ulong()
           << " , R" <<rs1.to_ulong() << " , R" << rs2.to_ulong()
           << "   |   R" << rd.to_ulong() << ": ";
      cout << R[rd.to_ulong()] << endl;
    break;

    case 0b111: //bitwise AND
      R[rd.to_ulong()] = R[rs1.to_ulong()] & R[rs2.to_ulong()];
      cout << "PSEUDO and R" << rd.to_ulong()
           << " , R" <<rs1.to_ulong() << " , R" << rs2.to_ulong()
           << "   |   R" << rd.to_ulong() << ": ";
      cout << R[rd.to_ulong()] << endl;
    break;

    case 0b001: //SHIFT LEFT LOGICAL
      R[rd.to_ulong()] = R[rs1.to_ulong()] << rs2.to_ulong();
      cout << "PSEUDO Shift Left Logical sll R" << rd.to_ulong()
           << " , R" << rs1.to_ulong() << " , x" << rs2.to_ulong()
           << "   |   R" << rd.to_ulong() << ": ";
      cout << R[rd.to_ulong()] << endl;
    break;

    case 0b101: //SHIFT RIGHT LOGICAL
      R[rd.to_ulong()] = R[rs1.to_ulong()] >> rs2.to_ulong();
      cout << "PSEUDO Shift Right Logical srl R" << rd.to_ulong()
           <<" , R" << rs1.to_ulong() << " , x" << rs2.to_ulong()
           << "   |   R" << rd.to_ulong() << ": ";
      cout << R[rd.to_ulong()] << endl;
    break;

    default:
      cout << "ERROR: Instruction Not Found in Library.";
    break;
  }
}

//Function to Show values in all registers
void show(long long int reg[32])
{
  int c = 0;
  cout<<"\t>> Current Values in Registers <<"<<endl;
  while(c < 16)
  {
    cout<<"R"<<c+1<<"\t=\t"<<reg[c]<<"\t | \t";
    cout<<"R"<<c+17<<"\t=\t"<<reg[c+16]<<endl;
    c++;
  }
}


int main()
{
  int n = 0;
  string filename, hextobin, cmd;
  long long int R[32] = {0}; //registers 1 to 31

  do{
    cout<<"\n[CMD] : ";
    getline(cin, cmd);
    size_t ffound = cmd.find("file");
    size_t nxfound = cmd.find("next");
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
          filename = filename + word + ' ';
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
            opR(raw32, R);
         break;
         case 0b0010011:
            opI(raw32, R);
         break;

         default:
          cout << "error";
         break;
      }
    }

    else if (cmd.at(0) == 'R')
    {
      int x;
      int sfound = cmd.find(" ");
      string reg = cmd.substr(1, sfound);
      string val = cmd.substr(sfound +1);
      cout<<"Changing register value of R"<<reg<<endl;

      stringstream ss;
      ss<<val;

      if (ss>>x)
      {
        R[stoi(reg)-1] = x;
        cout<< "R" << reg << ": " << x <<endl;
      }
      else cout<<"Invalid register value."<<endl;
    }

    else if (cmd == "show")
    {
      show(R);
    }

    else cout<<"Invalid Command."<<endl;
  }
  while (cmd != "exit");
  return 0;
}
