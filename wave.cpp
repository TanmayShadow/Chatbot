/*
 * wave.cpp
 *
 *  Created on: Dec 25, 2022
 *      Author: Tanmay
 */
#include<iostream>
#include<fstream>
#include<string>
#include<string.h>

using namespace std;

class Login
{
	char password[30];
public:
	Login()
	{
		strcpy(password,"");
	}
	Login(char pass[])
	{
		strcpy(password,pass);
	}
	bool checklogin(char pass[])
	{
		if(strcmp(password,pass)==0)
			return true;
		return false;
	}
};

class Wave
{
	char question[100];
	char answer[100];
public:
	Wave()
	{

	}
	Wave(char que[],char ans[])
	{
		strcpy(question,que);
		strcpy(answer,ans);
	}
	bool string_compare(char que[])
	{
		if(strlen(que)==strlen(question))
		{
			for(unsigned int i=0;i<strlen(question);i++)
			{
				if(tolower(question[i])!=tolower(que[i]))
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}

	void showAnswer()
	{
		cout<<"wave:\t"<<answer<<endl;
	}

	int builtInResponse(char que[])
	{
		if(strstr(que,"your name"))
			cout<<"Wave:\tI am wave"<<endl;
		else if(strstr(que,"how are you"))
			cout<<"Wave:\tI'm doing great, thank you for asking.How about you?"<<endl;
		else if(strstr(que,"i am fine"))
			cout<<"Wave:\tI'm glad to hear that. Fine is a good word. It means having satisfactory quality or condition.";
		else if(strstr(que,"stop") || strstr(que,"exit") || strstr(que,"bye"))
		{
			cout<<"Wave:\tGoodbye. Have a wonderful day. I hope to talk to you again soon. 👋";
			exit(0);
		}
		else if(strstr(que,"admin") || strstr(que,"login"))
			return 2;
		else
			return 1;
		return 0;
	}

	~Wave()
	{

	}
};

int main()
{
	char que[200];
	string name;
	ofstream fout("database.txt",ios::out | ios::app | ios::binary);//creating database file
	fout.close();
	ofstream login("login.txt",ios::out | ios::binary | ios::app);//creating login file
	login.close();
	ifstream fin;
	cout<<"Wave:\tHello User, may I know your nickname!"<<endl;
	cout<<"User:\t";
	cin>>name;
	cout<<"Wave:\tThat's the nice name.."<<endl;
	cin.get();
	do
	{
		cout<<name<<":\t";
		cin.getline(que,200);
		Wave w1;
		int res = w1.builtInResponse(que);
		if(res==1)
		{
			fin.open("database.txt",ios::in | ios::binary);
			int flag = 0;
			while(fin.read((char*)&w1,sizeof(Wave)))
			{
				if(w1.string_compare(que))
				{
					flag=1;
					w1.showAnswer();
					break;
				}
			}
			fin.close();
			if(flag==0)
			{
				char ch;
				cout<<"Wave:\tSorry, I have no idea. Do you have an answer to the question?(Y/N)"<<endl;
				cout<<name<<":\t";
				cin>>ch;
				cin.get();
				if(ch=='Y' || ch=='y')
				{
					char ans[200];
					cout<<"Wave:\tPlease share your answer?"<<endl;
					cout<<name<<":\t";
					cin.getline(ans,200);
					Wave newwave(que,ans);
					fout.open("database.txt",ios::out | ios::app | ios::binary);
					fout.write((char*)&newwave,sizeof(Wave));
					fout.close();
					cout<<"Wave:\tThanks for your answer."<<endl;
				}
				else
				{
					cout<<"Wave:\tOkay, no problem....."<<endl;
				}
			}
		}
		else if(res==2)
		{
			ifstream loginfile("login.txt",ios::in | ios::binary);\
			loginfile.seekg(0,ios::end);
			if(loginfile.tellg()==0)
			{
				cout<<"Wave:\tEnter new password for admin"<<endl;
				cout<<name<<":\t";
				char pass[30];
				cin.getline(pass,30);
				Login l(pass);
				ofstream loginfile("login.txt",ios::out | ios::binary);
				loginfile.write((char*)&l,sizeof(Login));
				loginfile.close();
			}
			else
			{
				loginfile.seekg(0);
				char pass[30];
								cout<<"Wave:\tEnter password"<<endl;
								cout<<name<<":\t";
								cin.getline(pass, 30);
								Login l;
								loginfile.read((char*)&l,sizeof(Login));
								if(l.checklogin(pass))
								{
									char ans[200];
									cout<<"Wave:\tEnter the question to be updated"<<endl;
									cout<<name<<":\t";
									cin.getline(que,200);
									cout<<"Wave:\tEnter the new answer"<<endl;
									cout<<name<<":\t";
									cin.getline(ans,200);
									Wave newwave(que,ans);
									fin.open("database.txt",ios::in | ios::binary);
									ofstream newfile("temp.txt",ios::out | ios::binary);
									while(fin.read((char*)&w1,sizeof(Wave)))
									{
										if(w1.string_compare(que))
										{
											newfile.write((char*)&newwave,sizeof(Wave));
										}
										else
										{
											newfile.write((char*)&w1, sizeof(Wave));
										}
									}
									fin.close();
									newfile.close();
									remove("database.txt");
									rename("temp.txt","database.txt");
								}
								else
									cout<<"Wave:\tInvalid Password!"<<endl;
			}
		}
	}while(strcmp(que,"exit") || strcmp(que,"stop"));
	return 0;
}
