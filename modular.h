#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<map>
#include<iomanip>
#include<stdio.h>
#include<windows.h>
#include <cstdlib>
#include <sstream>
#include <cstring> 
using namespace std;


map<string, int> opmap = {{"L.D",0},{"ADD.D",1},{"SUB.D" ,2},{"MUL.D",3},{"DIV.D",4} };

int getw()
{

	HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(outputHandle,&info);
	return info.srWindow.Right + 1;
}
union value
{
	double fv;
	long long lv;
};

class cachedatabus
{
public:
	vector<bool> res;
	vector<value> v;
	vector<int> pc;
};

class LoadStation
{
public:
	std::string q;
	int busy;
	int addr;
	value value;
};

class ReservationStation
{
public:
	int pc=0;
	int busy = 0;
	int q=0;
	std::string op;
	value value[2] = { 0,0 };
	int rname[2] = { -1,-1 };
	int lasttime=0;
	void printme(string name,string qj,string qk)
	{
		int resw = 6;
		if (busy)
		{
			cout << setw(resw) << name << setw(resw) << "yes" << setw(resw) << lasttime << setw(resw) << op;
			if (qj == "0")
				cout << setw(resw) << value[0].fv;
			else
				cout << setw(resw) << "";
			if (qk == "0")
				cout << setw(resw) << value[1].fv;
			else
				cout << setw(resw) << "";
			if (qj != "0")
				cout << setw(resw) << qj;
			else
				cout << setw(resw) << "";
			if (qk != "0")
				cout << setw(resw) << qk;
			else
				cout << setw(resw) << "";
			cout << endl;
		}
		else
		{
			cout << setw(resw) << name;
			cout<< endl;
		}
	}
	void printld(string name)
	{
		int resw = 6;
		if (busy)
		{
			cout << setw(resw) << name << setw(resw) << "yes" << setw(resw) << lasttime << setw(resw) << op;
			cout << setw(resw) << value[0].lv;
			cout << setw(resw) << "";
			cout << setw(resw) << "";
			cout << setw(resw) << "";
			cout << setw(resw) << value[1].lv;
			cout << endl;
		}
		else
		{
			cout << setw(resw) << name;
			cout << endl;
		}

	}
	void clear()
	{
		pc = 0;
		busy = 0;
		q = 0;
		op="";
		value[0].lv = 0;
		value[1].lv = 0;
		rname[0] = -1;
		rname[1] = -1;
		lasttime = 0;

	}
};

class RegisterStation
{
public:
	int q=-1;
	int state;
	value value;
};

class Instruction
{
public:
	int num;
	int state = 0;
	int t[4] = { 0,0,0,0 };
	std::string me;
	std::string name;
	std::string opr1;
	std::string opr2;
	std::string opr3;
	string ts(int i)
	{
		if (!i)return "";
		else return to_string(i);
	}
	void printme()
	{
		cout << setw(6) << name << setw(12) << me << setw(6) << ts(t[0]) << setw(3) << "" <<  setw(3) << ts(t[1]) << "  - " << setw(2) << ts(t[2]) << setw(6) << ts(t[3]) << setw(3) << "" << endl;
	}
};

class InstructionStation
{
public:
	//std::string q;
	int state;
	Instruction ins;
	int runtime;
};

class simulator	
{
public:
	vector<Instruction> instruction;
	vector<InstructionStation> IS;
	vector<ReservationStation> RS;
	vector<LoadStation> LS;
	vector<RegisterStation> RegS;
	cachedatabus cdb;
	value mem[1000] ;

	int pc = 0;
	int t = 0;
	int done = 0;

	int time[5] = { 3,3,3,11,41 };
	int mnum[5] = { 2,3,2,16,16 };


	int w = getw();
	int rw = 6;
	int resw = 6;
	int rssize= mnum[0] + mnum[1] + mnum[2];
	int resize = mnum[3] + mnum[4];

	simulator()
	{
		RS.resize(rssize, ReservationStation());
		RegS.resize(resize, RegisterStation());
		cdb.res.resize(rssize,false);
		cdb.v.resize(rssize);
		cdb.pc.resize(rssize, 0);
	}

