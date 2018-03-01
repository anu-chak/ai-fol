#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
using namespace std;

int vartrack[26]={0};
int global_flag=0;

struct clause
{
	int neg; // 0 if not negated, 1 if negated
	string pred; 
	vector<int> vartype;
	vector<string> variables;
};

struct clause* build_query(string s)
{
	int len= s.length();
	int i=0, pos=0;
	string copy_str=s;

	struct clause *node= new struct clause;

	string delimiter= "~";
	pos=copy_str.find(delimiter);
	if(pos==std::string::npos)
		node->neg=0;
	else
	{
		node->neg=1;
		copy_str.erase(0, pos+delimiter.length());
	}

	delimiter= "(";
	pos=copy_str.find(delimiter);
	node->pred= copy_str.substr(0, pos);
	copy_str.erase(0, pos+delimiter.length());

	delimiter= ",";
	pos=copy_str.find(delimiter);
	while(pos!= std::string::npos)
	{
		string token = copy_str.substr(0, pos);
		node->variables.push_back(token);

		if(token.length()==1 && token[0]>='a' && token[0]<='z')
			node->vartype.push_back(0);
		else if(token.length()>1 && token[0]>='a' && token[0]<='z' && int(token[1])>=0 && int(token[1])<=9)
			node->vartype.push_back(0);
		else
			node->vartype.push_back(1);

		copy_str.erase(0, pos+delimiter.length());
		pos=copy_str.find(delimiter);
	}

	delimiter= ")";
	node->variables.push_back(copy_str.substr(0,copy_str.find(delimiter)));
	if(((copy_str.substr(0,copy_str.find(delimiter))).length()==1 && (copy_str.substr(0,copy_str.find(delimiter)))[0]>='a' && (copy_str.substr(0,copy_str.find(delimiter)))[0]<='z')||
	   ((copy_str.substr(0,copy_str.find(delimiter))).length()>1 && (copy_str.substr(0,copy_str.find(delimiter)))[0]>='a' && (copy_str.substr(0,copy_str.find(delimiter)))[0]<='z' && 
	   	(copy_str.substr(0,copy_str.find(delimiter)))[1]>='0' && (copy_str.substr(0,copy_str.find(delimiter)))[1]<='9'))
			node->vartype.push_back(0);
		else
			node->vartype.push_back(1);

	return node;
}

void print_sentence(vector<struct clause> sentence)
{
	int count=0, c=0;

	vector<struct clause>::const_iterator it;
	for (it=begin(sentence); it!=end(sentence); it++)
	{
	   	
		if(it->neg==1)
		cout << "~";

		cout << it->pred << "(";

		vector<string>::const_iterator it1;
		for (it1=begin(it->variables); it1!=end(it->variables); it1++)
		{
		   	cout << *it1;
		   	if(c<it->variables.size()-1)
		   		cout << ",";
		   	c++;
		}
		cout << ")";

	   	if(count < sentence.size()-1)
	   		cout << " | ";
	   	count++;
	}
	
}

vector<struct clause> build_sentence(string s)
{
	int len= s.length();
	int i=0, pos=0;
	string copy_str=s;

	vector<struct clause> sentence;

	string delimiter = " | ";

	pos=copy_str.find(delimiter);	
	while(pos!= std::string::npos)
	{
		string token = copy_str.substr(0, pos);

		struct clause *node= build_query(token);
		sentence.push_back(*node);

		copy_str.erase(0, pos+delimiter.length());
		pos=copy_str.find(delimiter);
	}

	struct clause *node= build_query(copy_str);
	sentence.push_back(*node);

	return sentence;
}

string convert_to_string(vector<struct clause> sen)
{
	string sentence;
	int count1=0, count2=0;
	vector<struct clause>::const_iterator it;

	for (it=begin(sen); it!=end(sen); it++)
	{
		if(it->neg==1)
			sentence.append("~");

		sentence.append(it->pred);
		sentence.append("(");

		vector<string>::const_iterator it1;
		count1=0;

		for (it1=begin(it->variables); it1!=end(it->variables); it1++)
		{
		   	sentence.append(*it1);
		   	if(count1 < (it->variables).size()-1)
		   		sentence.append(",");
		   	count1++;
		}

		sentence.append(")");

		if(count2<sen.size()-1)
		   	sentence.append(" | ");
		count2++;
	}
	return sentence;
}

