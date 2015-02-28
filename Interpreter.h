#ifndef _INTERPRETER_H
#define  _INTERPRETER_H

#include<iostream>
#include<string>
#include<cstdlib>
#include "SQLStatement.h"
#include "catalog.h"
#include "constant.h"
#include "Handle.h"
using namespace std;



extern catalog ctlg;
extern Handle hdl;

class Interpreter
{
	int sqlType;
	int start;
	int last;
	string str;
	bool isAtt;
public:
	Interpreter();
	string readNextWord();
	void set(string s);
	void reset();
	string toLower(string str);
	attribute tellDataType(string st,attribute att);
	void tellSqlType();
	void controlor();

	string select(SQLSelect &sqlSelect);
	string dropTable(SQLDropTable &sqlDropTable);
	string dropIndex(SQLDropIndex &sqlDropIndex);
	string dropDatabase(SQLDropDatabase &sqlDropDatabase);
	string execfile(SQLExecFile &sqlExecFile);
	string deletefrom(SQLDelete &sqlDelete);
	SQLQuit quitSql();
	string helpSql(SQLHelp &sqlHelp);
	string useDB(SQLUse &sqlUse);
	string createIndex(SQLCreateIndex &sqlCreateIndex);
	string createDatabase(SQLCreateDatabase &sqlCreateDatabase);
	string createTable(SQLCreateTable &sqlCreateTable);
	string insert(SQLInsert &sqlInsert);


};


#endif 