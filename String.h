#pragma once

#ifdef MSVC_ENVIROMENT
#include<iostream>
#endif // MSVC_ENVIROMENT
namespace dsa {
	class string {
	private:
		char * m_str;
		int m_length;
		void releasebuffer();
		void strcopy(const char * src, char * dest);
		int  strlength(const char * str);
#ifdef MSVC_ENVIROMENT
		friend std::ostream& operator<<(std::ostream& os, const string& str) {
			os << str.m_str;
			return os;
		}
		friend std::istream & operator>>(std::istream & is, string & str){
			is >> str.m_str;
			return is;
		}
#endif // MSVC_ENVIROMENT
		
	public:
		string(){
			m_str = new char[1];
			*m_str = '\0';
		}
		string(const char* str);
		string(const string & str);
		char first();
		char last();
		class iterator {
		private:
			char* m_ptr;
		public:
			iterator(char* ptr) :m_ptr(ptr) {}
			char& operator*() { return *m_ptr; }
			iterator& operator++(int) {return iterator(m_ptr++);}
			iterator& operator++() {++m_ptr;return *this;}
			iterator& operator--(int) { return iterator(m_ptr--); }
			iterator& operator--() {--m_ptr;return *this;}
			bool operator==(const iterator& rhs) {return m_ptr == rhs.m_ptr;}
			bool operator!=(const iterator& rhs) {return m_ptr != rhs.m_ptr;}
		};
		iterator begin() const;
		iterator end() const;
		void erase(int pos=0, int len=0);
		void earse(int first, int last);
		string operator=(const string & str);
		string operator+(const string & str);
		string operator+(const char * str);
		string operator=(const char * str);
		bool operator==(const string & str);
		bool operator==(const char * str);
		int length() const;
		char & operator[](int index);
		char * c_str() const;
		char * data();
		int find(const char * str, int start = 0);
		int find(const string & str, int start = 0);
		string substr(int start, int length);
		int compare(const char * str);
		int compare(const string & str);
		void swap(string & str);
		bool empty();
		void pop_back();
		~string();
	};
	inline void string::releasebuffer(){
		if (m_str != nullptr) {
			delete[] m_str;
			m_str = nullptr;
		}
	}
	inline void string::strcopy(const char * src, char * dest){
		while (*src != '\0'){
			*dest = *src;
			src++;
			dest++;
		}
		*dest = '\0';
	}
	inline int  string::strlength(const char * str){
		int length = 0;
		while (*str != '\0'){
			length++;
			str++;
		}
		return length;
	}

	inline string::string(const char* str)
	{
		m_length = strlength(str);
		m_str = new char[m_length + 1];
		strcopy(str, m_str);
	}

	inline string::string(const string& str)
	{
		m_length = str.length();
		m_str = new char[m_length + 1];
		strcopy(str.c_str(), m_str);
	}

	inline char string::first()
	{
		if(m_length == 0){
			return '\0';
		}
		return m_str[0];
	}

	inline char string::last()
	{
		if (m_length == 0) {
			return '\0';
		}
		return m_str[m_length - 1];
	}

	inline string::iterator string::begin() const
	{
		return string::iterator(m_str);
	}

	inline string::iterator string::end() const
	{
		return string::iterator(m_str + m_length);
	}

	inline void string::erase(int pos, int len)
	{
		if (pos < 0 || pos >= m_length)
			return;
		if (len == 0)
			len = m_length - pos;
		if (len < 0 || pos + len > m_length)
			return;
		for (int i = pos; i < m_length - len; i++)
			m_str[i] = m_str[i + len];
		m_length -= len;
		m_str[m_length] = '\0';
	}

	inline void string::earse(int first, int last)
	{
		if (first < 0 || first >= m_length)
			return;
		if (last < 0 || last >= m_length)
			last = m_length - 1;
		if (first > last)
			return;
		for (int i = first; i < m_length - last; i++)
			m_str[i] = m_str[i + last - first + 1];
		m_length -= last - first + 1;
		m_str[m_length] = '\0';
	}

	inline string string::operator=(const string & str){
		if (this != &str){
			m_length = str.length();
			m_str = new char[m_length + 1];
			strcopy(str.c_str(), m_str);
		}
		return *this;
	}
	inline string string::operator+(const string & str){
		string temp;
		temp.m_length = m_length + str.length();
		temp.m_str = new char[temp.m_length + 1];
		strcopy(m_str, temp.m_str);
		strcopy(str.c_str(), temp.m_str + m_length);
		return temp;
	}
	inline string string::operator+(const char * str){
		string temp;
		temp.m_length = m_length + strlength(str);
		temp.m_str = new char[temp.m_length + 1];
		strcopy(m_str, temp.m_str);
		strcopy(str, temp.m_str + m_length);
		return temp;
	}
	inline string string::operator=(const char * str){
		releasebuffer();
		m_length = strlength(str);
		m_str = new char[m_length + 1];
		strcopy(str, m_str);
		return *this;
	}
	inline bool string::operator==(const string& str){
		if (m_length != str.length())
			return false;
		for (int i = 0; i < m_length; i++)
			if (m_str[i] != str.m_str[i])
				return false;
		return true;
	}
	inline bool string::operator==(const char* str){
		string temp(str);
		return operator==(temp);
	}
	inline int string::length() const{
		return m_length;
	}
	inline char & string::operator[](int index){
		if (index >= 0 || index <= m_length){
			return m_str[index];
		}else{
			throw "Index out of range";
		}
	}
	inline char * string::c_str() const{
		return m_str;
	}
	inline char * string::data(){
		return m_str;
	}
	inline int string::find(const char* str, int start)
	{
		string temp(str);
		return find(temp, start);
	}
	inline int string::find(const string& str, int start)
	{
		int index = -1;
		for (int i = start; i < m_length; i++)
		{
			if (m_str[i] == str.m_str[0])
			{
				bool flag = true;
				for (int j = 1; j < str.m_length; j++)
				{
					if (m_str[i + j] != str.m_str[j])
					{
						flag = false;
						break;
					}
				}
				if (flag)
				{
					index = i;
					break;
				}
			}
		}
		return index;
	}
	inline string string::substr(int start, int length)
	{
		string temp;
		temp.m_length = length;
		temp.m_str = new char[temp.m_length + 1];
		for (int i = 0; i < length; i++)
		{
			temp.m_str[i] = m_str[start + i];
		}
		temp.m_str[temp.m_length] = '\0';
		return temp;
	}
	inline int string::compare(const char* str)
	{
		string temp(str);
		return compare(temp);
	}
	inline int string::compare(const string& str)
	{
		if (m_length > str.m_length)
			return 1;
		else if (m_length < str.m_length)
			return -1;
		else
		{
			for (int i = 0; i < m_length; i++)
			{
				if (m_str[i] > str.m_str[i])
					return 1;
				else if (m_str[i] < str.m_str[i])
					return -1;
			}
			return 0;
		}
	}
	inline void string::swap(string& str)
	{
		char* temp = m_str;
		m_str = str.m_str;
		str.m_str = temp;
		int temp1 = m_length;
		m_length = str.m_length;
		str.m_length = temp1;
	}
	inline bool string::empty()
	{
		return m_length == 0;
	}
	inline void string::pop_back(){
		if (m_length > 0)
		{
			m_length--;
			m_str[m_length] = '\0';
		}
	}
	inline string::~string(){
		releasebuffer();
	}
}