string standardise(string str, int n)
{
	vector<struct clause> sen= build_sentence(str);
	vector<struct clause>::const_iterator it;
	int c=0, count1,count2=0;

	string newstr="";
	for (it=begin(sen); it!=end(sen); it++)
	{
		if(it->neg==1)
			newstr.append("~");

		newstr.append(it->pred);
		newstr.append("(");

		vector<string>::const_iterator it1;
		vector<int>::const_iterator it2=begin(it->vartype);

		count1=0;

		for (it1=begin(it->variables); it1!=end(it->variables); it1++)
		{
			cout<< "";
		   	newstr.append(*it1);
		   	string vtemp= *it1;
		   	char v= vtemp[0];

		   	if(*it2==0)
		   	{
		   		vartrack[atoi(&v)-97]= vartrack[atoi(&v)-97]+1;
		   		newstr.append(to_string(n));
		   	}

		   	if(count1 < (it->variables).size()-1)
		   		newstr.append(",");
		   	count1++;

		   	if(it2!=end(it->vartype))
		 	  	it2++;
		}

		newstr.append(")");

		if(count2<sen.size()-1)
		   	newstr.append(" | ");
		count2++;
	}
	return newstr;
}

void print_clause(struct clause* c)
{
	int count=0;

	if(c->neg==1)
		cout << "~";

	cout << c->pred << "(";

	vector<string>::const_iterator it;
	for (it=begin(c->variables); it!=end(c->variables); it++)
	{
	   	cout << *it;
	   	if(count<c->variables.size()-1)
	   		cout << ",";
	   	count++;
	}
	cout << ")";
}


int is_in(string s, vector<string> sl)
{
	vector<string>::const_iterator it;
	for(it=begin(sl); it!=end(sl); it++)
	{
		string c= *it;
		string c_s= c.substr(s.find("/"));
		//cout << "s= " << s << " c= " <<c << endl;
		if(s.compare(c_s)==0)
			return 1;
	}
	return 0;
}

vector<string> unify(string a, string b)
{
	int i=0, pos_a=0, pos_b=0, flag=0;
	string copy_a=a;
	string copy_b=b;

	vector<string> substitution_list;
	vector<string>::const_iterator it;

	string delimiter= "~";
	pos_a=copy_a.find(delimiter);
	pos_b=copy_b.find(delimiter);

	if(pos_a!=std::string::npos && pos_b==std::string::npos)
		copy_a.erase(0, pos_a+delimiter.length());
	else if((pos_b!=std::string::npos && pos_a==std::string::npos))
		copy_b.erase(0, pos_b+delimiter.length());

	// cout<< copy_a << ":" << copy_b<<endl;

		delimiter= "(";
		pos_a=copy_a.find(delimiter);
		pos_b=copy_b.find(delimiter);

		//cout<< pos_a << " " << pos_b << endl;

		string pred_a= copy_a.substr(0, pos_a);
		string pred_b= copy_b.substr(0, pos_b);

		//cout<< pred_a << " " << pred_b << endl;

		copy_a.erase(0, pos_a+delimiter.length());
		copy_b.erase(0, pos_b+delimiter.length());

		//cout<< copy_a << " " << copy_b << endl;

		if(pred_a.compare(pred_b)==0)
		{
			delimiter= ",";
			pos_a=copy_a.find(delimiter);
			pos_b=copy_b.find(delimiter);

			while(pos_a!= std::string::npos)
			{
				string token_a = copy_a.substr(0, pos_a);
				string token_b = copy_b.substr(0, pos_b);
	
				int vart_a, vart_b;
				if((token_a.length()==1 && token_a[0]>='a' && token_a[0]<='z') ||
				  (token_a.length()>1 && token_a[0]>='a' && token_a[0]<='z' && int(token_a[1]-48)>=0 && int(token_a[1]-48)<=9))
					vart_a=0;
				else
					vart_a=1;

				if((token_b.length()==1 && token_b[0]>='a' && token_b[0]<='z') ||
				  (token_b.length()>1 && token_b[0]>='a' && token_b[0]<='z' && int(token_b[1]-48)>=0 && int(token_b[1]-48)<=9))
					vart_b=0;
				else
					vart_b=1;

				if(vart_a==0 && vart_b==1)
				{
					string temp= token_a+"/"+token_b;
					//cout<< temp;
					if(is_in(token_a,substitution_list))
						substitution_list.clear();
					else
					{
						substitution_list.push_back(temp);
						flag=1;
					}
				}
				else if(vart_a==1 && vart_b==0)
				{
					string temp= token_b+"/"+token_a;
					//cout<< temp;
					if(is_in(token_a,substitution_list))
						substitution_list.clear();
					else
					{
						substitution_list.push_back(temp);
						flag=1;
					}
				}

				copy_a.erase(0, pos_a+delimiter.length());
				copy_b.erase(0, pos_b+delimiter.length());

				pos_a=copy_a.find(delimiter);
				pos_b=copy_b.find(delimiter);
			}

			delimiter= ")";
			string token_a=copy_a.substr(0,copy_a.find(delimiter));
			string token_b=copy_b.substr(0,copy_b.find(delimiter));

			//cout<< token_a << " " << token_b << " "<< int(token_b[1]-48) << " "<< endl;
			int vart_a, vart_b;
			if((token_a.length()==1 && token_a[0]>='a' && token_a[0]<='z') ||
			  (token_a.length()>1 && token_a[0]>='a' && token_a[0]<='z' && int(token_a[1]-48)>=0 && int(token_a[1]-48)<=9))
				vart_a=0;
			else
				vart_a=1;

			if((token_b.length()==1 && token_b[0]>='a' && token_b[0]<='z') ||
			  (token_b.length()>1 && token_b[0]>='a' && token_b[0]<='z' && int(token_b[1]-48)>=0 && int(token_b[1]-48)<=9))
				vart_b=0;
			else
				vart_b=1;

			//cout<< vart_a << " " << vart_b << endl;
			
			if(vart_a==0 && vart_b==1)
			{
				string temp= token_a+"/"+token_b;
				//cout<< temp;
				if(is_in(token_a,substitution_list))
						substitution_list.clear();
				else
				{
					substitution_list.push_back(temp);
					flag=1;
				}
			}
			else if(vart_a==1 && vart_b==0)
			{
				string temp= token_b+"/"+token_a;
				//cout<< temp;
				if(is_in(token_a,substitution_list))
					substitution_list.clear();
				else
				{
					substitution_list.push_back(temp);
					flag=1;
				}
			}
	
		}

	cout<< substitution_list.size();
	return substitution_list;
}