	string resname(int i)
	{
		string restype[] = {"LD","ADD","MUL"};
		i++;
		int t = 0;
		while (i > mnum[t])
		{
			i -= mnum[t];
			t++;
		}

		string a = restype[t] + to_string(i);
		return a;
	}

	int getregad(string a)
	{
		int regad = strtol(a.substr(1).c_str(), NULL, 10);
		if (a[0] == 'F')regad = regad + mnum[3];
		return regad;

	}

	void printdivline()
	{
		cout << endl;
		for (int i = 0; i < w; i++)
		{
			cout << "-";
		}
		cout << endl;
	}

	void printreg()
	{
		int j = 0;
		int rpl = w / rw;
		cout << "Registers:" << endl << endl;
		for (int i = 0; i < mnum[3]; i++)
		{
			string rn = "R" + to_string(i);
			cout << setw(rw) << rn;
			if (((i+1) % rpl == 0 && i!=0)||i==mnum[3]-1)
			{
				cout << endl;
				for (j ; j <= i; j++)
				{
					if(RegS[j].q==-1)
					cout << setw(rw) << RegS[j].value.lv;
					else
					{
						cout << setw(rw) << resname(RegS[j].q);
					}
				}
				j = i+1;
				cout << endl<<endl;
			}
		}
		j = 0;
		for (int i = 0; i < mnum[4]; i++)
		{
			string rn = "F" + to_string(i);
			cout << setw(rw) << rn;
			if (((i + 1) % rpl == 0 && i != 0)||i==mnum[4]-1)
			{
				cout << endl;
				for (j ; j <= i; j++)
				{
					if (RegS[j+mnum[3]].q==-1)
						cout << setw(rw) << RegS[j + mnum[3] ].value.fv;
					else
					{
						cout << setw(rw) << resname(RegS[j + mnum[3] ].q);
					}
				}
				j = i + 1;
				cout << endl << endl;
			}
		}
	}
	
	void printres()
	{
		cout << "ReservationStation:" << endl << endl;
		cout << setw(resw) << "name" << setw(resw) << "Busy" << setw(resw) << "t" << setw(resw) << "OP" << setw(resw) << "Vj" << setw(resw) << "Vk" << setw(resw) << "Qj" << setw(resw) << "Qk" << setw(resw) << "A" << endl;
		for (int i = 0; i < rssize; i++)
		{
			if (RS[i].op != "L.D")
			{
				string qj = "0", qk = "0";
				if (RS[i].rname[0] != -1)
				{
					qj = resname(RS[i].rname[0]);
				}
				if (RS[i].rname[1] != -1)
				{
					qk = resname(RS[i].rname[1]);
				}
				RS[i].printme(resname(i),qj,qk);
			}
			else
			{
				RS[i].printld(resname(i));

			}
		}
	}
	
	void printtime()
	{
		cout << "timetable:" << endl << endl;
		cout << setw(18) << "instruction" << setw(8) << "issue" << setw(10) << "execute" << setw(8) << "write" << endl;
		for (InstructionStation i : IS)
		{
			i.ins.printme();
		}
	}






	void simclear()
	{
		IS.clear();
		rssize = mnum[0] + mnum[1] + mnum[2];
		RS.resize(rssize, ReservationStation());
		resize = mnum[3] + mnum[4];
		RegS.resize(resize, RegisterStation());
		cdb.res.resize(rssize, false);
		cdb.v.resize(rssize);
		cdb.pc.resize(rssize, 0);
		memset(mem, 0, sizeof(mem));
		pc = 0;
		t = 0;
		done = 0;
	}

