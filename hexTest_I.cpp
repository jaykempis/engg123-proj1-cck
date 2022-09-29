#include<iostream>
#include<sstream>
#include<bitset>
#include<string>

using namespace std;

int main()
{
    long long int reg[64];
    string s = "0xFCE0E793"; //)xFCE is hex
    stringstream ss;  

    ss << hex << s;
    unsigned n;
    ss >> n;

    bitset<32> b(n); //raw 32-bit line
    cout << b.to_string() << endl;

    //extraction
    unsigned opcode = n & 0x0000007F; //extracted OP-code
    unsigned r_dest = (n & 0x00000F80) >> 7; //extracted rd
    unsigned funct_3 = (n & 0x00007000) >> 12; //extracted f3
    unsigned r_src1 = (n & 0x000F8000) >> 15; //extracted rs1
    unsigned immediate = (n & 0xFFF00000) >> 20; //extracted imm

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
    reg[rs1.to_ulong()] = 46;

    //converting signed 12-bit to long
    int16_t x_imm = (int16_t)(imm.to_ulong() & 0xFFF) << 4; //interprets the unsigned 12-bit bitset into a signed
    x_imm = x_imm >> 4;                                     //16-bit set then converts it from bitset to long
    //bitset<12> test_imm(x_imm);                             //the long is currently 16 bits long therefore we shift-right
    //cout << test_imm.to_string() << endl;                   //to extract the signed 12-bit from the signed 16-bit

    //sample orperation
    switch(f3.to_ulong()){
        case 0b000: //ADD I
            //cout << "Immediate Signed Value: " << x_imm << endl;
            reg[rd.to_ulong()] = reg[rs1.to_ulong()] + x_imm;
            cout << "PSEUDO addi R" << rd.to_ulong() <<
          " , R" <<rs1.to_ulong() << " , " << x_imm << "   |   R" << rd.to_ulong() << ": ";
            cout << reg[rd.to_ulong()] << endl;
        break;
        case 0b010: //SLTI
            if (reg[rs1.to_ulong()] < x_imm){
              reg[rd.to_ulong()] = 1;
            }
            else{reg[rd.to_ulong()] = 0;}
            cout << "PSEUDO slti R" << rd.to_ulong() <<
          " , R" <<rs1.to_ulong() << " , " << x_imm << "   |   R" << rd.to_ulong() << ": ";
            cout << reg[rd.to_ulong()] << endl;
        break;
        case 0b110: //ORI
            reg[rd.to_ulong()] = reg[rs1.to_ulong()] | x_imm;
            cout << "PSEUDO ori R" << rd.to_ulong() <<
          " , R" <<rs1.to_ulong() << " , " << x_imm << "   |   R" << rd.to_ulong() << ": ";
            cout << reg[rd.to_ulong()] << endl;
        break;
        case 0b111: //ANDI
            reg[rd.to_ulong()] = reg[rs1.to_ulong()] & x_imm;
            cout << "PSEUDO andi R" << rd.to_ulong() <<
          " , R" <<rs1.to_ulong() << " , " << x_imm << "   |   R" << rd.to_ulong() << ": ";
            cout << reg[rd.to_ulong()] << endl;
        break;
        default:
            cout << "error";
        break;
    }



}