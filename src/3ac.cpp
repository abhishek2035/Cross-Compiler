#include "3ac.h"
#include "typeCheck.h"
using namespace std;
using std::setw;

long long Index = -1;

vector <quad> emittedCode;
string getTmpVar(){
  static long a = 0;
  a++;
  string tmp =  string("__t");
  tmp = tmp + to_string(a);
  tmp = tmp + string("__");
  return tmp;
}

pair<string, sEntry*> getTmpSym(string type){
  string tmp = getTmpVar();
  char *cstr = new char[type.length() + 1];
  strcpy(cstr, type.c_str());
  insertSymbol(*curr, tmp, type, getSize(cstr),0, 1);
  return pair <string, sEntry* >(tmp, lookup(tmp));
}

int emit (qid op, qid id1, qid id2, qid  res, int stmtNum){
  quad t;
  t.id1 = id1;
  t.id2 = id2;
  t.res = res;
  t.op = op;
  t.stmtNum = stmtNum;
  emittedCode.push_back(t);
  Index++;
  return emittedCode.size()-1;
}

int getNextIndex(){
  return emittedCode.size();
}

void backPatch(list<int> li, int p){
  for(int i=0; i<li.size(); ++i){
    unsigned N = i;
    if (li.size() > N)
    {
      std::list<int>::iterator it = li.begin();
      std::advance(it, N);
      emittedCode[*it].stmtNum = p;
  }
}
return;
}

void setResult(int a, qid p){
  emittedCode[a].res = p;
  return;
}

void setId1(int a, qid p){
	emittedCode[a].id1 = p;
	return;
}

void assignmentExpression(char *o, string type, string type1, string type3, qid place1, qid place3){
	qid t = place3;
        qid t2;
	string op;
	string op1;
        int a=0;int b=0;
        if(!strcmp(o,"=")){
              a=1;
        }
	if(!strcmp(o,"*=")){
		op = "*";
                op1 = "*";
		t = getTmpSym(type);
	}
	else if(!strcmp(o,"/=")){
		op = "/";
                op1 = "/";
		t = getTmpSym(type);
	}
	else if(!strcmp(o,"+=")){
		op = "+";
                op1 = "+";
		t = getTmpSym(type);
	}
	else if(!strcmp(o,"-=")){
		op = "-";
                op1 = "-";
		t = getTmpSym(type);
	}
	if(isInt(type1) && isInt(type3)){
		op += "int";
	        if(strcmp(o,"=")) emit(pair<string, sEntry*>(op, lookup(op1)), place1, place3, t, -1);
	}
	else if(isFloat(type1) && isInt(type3)){
		t2 = getTmpSym(type);
		emit(pair<string, sEntry*>("inttoreal",NULL), place3,pair<string, sEntry*>("",NULL),t2,-1);
		op += "real";
		if(strcmp(o,"=")) emit(pair<string, sEntry*>(op, lookup(op1)), place1, t2, t, -1); 
                b=1;
	}
	else if(isFloat(type3) && isInt(type1)){
		t2 = getTmpSym(type);
		emit(pair<string, sEntry*>("realtoint",NULL),place3,pair<string, sEntry*>("",NULL),t2,-1);
		op += "int";
		if(strcmp(o,"=")) emit(pair<string, sEntry*>(op, lookup(op1)), place1, t2, t, -1);
                 b=1;
	}
	else if(isFloat(type3) && isFloat(type1)){
		op += "real";
		if(strcmp(o,"=")) emit(pair<string, sEntry*>(op, lookup(op1)), place1, place3, t, -1);
	}


	if(!(a &&b )) emit(pair<string, sEntry*>("=", lookup("=")),  t, pair<string, sEntry*>("", NULL), place1, -1);
	else emit(pair<string, sEntry*>("=", lookup("=")),  t2, pair<string, sEntry*>("", NULL), place1, -1);

return;

}
void assignment2(char *o, string type, string type1, string type3, qid place1, qid place3){
	qid t = getTmpSym(type);
	string op;
	string op1;
        if(!strcmp(o,"%=")) op = "%";
        else if(!strcmp(o,"^=")) op = "^";
        else if(!strcmp(o,"|=")) op = "|";
        else if(!strcmp(o,"&=")) op = "&";
        op1 = op;
        if(!strcmp(o,"<<=")){ op="LEFT_OP"; op1="<<"; }
        if(!strcmp(o,">>=")){ op="RIGHT_OP"; op1=">>"; }
        emit(pair<string, sEntry*>(op, lookup(op1)), place1, place3, t, -1);
	emit(pair<string, sEntry*>("=", lookup("=")),  t, pair<string, sEntry*>("", NULL), place1, -1);

}

void display3ac(){
	for(int i = 0; i<emittedCode.size(); ++i)  {
		display(emittedCode[i], i);
	}
	return;
}


void display(quad q, int i){
	if(q.stmtNum==-1){
		cout << setw(5) << "[" << i << "]" << ": " << setw(15) << q.op.first << " " <<
			setw(15) << q.id1.first << " " <<
			setw(15) << q.id2.first << " " <<
			setw(15) << q.res.first << '\n';
	}
        else if(q.stmtNum==-2){
		cout  << "[" << i << "]" << ": "<<
		 q.op.first << endl << endl;
	}

	else{
		cout << setw(5) << "[" << i << "]" << ": " << setw(15) << q.op.first << " " <<
			setw(15) << q.id1.first << " " <<
			setw(15) << q.id2.first << " " <<
			setw(15) << q.stmtNum << "---" << '\n';
	}
}