	void insload(ifstream& ops)
	{
		if (!ops.is_open())
		{
			cout << "can not open this file" << endl;
			return;
		}
		string con;
		char* next;
		string me;
		for(;;)
		{
			ops >> con;
			if (ops.eof())break;
			InstructionStation* inst = new InstructionStation;
			inst->ins.num = strtol(con.c_str(),NULL,10);

			ops >> con;
			inst->ins.name = con;

			ops >> con;
			inst->ins.opr1 = strtok_s((char *)con.c_str(),",",&next);
			inst->ins.me += inst->ins.opr1;

			if (inst->ins.name == "L.D")
			{
				inst->ins.opr2 = strtok_s(NULL, "(", &next);
				inst->ins.opr3 = strtok_s(NULL, ")", &next);
				inst->ins.me = inst->ins.me + "," + inst->ins.opr2 + "(" + inst->ins.opr3 + ")";
			}
			else
			{
				inst->ins.opr2 = strtok_s(NULL, ",", &next);
				inst->ins.opr3 = strtok_s(NULL, "\n", &next);
				inst->ins.me = inst->ins.me + "," + inst->ins.opr2 + "," + inst->ins.opr3 + ",";
			}
			inst->runtime = time[opmap[inst->ins.name]];
			inst->state = 0;
			IS.push_back(*inst);
		}
		ops.close();
	}

	void printsim()
	{
		cout << "clock:" << t << endl;
		printdivline();
		printreg();
		printdivline();
		printres();
		printdivline();
		printtime();
		printdivline();
	}

	int issue()
	{
		if (pc >= IS.size())
		{
			return -1;
		}
		if (IS[pc].ins.name == "L.D")
		{
			int i;
			for ( i = 0; i < mnum[0]; i++)
			{
				if (!RS[i].busy)break;
			}
			if (i == mnum[0])return -1;
			RS[i].op = IS[pc].ins.name;
			RS[i].lasttime = IS[pc].runtime;
			int targ = getregad(IS[pc].ins.opr1);
			int opf = getregad(IS[pc].ins.opr3);
			long long imm = strtol(IS[pc].ins.opr2.c_str(), NULL, 10);

			if (RegS[opf].q != -1)
			{
				RS[i].rname[0] = RegS[opf].q;
			}
			else
			{
				RS[i].value[0] = RegS[opf].value;
			}

			RegS[targ].q = i;
			RS[i].pc = pc; 
			RS[i].value[1].lv = imm;
			IS[pc].ins.t[0] = t;
			IS[pc].ins.state = 1;
			pc++;
			return i;

		}
		if (IS[pc].ins.name == "SUB.D"|| IS[pc].ins.name == "ADD.D")
		{
			int i;
			for (i = mnum[0]; i < mnum[0]+mnum[1]; i++)
			{
				if (!RS[i].busy)break;
			}
			if (i == mnum[0] + mnum[1])return -1;

			int targ = getregad(IS[pc].ins.opr1);
			int op1 = getregad(IS[pc].ins.opr2);
			int op2 = getregad(IS[pc].ins.opr3);

			if (RegS[op1].q != -1)
			{
				RS[i].rname[0] = RegS[op1].q;
			}
			else
			{
				RS[i].value[0] = RegS[op1].value;
			}

			if (RegS[op2].q != -1)
			{
				RS[i].rname[1] = RegS[op2].q;
			}
			else
			{
				RS[i].value[1] = RegS[op2].value;
			}


			RegS[targ].q = i;
			RS[i].pc = pc;
			RS[i].busy = 1;
			IS[pc].ins.t[0] = t;
			IS[pc].ins.state = 1;
			RS[i].op = IS[pc].ins.name;
			RS[i].lasttime = IS[pc].runtime;
			pc++;
			return i;
		}
		if (IS[pc].ins.name == "MUL.D"||IS[pc].ins.name == "DIV.D")
		{
			int i;
			for (i = mnum[0] + mnum[1]; i < mnum[0] + mnum[1] + mnum[2]; i++)
			{
				if (!RS[i].busy)break;
			}
			if (i == mnum[0] + mnum[1] + mnum[2])return -1;

			int targ = getregad(IS[pc].ins.opr1);
			int op1 = getregad(IS[pc].ins.opr2);
			int op2 = getregad(IS[pc].ins.opr3);

			if (RegS[op1].q != -1)
			{
				RS[i].rname[0] = RegS[op1].q;
			}
			else
			{
				RS[i].value[0] = RegS[op1].value;
			}

			if (RegS[op2].q != -1)
			{
				RS[i].rname[1] = RegS[op2].q;
			}
			else
			{
				RS[i].value[1] = RegS[op2].value;
			}


			RegS[targ].q = i;
			RS[i].pc = pc;
			IS[pc].ins.t[0] = t;
			IS[pc].ins.state = 1;
			RS[i].op = IS[pc].ins.name;
			RS[i].lasttime = IS[pc].runtime;
			pc++;
			return i;

		}
	}