// void unify_demo(string a, string b, vector<string*> *substitution_list)
// {
// 	int i=0, pos_a=0, pos_b=0;
// 	string copy_a=a;
// 	string copy_b=b;

// 	string delimiter= "~";
// 	pos_a=copy_a.find(delimiter);
// 	pos_b=copy_b.find(delimiter);

// 	if(pos_a!=std::string::npos && pos_b==std::string::npos)
// 		copy_a.erase(0, pos_a+delimiter.length());
// 	else if((pos_b!=std::string::npos && pos_a==std::string::npos))
// 		copy_b.erase(0, pos_b+delimiter.length());

// 	// cout<< copy_a << ":" << copy_b<<endl;

// 		delimiter= "(";
// 		pos_a=copy_a.find(delimiter);
// 		pos_b=copy_b.find(delimiter);

// 		//cout<< pos_a << " " << pos_b << endl;

// 		string pred_a= copy_a.substr(0, pos_a);
// 		string pred_b= copy_b.substr(0, pos_b);

// 		//cout<< pred_a << " " << pred_b << endl;

// 		copy_a.erase(0, pos_a+delimiter.length());
// 		copy_b.erase(0, pos_b+delimiter.length());

// 		//cout<< copy_a << " " << copy_b << endl;

// 		if(pred_a.compare(pred_b)==0)
// 		{
// 			delimiter= ",";
// 			pos_a=copy_a.find(delimiter);
// 			pos_b=copy_b.find(delimiter);

// 			while(pos_a!= std::string::npos)
// 			{
// 				string token_a = copy_a.substr(0, pos_a);
// 				string token_b = copy_b.substr(0, pos_b);
	
// 				int vart_a, vart_b;
// 				if((token_a.length()==1 && token_a[0]>='a' && token_a[0]<='z') ||
// 				  (token_a.length()>1 && token_a[0]>='a' && token_a[0]<='z' && int(token_a[1]-48)>=0 && int(token_a[1]-48)<=9))
// 					vart_a=0;
// 				else
// 					vart_a=1;

