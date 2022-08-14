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
	const char pri[N_OPTR][N_OPTR] = { //��������ȵȼ� [ջ��] [��ǰ]
	   /*              |-------------------- �� ǰ �� �� �� --------------------| */
	   /*              +      -      *      /      ^      !      (      )      \0 */
	   /* --  + */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
	   /* |   - */    '>',   '>',   '<',   '<',   '<',   '<',   '<',   '>',   '>',
	   /* ջ  * */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
	   /* ��  / */    '>',   '>',   '>',   '>',   '<',   '<',   '<',   '>',   '>',
	   /* ��  ^ */    '>',   '>',   '>',   '>',   '>',   '<',   '<',   '>',   '>',
	   /* ��  ! */    '>',   '>',   '>',   '>',   '>',   '>',   ' ',   '>',   '>',
	   /* ��  ( */    '<',   '<',   '<',   '<',   '<',   '<',   '<',   '=',   ' ',
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

	inline void append(char* rpn, double opnd) { //������������RPNĩβ
		char buf[64];
		if ((int)opnd < opnd) sprintf(buf, "%6.2f \0", opnd); //�����ʽ����
		else                       sprintf(buf, "%d \0", (int)opnd); //������ʽ
		strcat(rpn, buf); //RPN�ӳ�
		
	}
	inline void append(char* rpn, char optr) { //�����������RPNĩβ
		   int n = strlength(rpn); //RPN��ǰ���ȣ���'\0'��β������n + 1��
		   sprintf(rpn + n, "%c \0", optr); //����ָ���������
	}
	inline int facInt(int n) { //�׳�
		int res = 1;
		for (int i = 1; i <= n; i++)
			res *= i;
		return res;
	}
	
	inline float calcu(char op, float b) { //ִ��һԪ����
		switch (op) {
			case '!': return (float)facInt((int)b); //Ŀǰ���н׳ˣ����մ˷�ʽ���
			default: return 0;
		}
	}
	inline float calcu(float a, char op, float b) {//ִ��2Ԫ����
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
	inline double evaluate(char* S, char* RPN) { //�ԣ����޳��׿ո�ģ����ʽS��ֵ����ת��Ϊ�沨��ʽRPN
		Stack<float> opnd; Stack<char> optr; //������ջ�������ջ
		optr.push('\0'); //β�ڱ�'\0'Ҳ��Ϊͷ�ڱ�������ջ
		while (!optr.empty()) { //�������ջ�ǿ�֮ǰ�����������ʽ�и��ַ�
			if (isdigit(*S)) { //����ǰ�ַ�Ϊ����������
				readNumber(S, opnd); append(RPN, opnd.top()); //��������������������RPNĩ
			}
			else //����ǰ�ַ�Ϊ���������
				switch (orderBetween(optr.top(), *S)) { //������ջ�������֮�����ȼ��ߵͷֱ���
				case '<': //ջ����������ȼ�����ʱ
				optr.push(*S); S++; //�����Ƴ٣���ǰ�������ջ
				break;
				case '=': //���ȼ���ȣ���ǰ�����Ϊ�����Ż���β���ڱ�'\0'��ʱ
				optr.pop(); S++; //�����Ų�������һ���ַ�
				break;
				case '>': { //ջ����������ȼ�����ʱ����ʵʩ��Ӧ�ļ��㣬�������������ջ
				char op = optr.pop(); append(RPN, op); //ջ���������ջ��������RPNĩβ
				if ('!' == op) //������һԪ�����
				opnd.push(calcu(op, opnd.pop())); //��ȡһ������������������ջ
				else { //������������Ԫ�������
						float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop(); //ȡ����ǰ������
						opnd.push(calcu(pOpnd1, op, pOpnd2)); //ʵʩ��Ԫ���㣬�����ջ
					}
					break;
				}
			}//switch

		}//while
		return opnd.pop(); //�������������ļ�����
	}
}