	vector<int> exe()
	{
		vector<int> ret;
		for (int i = 0; i < rssize; i++)
		{
			if (i < mnum[0])
			{
				if (RS[i].rname[0] == -1 && RS[i].busy && RS[i].lasttime > 0)
				{
					RS[i].lasttime--;
					if (RS[i].lasttime == IS[RS[i].pc].runtime - 1)
					{
						IS[RS[i].pc].ins.t[1] = t;
					}
					else if (!RS[i].lasttime)
					{
						ret.push_back(i);
						RS[i].value[1].lv += RS[i].value[0].lv;
						cdb.pc[i] = RS[i].pc;
						cdb.v[i] = mem[RS[i].value[1].lv];
						IS[RS[i].pc].state = 2;
						IS[RS[i].pc].ins.t[2] = t;
					}
				}
			}
			else
			{
				if (RS[i].rname[0] == -1 && RS[i].rname[1] == -1 && RS[i].busy && RS[i].lasttime > 0)
				{
					RS[i].lasttime--;
					if (RS[i].lasttime == IS[RS[i].pc].runtime - 1)
					{
						IS[RS[i].pc].ins.t[1] = t;
					}
					if (!RS[i].lasttime)
					{
						ret.push_back(i);
						cdb.pc[i] = RS[i].pc;
						IS[RS[i].pc].ins.t[2] = t;
						IS[RS[i].pc].state = 2;
						if (RS[i].op == "ADD.D")
						{
							cdb.v[i].fv = RS[i].value[0].fv + RS[i].value[1].fv;
						}
						if (RS[i].op == "SUB.D")
						{
							cdb.v[i].fv = RS[i].value[0].fv - RS[i].value[1].fv;
						}
						if (RS[i].op == "MUL.D")
						{
							cdb.v[i].fv = RS[i].value[0].fv * RS[i].value[1].fv;
						}
						if (RS[i].op == "DIV.D")
						{
							cdb.v[i].fv = RS[i].value[0].fv / RS[i].value[1].fv;
						}
					}
				}
			}
		}
		return ret;
	}

	void result()
	{
		for (int i = 0; i < rssize; i++)
		{
			if (cdb.res[i])
			{
				cdb.res[i] = false;
				IS[cdb.pc[i]].state = 3;
				IS[cdb.pc[i]].ins.t[3] = t;
				for (int j = 0; j < mnum[3] + mnum[4]; j++)
				{
					if (RegS[j].q == i)
					{
						RegS[j].q = -1;
						RegS[j].value = cdb.v[i];
					}
				}

				for (int j = 0; j < rssize; j++)
				{
					if (RS[j].rname[0] == i)
					{
						RS[j].rname[0] = -1;
						RS[j].value[0] = cdb.v[i];
					}
					if (RS[j].rname[1] == i)
					{
						RS[j].rname[1] = -1;
						RS[j].value[1] = cdb.v[i];
					}
				}
				RS[i].clear();
			}
		}
	}

	bool step()
	{
		if (done)return false;
		t++;
		int issuenum;
		vector<int> exenum;
		issuenum=issue();
		exenum=exe();
		result();
		if (issuenum != -1)
		{
			RS[issuenum].busy = 1;
		}
		for (int i : exenum)
		{
			cdb.res[i] = true;
		}
		for (InstructionStation inst : IS)
		{
			if (inst.state != 3)return true;
		}

		for (InstructionStation inst : IS)
		{
		}
		return false;
	}
	
	void run()
	{
		while (step())
		{
			printsim();
		}
		printsim();
		return;
	}

};