// 				if((token_b.length()==1 && token_b[0]>='a' && token_b[0]<='z') ||
// 				  (token_b.length()>1 && token_b[0]>='a' && token_b[0]<='z' && int(token_b[1]-48)>=0 && int(token_b[1]-48)<=9))
// 					vart_b=0;
// 				else
// 					vart_b=1;

// 				if(vart_a==0 && vart_b==1)
// 				{
// 					if(!is_in(token_a, (*substitution_list)))
// 					{	
// 						string temp[2]= {token_a,token_b};
// 						(*substitution_list).push_back(temp);
// 					}
// 				}
// 				else if(vart_a==1 && vart_b==0)
// 				{
// 					if(!is_in(token_b, (*substitution_list)))
// 					{	
// 						string temp[2]= {token_b,token_a};
// 						(*substitution_list).push_back(temp);
// 					}
// 				}

// 				copy_a.erase(0, pos_a+delimiter.length());
// 				copy_b.erase(0, pos_b+delimiter.length());

// 				pos_a=copy_a.find(delimiter);
// 				pos_b=copy_b.find(delimiter);
// 			}

// 			delimiter= ")";
// 			string token_a=copy_a.substr(0,copy_a.find(delimiter));
// 			string token_b=copy_b.substr(0,copy_b.find(delimiter));

// 			//cout<< token_a << " " << token_b << " "<< int(token_b[1]-48) << " "<< endl;
// 			int vart_a, vart_b;
// 			if((token_a.length()==1 && token_a[0]>='a' && token_a[0]<='z') ||
// 			  (token_a.length()>1 && token_a[0]>='a' && token_a[0]<='z' && int(token_a[1]-48)>=0 && int(token_a[1]-48)<=9))
// 				vart_a=0;
// 			else
// 				vart_a=1;

// 			if((token_b.length()==1 && token_b[0]>='a' && token_b[0]<='z') ||
// 			  (token_b.length()>1 && token_b[0]>='a' && token_b[0]<='z' && int(token_b[1]-48)>=0 && int(token_b[1]-48)<=9))
// 				vart_b=0;
// 			else
// 				vart_b=1;

// 			//cout<< vart_a << " " << vart_b << endl;
			
// 			if(vart_a==0 && vart_b==1)
// 			{
// 				if(!is_in(token_a, *(substitution_list)))
// 				{	
// 					string temp[2]= {token_a,token_b};
// 					(*substitution_list).push_back(temp);
// 				}
// 			}
// 			else if(vart_a==1 && vart_b==0)
// 			{
// 				if(!is_in(token_b, (*substitution_list)))
// 				{	
// 					string temp[2]= {token_b,token_a};
// 					(*substitution_list).push_back(temp);
// 				}
// 			}
	
// 	}
// 	cout<< (*substitution_list).size();
// 	vector<string*>::const_iterator it;
// 	for(it=begin(*substitution_list); it!=end(*substitution_list); it++)
// 	{
// 		string c1= (*it)[0];
// 		string c2= (*it)[1];
// 		cout << c1 << "/" << c2 << endl;
// 	}
// 	//return substitution_list;
// }

int is_goal(string sentence)
{
	string copy_str=sentence;

	string delimiter=" | ";
	int pos= copy_str.find(delimiter);
	string c1= copy_str.substr(0,pos);
	copy_str.erase(0,pos+delimiter.length());

	if(c1[0]=='~' && copy_str[0]!='~')
	{
		if((c1.substr(1,c1.length())).compare(copy_str)==0)
			return 1;
	}
	else if(c1[0]!='~' && copy_str[0]=='~')
	{
		if((copy_str.substr(1,copy_str.length())).compare(c1)==0)
			return 1;
	}
	return 0;
}

