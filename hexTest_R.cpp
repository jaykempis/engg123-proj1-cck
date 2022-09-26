#include<iostream>
#include<sstream>
#include<bitset>
#include<string>

using namespace std;

int main()
{
    long long int reg[64];
    string s = "0x007302B3";
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
    unsigned r_src2 = (n & 0x01F00000) >> 20; //extracted rs1
    unsigned funct_7 = (n & 0xFE000000) >> 25; //extracted rs1

    //fields
    bitset<7> op(opcode);
    bitset<5> rd(r_dest);
    bitset<3> f3(funct_3);
    bitset<5> rs1(r_src1);
    bitset<5> rs2(r_src2);
    bitset<7> f7(funct_7);

    //outputs
    cout << op.to_string() << endl;
    cout << rd.to_string() << endl;
    cout << f3.to_string() << endl;
    cout << rs1.to_string() << endl;
    cout << rs2.to_string() << endl;
    cout << f7.to_string() << endl;

    //storing values in memory
    reg[rs1.to_ulong()] = 46;
    reg[rs2.to_ulong()] = 23;

    //sample orperation
    switch(f3.to_ulong()){
        case 0b000:
            if(f7.to_ulong() == 0b0000000){
                reg[rd.to_ulong()] = reg[rs1.to_ulong()] + reg[rs2.to_ulong()];
            }
            else if(f7.to_ulong() == 0b0100000){
                reg[rd.to_ulong()] = reg[rs1.to_ulong()] - reg[rs2.to_ulong()];
            }

            cout << reg[rd.to_ulong()] << endl;
        break;

        default:
            cout << "error";
        break;
    }



}