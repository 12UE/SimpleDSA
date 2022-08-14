#pragma once
namespace dsa {
	inline void convert(Stack<char>& s, __int64 n, int base) {
		static char digit[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
		while (n > 0) {
			s.push(digit[n % base]);
			n /= base;
		}
	}
	
	inline double Abs(double value) {
		return value < 0 ? -value : value;
	}
	inline bool IsEqual(float num1, float num2) {
		return (Abs(num2 - num1) < 1e-3);
	}
	
	__forceinline float q_rsqrt(float number) {
		long i;
		float x2, y;
		const float threehalfs = 1.5F;
		x2 = number * 0.5F;
		y = number;
		i = *(long*)&y;
		i = 0x5f3759df - (i >> 1);
		y = *(float*)&i;
		y = y * (threehalfs - (x2 * y * y));   // 1st iteration
		y = y * (threehalfs - (x2 * y * y));   // 2rd iteration this can be remove
		return number * y;
	}
	inline float Pow(float base,float expoment){
		float result=base;
		while (expoment > 0) {
			result *= base;
			expoment--;
		}
		return result;
	}

constexpr int N_OPTR = 9;
	typedef enum {
		ADD,
		SUB,
		MUL,
		DIV,
		POW,
		FAC,
		L_P,
		R_P,
		EOE,
	}Operator;
	const char pri[N_OPTR][N_OPTR] = { //运算符优先等级 [栈顶] [当前]
	   /*              |-------------------- 当 前 运 算 符 --------------------| */
	   /*              +      -      *      /      ^      !      (      )      \0 */
	   /* --  + */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
	   /* |   - */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
	   /* 栈  * */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
	   /* 顶  / */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
	   /* 运  ^ */    '>',   '>',   '>',   '>',   '>',   '<',   '<',   '>',   '>',
	   /* 算  ! */    '>',   '>',   '>',   '>',   '>',   '>',   ' ',   '>',   '>',
	   /* 符  ( */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   '=',   ' ',
	   /* |   ) */    ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',   ' ',
	   /* -- \0 */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   ' ',   '='
	};
	inline Operator optr2rank(char op) {
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
			default: return EOE;
		}
	}
	inline char orderBetween(char op1, char op2) {
		return pri[optr2rank(op1)][optr2rank(op2)];
	}
	inline void readNumber(char*& p, Stack<float>& stk) {
		stk.push((float)(*p - '0'));
		while (isdigit(*(++p)))
			stk.push(stk.pop() * 10 + (*p - '0'));
		if ('.' != *p)return;
		float fraction =1.0f;
		while (isdigit(*(++p))) {
			fraction /= 10.0f;
			stk.push(stk.pop() + (*p - '0') * fraction);
		}
	}

	inline void append(char* rpn, double opnd) { //将操作数接至RPN末尾
		char buf[64];
		if ((int)opnd < opnd) sprintf(buf, "%6.2f \0", opnd); //浮点格式，或
		else                       sprintf(buf, "%d \0", (int)opnd); //整数格式
		strcat(rpn, buf); //RPN加长
		
	}
	inline void append(char* rpn, char optr) { //将运算符接至RPN末尾
		   int n = strlength(rpn); //RPN当前长度（以'\0'结尾，长度n + 1）
		   sprintf(rpn + n, "%c \0", optr); //接入指定的运算符
	}
	inline int facInt(int n) { //阶乘
		int res = 1;
		for (int i = 1; i <= n; i++)
			res *= i;
		return res;
	}
	
	inline float calcu(char op, float b) { //执行一元运算
		switch (op) {
			case '!': return (float)facInt((int)b); //目前仅有阶乘，可照此方式添加
			default: return 0;
		}
	}
	inline float calcu(float a, char op, float b) {//执行2元运算
		switch (op) {
			case '+': return a + b;
			case '-': return a - b;
			case '*': return a * b;
			case '/': return a / b;
			case '^': return pow(a, b);
			default:
				return 0;
		}
		
	}
	inline double evaluate(char* S, char* RPN) { //对（已剔除白空格的）表达式S求值，并转换为逆波兰式RPN
		Stack<float> opnd; Stack<char> optr; //运算数栈、运算符栈
		optr.push('\0'); //尾哨兵'\0'也作为头哨兵首先入栈
		while (!optr.empty()) { //在运算符栈非空之前，逐个处理表达式中各字符
			if (isdigit(*S)) { //若当前字符为操作数，则
				readNumber(S, opnd); append(RPN, opnd.top()); //读入操作数，并将其接至RPN末
			}
			else //若当前字符为运算符，则
				switch (orderBetween(optr.top(), *S)) { //视其与栈顶运算符之间优先级高低分别处理
				case '<': //栈顶运算符优先级更低时
				optr.push(*S); S++; //计算推迟，当前运算符进栈
				break;
				case '=': //优先级相等（当前运算符为右括号或者尾部哨兵'\0'）时
				optr.pop(); S++; //脱括号并接收下一个字符
				break;
				case '>': { //栈顶运算符优先级更高时，可实施相应的计算，并将结果重新入栈
				char op = optr.pop(); append(RPN, op); //栈顶运算符出栈并续接至RPN末尾
				if ('!' == op) //若属于一元运算符
				opnd.push(calcu(op, opnd.pop())); //则取一个操作数，计算结果入栈
				else { //对于其它（二元）运算符
						float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop(); //取出后、前操作数
						opnd.push(calcu(pOpnd1, op, pOpnd2)); //实施二元计算，结果入栈
					}
					break;
				}
			}//switch

		}//while
		return opnd.pop(); //弹出并返回最后的计算结果
	}
}