string replace_in_clause(string str, vector<string*> substitution_list)
{
	string result="";
	vector<string*>::const_iterator it;

	int i=0, pos=0, p=0, flag=0;
	string copy_str=str;

	string delimiter= "~";
	pos=copy_str.find(delimiter);

		delimiter= "(";
		pos=copy_str.find(delimiter);
		result.append(copy_str.substr(0, pos+1));
		copy_str.erase(0, pos+delimiter.length());


		delimiter= ",";
		pos=copy_str.find(delimiter);
		while(pos!= std::string::npos)
		{
			string token = copy_str.substr(0, pos);
			flag=0;
			if((token.length()==1 && token[0]>='a' && token[0]<='z')||
				(token.length()>1 && token[0]>='a' && token[0]<='z' && int(token[1]-48)>=0 && int(token[1]-48)<=9))
			{
				for(it=begin(substitution_list); it!=end(substitution_list); it++)
				{
					string v= (*it)[0];
					string rep= (*it)[1];
					// cout << v << "-" << rep << "-"<< endl;
					if(token.compare(v)==0)
					{
						result.append(rep);
						result.append(",");
						flag=1;
						break;
					}
				}
				if(flag==0)
				{
					result.append(token);
					result.append(",");
				}
			}
			else
			{
				result.append(token);
				result.append(",");
			}

			copy_str.erase(0, pos+delimiter.length());
			pos=copy_str.find(delimiter);
		}

		flag=0;
		delimiter= ")";
		string x= copy_str.substr(0,copy_str.find(delimiter));
		// cout<< x << endl;
		if((x.length()==1 && x[0]>='a' && x[0]<='z')||
		   (x.length()>1 && x[0]>='a' && x[0]<='z' && (x[1]>='0' && x[1]<='9') && int(x[1]-48)>=0 && int(x[1]-48)<=9))
			{
				for(it=begin(substitution_list); it!=end(substitution_list); it++)
				{
					// cout<< "b";
					string v= (*it)[0];
					string rep= (*it)[1];
					// cout << v << "-" << rep << endl;
					if(x.compare(v)==0)
					{
						// cout<< "b";
						result.append(rep);
						result.append(")");
						flag=1;
						break;
					}
					// if(flag==1)
					// 	break;
				}
				if(flag==0)
				{
					result.append(x);
					result.append(")");
				}
			}
			else
			{
				result.append(x);
				result.append(")");
			}

	// cout<< "re :" << result << "." << endl;
	return result;
}

string replace(string str, vector<string*> substitution_list)
{
	string result= "";
	string delimiter= " | ";
	string copy_str=str;
	// cout<< copy_str << endl;
	// cout << copy_str << endl;

	int pos= copy_str.find(delimiter);
	// cout<< pos;
	
	while(pos!=std::string::npos)
	{
		string token= copy_str.substr(0,pos);
		copy_str.erase(0,pos+delimiter.length());
		// cout<< token << endl;

		string res= replace_in_clause(token,substitution_list);
		// cout<<res <<endl;
		result.append(res);
		result.append(" | ");
		pos= copy_str.find(delimiter);
	}
	//cout<< copy_str << endl ;
	string res= replace_in_clause(copy_str,substitution_list);
	result.append(res);
	
	// cout << "replace: " << result << endl;
	return result;
}

string resolution(string a, string b, string p_a, string p_b)
{
	// cout<< a << ", " << b << ", " << p_a << ", " << p_b << endl;

	int pos1, pos2;
	int flag1=0, flag2=0;

	string p1= " | " + p_a + " | ";
	string p2= " | " + p_b + " | ";

	string q1= " | " + p_a;
	string q2= " | " + p_b;

	string r1= p_a + " | ";
	string r2= p_b + " | ";

	string new_a, new_b;

	if(a.find(p1)!=std::string::npos)
	{	
		pos1= a.find(p1);
		flag1=1;
	}
	else if(a.find(q1)!=std::string::npos)
	{	
		pos1= a.find(q1);
		flag1=2;
	}
	else if(a.find(r1)!=std::string::npos)
	{	
		pos1= a.find(r1);
		flag1=3;
	}
	else if(a.compare(p_a)==0)
	{	
		pos1= a.find(p_a);
		flag1=4;
	}	

	if(b.find(p2)!=std::string::npos)
	{	
		pos2= b.find(p2);
		flag2=1;
	}
	else if(b.find(q2)!=std::string::npos)
	{	
		pos2= b.find(q2);
		flag2=2;
	}
	else if(b.find(r2)!=std::string::npos)
	{	
		pos2= b.find(r2);
		flag2=3;
	}
	else if(b.compare(p_b)==0)
	{	
		pos2= b.find(p_b);
		flag2=4;
	}

	// cout<< pos1 << ", " << flag1 << endl;
	// cout<< pos2 << ", " << flag2 << endl;
	
	if(flag1==1)
		a.erase(pos1,pos1+p1.length());
	else if (flag1==2)
		a.erase(pos1,pos1+q1.length());
	else if(flag1==3)
		a.erase(pos1,pos1+r1.length());
	else if(flag1==4)
		a="";


	if(flag1==1)
		b.erase(pos2,pos2+p2.length());
	else if (flag2==2)
		b.erase(pos2,pos2+q2.length());
	else if(flag2==3)
		b.erase(pos2,pos2+r2.length());
	else if(flag2==4)
		b="";

	// cout<< "a: "<< a << endl;
	// cout<< "b: "<< b << endl;

	string result="";

	if(a.length()>0)
		result.append(a);
	if(a.length()>0 && b.length()>0)
		result.append(" | ");
	if(b.length()>0)
		result.append(b);

	// cout << result << endl;
	return result;
}

