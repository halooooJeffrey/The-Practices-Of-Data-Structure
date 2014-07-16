#include <iostream>
#include <stack>
#include <algorithm>
using namespace std;

#define N_OPTR 9
typedef enum {ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE} Operator;

const char pri[N_OPTR][N_OPTR] = {
	'>', '>', '<', '<', '<', '<', '<', '>', '>',
	'>', '>', '<', '<', '<', '<', '<', '>', '>',
	'>', '>', '>', '>', '<', '<', '<', '>', '>',
	'>', '>', '>', '>', '<', '<', '<', '>', '>',
	'>', '>', '>', '>', '>', '<', '<', '>', '>',
	'>', '>', '>', '>', '<', '>', ' ', '>', '>',
	'<', '<', '<', '<', '<', '<', '<', '=', ' ',
	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
	'<', '<', '<', '<', '<', '<', '<', ' ', '=',
};

void readNumber(char*& p, stack<float>& stk) {
	stk.push((float)(*p - '0'));
	while (isdigit(*(++p))) {
		float f = stk.top();	//无法直接使用pop()，STL中的stack的pop不返回值，可以使用自定义的stack
		stk.pop();
		stk.push(f * 10 + (*p - '0'));
	}
	if ('.' != *p)
		return;
	float fraction = 1;
	while (isdigit(*(++p))) {
		float f = stk.top();
		stk.pop();
		stk.push(f + (*p - '0') * (fraction /= 10));
	}
}

Operator optr2rank(char op) {
	switch (op) {
        case '+': return ADD;
        case '-': return SUB;
        case '*': return MUL;
        case '/': return DIV;
        case '^': return POW;
        case '!': return FAC;
        case '(': return L_P;
        case ')': return R_P;
        case '\0': return EOE;
        default: exit(-1);
	}
}

char orderBetween(char op1, char op2) {
	return pri[optr2rank(op1)][optr2rank(op2)];
}

void append(char* &rpn, float opnd) {
	size_t n = strlen(rpn);
	char buf[64];
	if (opnd != (float)(int)opnd) //判断是不是浮点数，好技巧
		sprintf(buf, "%.2f", opnd);
	else
		sprintf(buf, "%d", (int)opnd);
	//char* newRPN = new char[n + strlen(buf) + 1];
	//memcpy(newRPN, rpn, n + strlen(buf) + 1);
	//为什么这里delete会出错？因为一开始rpn不是new出来的，所以delete只能用在分配过内存的指针。
    //delete rpn;
	//rpn = newRPN;
	//使用realloc先要有malloc
	rpn = (char*) realloc(rpn, sizeof(char) * (n + strlen(buf) + 1));
	strcat(rpn, buf);
}

void append(char*& rpn, char optr) {
	size_t n = strlen(rpn);
	//char* newRPN = new char[n + 2];
	//memcpy(newRPN, rpn, n + 2);
	rpn = (char*) realloc(rpn, sizeof(char) * (n + 2));
	sprintf(rpn+ n, "%c", optr);
	rpn[n + 2] = '\0';
}

float calcu(char op, float opnd) {
	if (opnd == 0)
		return 1;
	float temp = opnd;
	while (--opnd > 0)
		temp = temp * opnd;
	return temp;
}

float power(float f1, float f2) {
	float temp = f1;
	while (--f2 > 0)
		temp *= f1;
	return temp;
}

float calcu(float opnd1, char op, float opnd2) {
	switch (op) {
        case '+' : return opnd2 + opnd1; break;
        case '-' : return opnd1 - opnd2; break;
        case '*' : return opnd1 * opnd2; break;
        case '/' : return opnd1 / opnd2; break;
        case '^' : return power(opnd1, opnd2); break;
        default : exit(-1);
	}
}

float evaluate(char* S, char*& RPN) {
	stack<float> opnd;
	stack<char> optr;
	optr.push('\0');
	while (!optr.empty()) {
		if (isdigit(*S)) {
			readNumber(S, opnd);
			append(RPN, opnd.top());
		}
		else {
			switch (orderBetween(optr.top(), *S)) {
                case '<':
                    optr.push(*S);
                    S++;
                    break;
                case '=':
                    optr.pop();
                    S++;
                    break;
                case '>': {
                    char op = optr.top();
                    optr.pop();
                    append(RPN, op);
                    if ('!' == op) {
                        float pOpnd = opnd.top();
                        opnd.pop();
                        opnd.push(calcu(op, pOpnd));
                    }
                    else {
                        float pOpnd2 = opnd.top();
                        opnd.pop();
                        float pOpnd1 = opnd.top();
                        opnd.pop();
                        opnd.push(calcu(pOpnd1, op, pOpnd2));
                    }
                    break;
                }
                default: exit(-1); 
			}
		}
	}
	return opnd.top();
}

int main() {
	char* S = "(0!+1)*2^(3!+4)-(5!-67-(8+9))";
	char* RPN = new char[0];
	cout << evaluate(S, RPN) << endl;
	cout << RPN << endl;
	return 0;
}