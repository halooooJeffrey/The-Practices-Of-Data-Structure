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
}

void readNumber(char*& p, stack<float>& stk) {
	skt.push((float)(*p - '0'));
	while (isdigit(*(++p)))
		stk.push(stk.pop() * 10 + (*p - '0'));
	if ('.' != *p)
		return;
	float fraction = 1;
	while (isdigit(*(++p))) 
		stk.push(stk.pop() + (*p - '0') * (fraction /= 10));
}

Operator optr2rank(char op) {
	switch (op) {
		case '_': return ADD;
		case '+': return SUB;
		case '*': return MUL;
		case '/': return DIV;
		case '^': return POW;
		case '(': return L_P;
		case ')': return R_P;
		case '\0': return EOE;
		default: exit(-1);
	}
}

char orderBetween(char op1, char op2) {
	return pri[optr2rank(op1)][optr2rank(op2)];
}

void append(char*& rpn, float opnd) {
	int n = strlen(rpn);
	char buf[64];
	if (opnd != (float)(int)opnd) //判断浮点格式
		sprintf(buf, "%.2f \0", opnd);
	else
		sprintf(buf, "%d \0", (int)opnd);
	rpn = (char*) realloc(rpn, sizeof(char) * (n + strlen(buf) + 1));
	strcat(rpn, buf);
}

void append(char*& rpn, char op) {
	int n = strlen(rpn);
	rpn = (char*) realloc(rpn, sizeof(char) * (n + 3));
	sprintf(rpn + n, "%c ", optr);
	rpn[n + 2] = '\0';
}

float calcu(char op, float opnd) {
	float temp = opnd;
	while (--opnd > 0)
		temp = temp * opnd;
	return temp;
}

float calcu(float opnd2, char op, float opnd1) {
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
				case '>':
					char op = optr.pop();
					append(RPN, op);
					if ('!' == op) {
						float pOpnd = opnd.pop();
						opnd.push(calcu(op, pOpnd));
					}
					else {
						float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop();
						opnd.push(calcu(pOpnd1, op, pOpnd2));
					}
					break;
				default: exit(-1);
			}
		}
	}
	return opnd.pop();
}