int resolve_theorem(string a, vector<string> KB)
{
	int len_s= KB.size();
	int i=0, j=0, ctr=0, c=0, count=0;

	int *markers= new int(len_s);
	for(int i=0; i<len_s; i++)
		markers[i]=0;

	vector<string> copy_KB;

	vector<string>::const_iterator it;
	for(it=begin(KB); it!=end(KB); it++)
		copy_KB.push_back(*it);

	// Look for sentences
	string working_sentence=a;
	vector<string*> substitution_list;

	while(count<len_s)
	{
		cout << "************************ iteration : "<< count++ << " *************************" << endl;
		// count++;
		int flag=0;
		global_flag=0;
		vector<struct clause> sentence= build_sentence(working_sentence);
		vector<struct clause>::const_iterator it1;
		string p; 

		for(it1=begin(sentence); it1!=end(sentence); it1++)
		{
			//cout << copy_KB[copy_KB.size()-1] << "+" << working_sentence << endl;
			
			p="";
			int index=0;

			if(it1->neg==0)
				p.append(" ~");
			else
				p.append(" ");
			
			p.append(it1->pred);
			p.append("(");
			//cout<< p<<endl;

			flag=0;

			if(copy_KB[copy_KB.size()-1].compare(working_sentence)==0)
				markers[len_s-1]=0;

			for(i=0; i<len_s; i++)
			{
				string fact= " "+copy_KB[i];
				index=fact.find(p);

				
				if(index!=std::string::npos)
				{
					if(markers[i]==ctr)
					{
							markers[i]++;
							flag=1;
							break;
					}
					else
						continue;
				}
				if(flag==0 && i==len_s-1)
				{
					ctr++;
					i=0;
				}
			}

			if(flag==1)
				break;
		}
		// cout<< flag << "-"<< i <<endl;

		if(flag==1)
		{
			// cout <<  "i= " << i << ":" << copy_KB[i] << endl;
			vector<struct clause> dummy;
			dummy.push_back(*it1);
			string a= convert_to_string(dummy);
			//cout<< a << endl;

			p= p.substr(1,p.length());
			string fact= copy_KB[i];
			// cout<< copy_KB[i];
			// cout <<fact << endl;
			int pos= fact.find(p);
			cout<< pos << endl;
			if(pos>0)
				fact.erase(0,pos-1);
			else
				fact.erase(0,pos);

			// if(fact[0]=='~')
			// 	fact.erase(0,1);


			//cout<< fact << endl;

			int endpos= fact.find(")");
			string b;
			if(fact[0]==' ')
				b= fact.substr(1,endpos+1);
			else
				b= fact.substr(0,endpos+1);

			// cout << "main b= ." <<  b << "." << endl;
 			// cout<< a << "->" << b << endl;

 			// ***************** UNIFICATION ******************
			vector<string> sl= unify(a,b);
			cout<< sl.size();
			
			//Updating substitution list
			if(sl.size()>0)
			{
				int flag2=0;

				vector<string>::const_iterator itx;
				// int flag2=0;
				for(itx=begin(sl); itx!=end(sl); itx++)
				{
					cout<< (*itx) << endl;
					string token = *itx;
					int pos_m= token.find("/");

					string x= token.substr(0,pos_m);
					string y= token.substr(pos_m+1,token.length());
					// cout<< x << "-" << y << endl;

					vector<string*>::const_iterator r;
					for(r=begin(substitution_list); r!=end(substitution_list); r++)
					{
						string c1= (*r)[0];
						string c2= (*r)[1];
						if(c1==x)
						{
							flag2=1;
							break;
						}
					}
					if(flag2==0)
					{
						string f[2]= {x,y};
						substitution_list.push_back(f);
					}
				// 	else
				// 	{
				// 		vector<string*>::const_iterator ity;
				// 		for(ity=begin(substitution_list); ity!=end(substitution_list); ity++)
				// 		{
				// 			string v= (*ity)[0];
				// 			string val= (*ity)[1];
				// 			// cout<< v <<"-"<< val<< "-"<<endl;
				// 			if(v.compare(x)==0 && y.compare(val)!=0)
				// 				global_flag=0;
				// 		}
				// 	}
				}
			
			}
			// vector<string*>::const_iterator r;
			// for(r=begin(substitution_list); r!=end(substitution_list); r++)
			// {
			// 	string c1= (*r)[0];
			// 	string c2= (*r)[1];
			// 	cout << substitution_list.size() << " c1= " << c1 << " c2= " << c2 << endl;
			// }
						
			// cout<< working_sentence << ", " <<  copy_KB[i] << ", " << a << ", " << b << endl;

			string m= replace(working_sentence, substitution_list);
			string n= replace(copy_KB[i], substitution_list);

			string m_a= replace(a,substitution_list);
			string n_b= replace(b,substitution_list);

			cout << substitution_list.size() << endl;
			cout << "i: " << i << ", "<< copy_KB[i] <<endl;
			cout << "m= " << m << ", n= " << n << ", m_a= " << m_a << ", n_b= " << n_b << endl;

			// ******************** RESOLUTION ********************
			working_sentence="";
			//cout<< cpy << " -> " << copy_KB[i] << endl;
			working_sentence= resolution(m, n, m_a, n_b);
			cout << working_sentence.length() << ":"<< working_sentence << endl;
			if(working_sentence.length()==0)
				return 1;

			// sentence not unified - fail
			// when it goes into a loop - fail
		}
		if(is_goal(working_sentence))
			return 1;
	}
	return 0;
}

void print_output(int *flags, int n)
{
	int i;

	ofstream myfile;

  	// Write the result to output file
  	myfile.open ("output.txt");

 	for(i=0; i<n; i++)
 	{
 		if(flags[i]==1)
 			myfile << "TRUE";
 		else myfile << "FALSE";
 		if(i!=n-1)
	 		myfile << endl;
 	}
 	myfile.close();
}

void prove_queries(vector<string> query_list, vector<string> KB)
{
	int len_q= query_list.size();
	int len_s= KB.size();
	int i; 

	int *flags= new int(len_q);

	for(i=0; i<len_q; i++)
	{
		if(query_list[i][0]!='~')
		// 	KB.push_back(query_list[i].substr(1,query_list[i].length()));
		// else
		{
			string temp= "~";
			temp.append(query_list[i]);
			KB.push_back(temp);
			flags[i]=resolve_theorem(temp, KB);
		}
		else
		{
			KB.push_back(query_list[i].substr(1,query_list[i].length()));
			flags[i]=resolve_theorem(query_list[i].substr(1,query_list[i].length()), KB);
		}
		
		if(flags[i]==0)
			KB.pop_back();
	}
	//cout << KB.size() << endl;
	print_output(flags, len_q);
}

int main()
{
	string str;
	int nq=0, ns=0, count=0;
	ifstream file("input.txt");	
	vector<string> query_list;
	vector<string> KB;

	while(getline(file,str))
	{
		if(count==0)
			nq=stoi(str);
		else if(count<=nq)
		{
			//struct clause *c= build_query(str);
			query_list.push_back(str);
		}
		else if(count==nq+1)
			ns=stoi(str);
		else if(count<=nq+ns+1)
		{
			string temp= standardise(str,count-nq-1);
			KB.push_back(temp);
		}
		count++;
	}	

	prove_queries(query_list, KB);

	return 0;
}

// int main()
// {
// 	vector<string*> sl;
// 	// string f[2]={"x","Jack"};
// 	// sl.push_back(f);
// 	string a= "C(x,Fei)";
// 	string b= "C(Fang, Fei)";
// 	unify_demo(a, b, &sl);

// 	a="";
// 	b="";
// 	a="A(Seulgi)";
// 	b="A(s)";
// 	unify_demo(a,b, &sl);

// 	a="";
// 	b="";
// 	a="A(x)";
// 	b="A(Yeri)";
// 	unify_demo(a,b, &sl);

// 	cout<< sl.size();
// 	vector<string*>::const_iterator it;
// 	for(it=begin(sl); it!=end(sl); it++)
// 	{
// 		string c1= (*it)[0];
// 		string c2= (*it)[1];
// 		cout << c1 << "/" << c2 << endl;
// 	}

// 	return 0;
// }
