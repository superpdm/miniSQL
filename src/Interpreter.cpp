
#include "Interpreter.h"
/*#include "catalog.h"*/
#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include "Record.h"
;using namespace std;

	Interpreter::Interpreter()
	{
		sqlType=-1;
		start=0;
		last=0;
		isAtt=false;
		str.clear();
	}


	string Interpreter::readNextWord()
	{
		string nextWord;
		start=last+1;
		last=str.find_first_of(',',start);
		if(isAtt && last<str.length()){
			isAtt=true;
		}
		else{
			last=str.find_first_of(' ',start);
			isAtt=false;
		}
		
		if(last==str.length()){
			return "";
		}
		else{
			nextWord.assign(str,start,last-start);
			return nextWord;
		}			
	}


	void Interpreter::set(string s)
	{
		str=s;
		str=' '+str+' ';
		int pos1=0,pos2;
		bool dup=false;		
		//消除换行
		pos1=0;
		pos2=str.find_first_of('\n',pos1);
		while(pos2<str.length()){
			str[pos2]=' ';
			pos2=str.find_first_of('\n',pos1);
		}
		//消除制表
		pos1=0;
		pos2=str.find_first_of('\t',pos1);
		while(pos2<str.length()){
			str[pos2]=' ';
			pos2=str.find_first_of('\t',pos1);
		}
		//消除重复空格
		pos1=0;
		pos2=str.find_first_of(' ',pos1);
		while(pos2<str.length()-1){
			
			dup=false;
			if(str[pos2+1]==' '){
				str.replace(pos2,2,1,' ');
				dup=true;
			}
			if(dup){
				pos1=pos2;
			}
			else{
				pos1=pos2+1;
			}
			pos2=str.find_first_of(' ',pos1);
		}
		//消除逗号后的空格
		pos1=0;
		pos2=str.find_first_of(',',pos1);
		while(pos2<str.length()-1){
			dup=false;
			if(str[pos2+1]==' '){
				str.replace(pos2,2,1,',');
				dup=true;
			}
			if(dup){
				pos1=pos2;
			}
			else{
				pos1=pos2+1;
			}
			pos2=str.find_first_of(',',pos1);
		}
		//消除逗号前的空格
		pos1=0;
		pos2=str.find_first_of(' ',pos1);
		while(pos2<str.length()-1){
			dup=false;
			if(str[pos2+1]==','){
				str.replace(pos2,2,1,',');
				dup=true;
			}
			if(dup){
				pos1=pos2;
			}
			else{
				pos1=pos2+1;
			}
			pos2=str.find_first_of(' ',pos1);
		}
		//在'('前后插入空格
		pos1=0;
		pos2=str.find_first_of('(',pos1);
		while(pos2<str.length()-1){
			dup=false;
			if(str[pos2+1]!=' '){
				str.insert(pos2+1,1,' ');
				dup=true;
			}
			if(str[pos2-1]!=' '){
				str.insert(pos2,1,' ');
				dup=false;
			}
			if(dup){
				pos1=pos2;
			}
			else{
				pos1=pos2+1;
			}
			pos2=str.find_first_of('(',pos1);
		}
		//在')'前后插入空格
		pos1=0;
		pos2=str.find_first_of(')',pos1);
		while(pos2<str.length()-1){
			dup=false;
			if(str[pos2+1]!=' '){
				str.insert(pos2+1,1,' ');
				dup=true;
			}
			if(str[pos2-1]!=' '){
				str.insert(pos2,1,' ');
				dup=false;
			}
			if(dup){
				pos1=pos2;
			}
			else{
				pos1=pos2+1;
			}
			pos2=str.find_first_of(')',pos1);
		}
		//在'='前后插入空格
		pos1=0;
		pos2=str.find_first_of('=',pos1);
		while(pos2<str.length()-1){
			dup=false;
			if(str[pos2+1]!=' ' && str[pos2+1]!='<'){
				str.insert(pos2+1,1,' ');
				dup=true;
			}
			if(str[pos2-1]!=' ' && str[pos2-1]!='>'){
				str.insert(pos2,1,' ');
				dup=false;
			}
			if(dup){
				pos1=pos2;
			}
			else{
				pos1=pos2+1;
			}
			pos2=str.find_first_of('=',pos1);
		}
		//在'<'前后插入空格，在"<>"前插入空格
		pos1=0;
		pos2=str.find_first_of('<',pos1);
		while(pos2<str.length()-1){
			dup=false;
			if(str[pos2+1]!=' ' && str[pos2+1]!='>'){
				str.insert(pos2+1,1,' ');
				dup=true;
			}
			if(str[pos2-1]!=' ' && str[pos2-1]!='='){
				str.insert(pos2,1,' ');
				dup=false;
			}
			if(dup){
				pos1=pos2;
			}
			else{
				pos1=pos2+1;
			}
			pos2=str.find_first_of('<',pos1);
		}
		//在'>'前后插入空格，在"<>"后插入空格
		pos1=0;
		pos2=str.find_first_of('>',pos1);
		while(pos2<str.length()-1){
			dup=false;
			if(str[pos2+1]!=' ' && str[pos2+1]!='='){
				str.insert(pos2+1,1,' ');
				dup=true;
			}
			if(str[pos2-1]!=' ' && str[pos2-1]!='<'){
				str.insert(pos2,1,' ');
				dup=false;
			}
			if(dup){
				pos1=pos2;
			}
			else{
				pos1=pos2+1;
			}
			pos2=str.find_first_of('>',pos1);
		}
		
	}

	void Interpreter::reset(){
		sqlType=-1;
		start=0;
		last=0;
		isAtt=false;
		str.clear();
	}

	string Interpreter::toLower(string str){
		char c;
		unsigned int i;
		for(i=0;i<str.length();i++){
			c=str.at(i);
			if(c>='A'&&c<='Z'){
				str[i]=c-'A'+'a';
			}
		}
		return str;
	}

	attribute Interpreter::tellDataType(string st,attribute att){
		string tdt;
		int s=0,l=0;
		int len=0,prc=0;
		unsigned int i;

		st=' '+st+' ';

		s=l+1;
		l=st.find_first_of(' ',s);				
		tdt.assign(st,s,l-s);
		
		if(toLower(tdt)=="int"){
			cout<<"INT"<<endl;
			att.dataType=T_INT;
			att.len=4;
		}
		else if(toLower(tdt)=="char"){
			s=l+1;
			l=st.find_first_of(' ',s);				
			tdt.assign(st,s,l-s);
			if(tdt=="("){
				s=l+1;
				l=st.find_first_of(' ',s);				
				tdt.assign(st,s,l-s);
				if(tdt==""){
					cout<<"Sql ERROR"<<endl;
				}
				else{
					for(i=0;i<tdt.length();i++){
						len=len*10+tdt[i]-'0';
					}
					s=l+1;
					l=st.find_first_of(' ',s);				
					tdt.assign(st,s,l-s);
					if(tdt==")"){
						cout<<"长度为 "<<len<<" 的 CHAR"<<endl;
						att.dataType=T_CHAR;
						att.len=len;
					}
					else{
						cout<<"Sql ERROR"<<endl;
					}
				}
			}
			else{
				cout<<"CHAR"<<endl;
				att.dataType=T_CHAR;
				att.len=1;
			}
		}
		else if(toLower(tdt)=="smallint"){
			cout<<"SMALL INT"<<endl;
			att.dataType=T_INT;
		}
		else if(toLower(tdt)=="varchar"){
			s=l+1;
			l=st.find_first_of(' ',s);				
			tdt.assign(st,s,l-s);
			if(tdt=="("){
				s=l+1;
				l=st.find_first_of(' ',s);				
				tdt.assign(st,s,l-s);
				if(tdt==""){
					cout<<"Sql ERROR"<<endl;
				}
				else{
					for(i=0;i<tdt.length();i++){
						len=len*10+tdt[i]-'0';
					}
					s=l+1;
					l=st.find_first_of(' ',s);				
					tdt.assign(st,s,l-s);
					if(tdt==")"){
						cout<<"最大长度为 "<<len<<" 的 VARCHAR"<<endl;
						att.dataType=T_CHAR;
						att.len=len;
					}
					else{
						cout<<"Sql ERROR"<<endl;
					}
				}
			}
			else{
				cout<<"Sql ERROR"<<endl;
			}
		}
		else if(toLower(tdt)=="float"){
			s=l+1;
			l=st.find_first_of(' ',s);				
			tdt.assign(st,s,l-s);
			if(tdt=="("){
				s=l+1;
				l=st.find_first_of(' ',s);				
				tdt.assign(st,s,l-s);
				if(tdt==""){
					cout<<"Sql ERROR"<<endl;
				}
				else{
					for(i=0;i<tdt.length();i++){
						len=len*10+tdt[i]-'0';
					}
					s=l+1;
					l=st.find_first_of(' ',s);				
					tdt.assign(st,s,l-s);
					if(tdt==")"){
						cout<<"宽度为 "<<len<<" 的 FLOAT"<<endl;
						att.dataType=T_FLOAT;
						att.len=4;
					}
					else{
						cout<<"Sql ERROR"<<endl;
					}
				}
			}
			else{
				cout<<"FLOAT"<<endl;
				att.dataType=T_FLOAT;
				att.len=4;
			}
		}
		else if(toLower(tdt)=="numeric"){
			s=l+1;
			l=st.find_first_of(' ',s);				
			tdt.assign(st,s,l-s);
			if(tdt=="("){
				s=l+1;
				l=st.find_first_of(',',s);				
				tdt.assign(st,s,l-s);
				if(tdt==""){
					cout<<"Sql ERROR"<<endl;
				}
				else{
					for(i=0;i<tdt.length();i++){
						len=len*10+tdt[i]-'0';
					}
					s=l+1;
					l=st.find_first_of(' ',s);				
					tdt.assign(st,s,l-s);
					if(tdt==""){
						cout<<"Sql ERROR"<<endl;
					}
					else{
						for(i=0;i<tdt.length();i++){
							prc=prc*10+tdt[i]-'0';
						}
						s=l+1;
						l=st.find_first_of(' ',s);				
						tdt.assign(st,s,l-s);
						if(tdt==")"){
							cout<<"宽度为 "<<len<<" 小数点后 "<<prc<<" 位的 "<<"NUMERIC"<<endl;
						}
						else{
							cout<<"Sql ERROR"<<endl;
						}
					}
				}
			}
			else{
				cout<<"Sql ERROR"<<endl;
			}
		}
		else{
			cout<<"Sql ERROR"<<endl;
		}	
		return att;
	}

	void Interpreter::tellSqlType(){
		string typeIns;
		typeIns=readNextWord();
				
		if(toLower(typeIns)=="insert")
			sqlType=0;
		else if(toLower(typeIns)=="select")
			sqlType=1;
		else if(toLower(typeIns)=="create"){
			//start=last+1;
			typeIns=readNextWord();
			if(typeIns==""){
				sqlType=-1;
			}
			else{
				if(toLower(typeIns)=="table")
					sqlType=2;
				else if(toLower(typeIns)=="database")
					sqlType=3;
				else if(toLower(typeIns)=="index")
					sqlType=4;
				else
					sqlType=-1;
			}
		}
		else if(toLower(typeIns)=="help")
			sqlType=5;
		else if(toLower(typeIns)=="delete")
			sqlType=6;
		else if(toLower(typeIns)=="use")
			sqlType=7;
		else if(toLower(typeIns)=="execfile")
			sqlType=8;
		else if(toLower(typeIns)=="drop"){
			//start=last+1;
			typeIns=readNextWord();
			if(typeIns==""){
				sqlType=-1;
			}
			else{
				if(toLower(typeIns)=="index")
					sqlType=9;
				else if(toLower(typeIns)=="database")
					sqlType=10;
				else if(toLower(typeIns)=="table")
					sqlType=11;
				else
					sqlType=-1;
			}
		}
		else if(toLower(typeIns)=="quit")
			sqlType=12;
		else
			sqlType=-1;		
	}

	void Interpreter::controlor(){
		switch(sqlType){
			case 0:{
				cout<<"-----------------------------------"<<endl;
				cout<<"Sql type is #INSERT#"<<endl;
				cout<<"-----------------------------------"<<endl;
				//call insert
				SQLInsert sqlInsert;

				if(insert(sqlInsert)=="")
				{
					exeSQL::exeInsert(ctlg,sqlInsert,hdl);
				}
				else
				{
					cout<<insert(sqlInsert)<<endl;
				}
				cout<<"-----------------------------------"<<endl;
				break;
			}
			case 1:{
				cout<<"-----------------------------------"<<endl;
				cout<<"Sql type is #SELECT#"<<endl;
				cout<<"-----------------------------------"<<endl;
				//call select
				SQLSelect sqlSelect;
				if(select(sqlSelect)=="")
				{
					cout<<"-----------------------------------"<<endl;
					int size = ctlg.dbs[ctlg.curDB].GetTable(sqlSelect.tblName)->attrNum;
					for(int i =0;i<size;++i)
					{
						if(i!=size-1){
							cout<<ctlg.dbs[ctlg.curDB].GetTable(sqlSelect.tblName)->attrs[i].name<<setw(9);
						}
						else{
							cout<<ctlg.dbs[ctlg.curDB].GetTable(sqlSelect.tblName)->attrs[i].name<<endl;
						}
						
					}
					cout<<endl;
					cout<<"-----------------------------------"<<endl;
					exeSQL::exeSelect(ctlg,sqlSelect,hdl);
					cout<<"-----------------------------------"<<endl;
				}
				else
				{

					cout<<select(sqlSelect)<<endl;
				}
				cout<<"-----------------------------------"<<endl;
				break;
			}
			case 2:{
				cout<<"-----------------------------------"<<endl;
				cout<<"Sql type is #CREATE TABLE#"<<endl;
				cout<<"-----------------------------------"<<endl;
				//call create table
				SQLCreateTable sqlCreateTable;
				if(createTable(sqlCreateTable)=="")
				{
					ctlg.dbs[ctlg.curDB].create_table(sqlCreateTable);
				}
				else
				{

					cout<<createTable(sqlCreateTable)<<endl;
				}

				cout<<"-----------------------------------"<<endl;
				break;
			}
			case 3:{
				cout<<"-----------------------------------"<<endl;
				cout<<"Sql type is #CREATE DATABASE#"<<endl;
				cout<<"-----------------------------------"<<endl;
				//call create database
				SQLCreateDatabase sqlCreateDatabase;
				if(createDatabase(sqlCreateDatabase)=="")
				{
					ctlg.create_database(sqlCreateDatabase);
				}
				else
				{

					cout<<createDatabase(sqlCreateDatabase)<<endl;
				}
				
				
				cout<<"-----------------------------------"<<endl;
				break;
			}
			case 4:{
				cout<<"-----------------------------------"<<endl;
				cout<<"Sql type is #CREATE INDEX#"<<endl;
				cout<<"-----------------------------------"<<endl;
				//call create index
				SQLCreateIndex sqlCreateIndex;
				if(createIndex(sqlCreateIndex)=="")
				{
					/*ctlg.dbs[ctlg.curDB].create_index(sqlCreateIndex);*/
					exeSQL::exeCreateIndex(ctlg,sqlCreateIndex,hdl);
				}
				else
				{

					cout<<createIndex(sqlCreateIndex)<<endl;
				}
				
				cout<<"-----------------------------------"<<endl;
				break;
			}
			case 5:{
				cout<<"-----------------------------------"<<endl;
				cout<<"Sql type is #HELP#"<<endl;
				cout<<"-----------------------------------"<<endl;
				//call help
				SQLHelp sqlHelp;
				helpSql(sqlHelp);
				
				cout<<"-----------------------------------"<<endl;
				break;
			}
			case 6:{
				cout<<"-----------------------------------"<<endl;
				cout<<"Sql type is #DELETE#"<<endl;
				cout<<"-----------------------------------"<<endl;
				//call delete
				SQLDelete sqlDelete;
				if(deletefrom(sqlDelete)=="")
				{
						exeSQL::exeDelete(ctlg,sqlDelete,hdl);
				}
				else
				{

					cout<<deletefrom(sqlDelete)<<endl;
				}
				
			
				cout<<"-----------------------------------"<<endl;
				break;
			}
			case 7:{
				cout<<"-----------------------------------"<<endl;
				cout<<"Sql type is #USE#"<<endl;
				cout<<"-----------------------------------"<<endl;
				//call use
				SQLUse sqlUse;
				if(useDB(sqlUse)=="")
				{
					ctlg.use_database(sqlUse);
				}
				else
				{

					cout<<useDB(sqlUse)<<endl;
				}

				
				cout<<"-----------------------------------"<<endl;
				break;
			}
			case 8:{
				cout<<"-----------------------------------"<<endl;
				cout<<"Sql type is #EXECFILE#"<<endl;
				cout<<"-----------------------------------"<<endl;
				//call execfile
				SQLExecFile sqlExecFile;
				if(execfile(sqlExecFile)=="")
				{
					
				}
				else
				{

					cout<<execfile(sqlExecFile)<<endl;
				}
				
				cout<<"-----------------------------------"<<endl;
				break;
			}
		    case 9:{
				cout<<"-----------------------------------"<<endl;
				cout<<"Sql type is #DROP INDEX#"<<endl;
				cout<<"-----------------------------------"<<endl;
				//call drop index
				SQLDropIndex sqlDropIndex;
				if(dropIndex(sqlDropIndex)=="")
				{
					ctlg.dbs[ctlg.curDB].drop_index(sqlDropIndex);
				}
				else
				{

					cout<<dropIndex(sqlDropIndex)<<endl;
				}
				
				
				cout<<"-----------------------------------"<<endl;
				break;
			}
			case 10:{
				cout<<"-----------------------------------"<<endl;
				cout<<"Sql type is #DROP DATABASE#"<<endl;
				cout<<"-----------------------------------"<<endl;
				//call drop database
				SQLDropDatabase sqlDropDatabase;
				if(dropDatabase(sqlDropDatabase)=="")
				{
					ctlg.drop_database(sqlDropDatabase);
				}
				else
				{

					cout<<dropDatabase(sqlDropDatabase)<<endl;
				}
			
				
				cout<<"-----------------------------------"<<endl;
				break;
			}
			case 11:{
				cout<<"-----------------------------------"<<endl;
				cout<<"Sql type is #DROP TABLE#"<<endl;
				cout<<"-----------------------------------"<<endl;
				//call drop table
				SQLDropTable sqlDropTable;
				if(dropTable(sqlDropTable)=="")
				{
					ctlg.dbs[ctlg.curDB].drop_table(sqlDropTable);
				}
				else
				{

					cout<<dropTable(sqlDropTable)<<endl;
				}
				

				cout<<"-----------------------------------"<<endl;
				break;
			}
			case 12:{
				cout<<"-----------------------------------"<<endl;
				cout<<"Sql type is #QUIT#"<<endl;
				cout<<"-----------------------------------"<<endl;
				//call quit
				quitSql();
				cout<<"-----------------------------------"<<endl;
				break;
			}
			case -1:{
				cout<<"-----------------------------------"<<endl;
				cout<<"Sql ERROR"<<endl;
				cout<<"-----------------------------------"<<endl;
				//call error
				break;
			}
		}
	}

	string Interpreter::select(SQLSelect &sqlSelect){
		string err="";
		string sel;
		int i=0;
		//start=last+1;
		sqlSelect.SQLType=1;
		isAtt=true;
		cout<<"指令：SELECT"<<endl;
		while(isAtt){
			sel=readNextWord();
			if(sel==""){
				cout<<"Sql ERROR"<<endl;
				isAtt=false;
			}
			else{
				if(sel=="*"){				
					cout<<"属性名：所有属性"<<endl;
					isAtt=false;
				}
				else{
					cout<<"属性名"<<++i<<"："<<sel<<endl;
				}
			}
		}
		
		sel=readNextWord();
		if(toLower(sel)=="from"){
			//start=last+1;
			sel=readNextWord();
			if(sel==""){
				cout<<"Sql ERROR"<<endl;
			}
			else{
				cout<<"表名："<<sel<<endl;
				sqlSelect.tblName=sel;
				//start=last+1
				sel=readNextWord();
				if(toLower(sel)=="where"){
					bool loop=true;
					int i=0;
					bool isBtw=false;
					while(loop){
						SQLWhere sqlWhere;
						isBtw=false;
						sel=readNextWord();
						if(sel==""){
							cout<<"Sql ERROR"<<endl;
						}
						else{						
							cout<<"查找条件"<<++i<<"："<<sel<<' ';
							sqlWhere.key=sel;
							sel=readNextWord();
							if(sel==""){
								cout<<"Sql ERROR"<<endl;
							}
							else{
								if(sel=="="){
									cout<<"等于 ";
									sqlWhere.SignType=0;
								}
								else if(sel=="<"){
									cout<<"小于 ";
									sqlWhere.SignType=2;
								}
								else if(sel==">"){
									cout<<"大于 ";
									sqlWhere.SignType=3;
								}
								else if(sel=="=<"){
									cout<<"小于等于 ";
									sqlWhere.SignType=4;
								}
								else if(sel==">="){
									cout<<"大于等于 ";
									sqlWhere.SignType=5;
								}
								else if(sel=="<>"){
									cout<<"不等于 ";
									sqlWhere.SignType=1;
								}
								else if(toLower(sel)=="between"){
									isBtw=true;
									SQLWhere sqlWhere2=sqlWhere;
									sel=readNextWord();
									if(sel==""){
										cout<<"Sql ERROR"<<endl;
									}
									else{
										cout<<"在 "<<sel;
										for(unsigned int j=0;j<sel.length();j++){
											sqlWhere.dataType=T_INT;
											sqlWhere2.dataType=T_INT;
												if(sel.at(j)=='.'){
												sqlWhere.dataType=T_FLOAT;
												sqlWhere2.dataType=T_FLOAT;break;
											}
										}
										sqlWhere.SignType=5;
										sqlWhere.value=sel;
										sel=readNextWord();
										if(toLower(sel)=="and"){
											sel=readNextWord();
											if(sel==""){
												cout<<"Sql ERROR"<<endl;
											}
											else{
												cout<<" 和 "<<sel<<" 之间"<<endl;
												sqlWhere2.SignType=4;
												sqlWhere2.value=sel;
											}
										}
										else{
											cout<<"Sql ERROR"<<endl;											
										}
									}
									sqlSelect.wheres.push_back(sqlWhere);
									sqlSelect.wheres.push_back(sqlWhere2);
								}
								else{
									cout<<"Sql ERROR"<<endl;
								}
								if(!isBtw){
									sel=readNextWord();
									if(sel==""){
										cout<<"Sql ERROR"<<endl;
									}
									else{
										char c =sel.at(0);
										if(c=='\'' || c=='\"'){
											sel.assign(sel,1,sel.length()-2);
											sqlWhere.dataType=T_CHAR;
										}
										else{
											for(unsigned int j=0;j<sel.length();j++){
												
													sqlWhere.dataType=T_INT;
													if(sel.at(j)=='.'){
													sqlWhere.dataType=T_FLOAT;
													break;
												}
												
											}
										}
										cout<<sel<<endl;
										sqlWhere.value=sel;
									}
								}
							}
							sel=readNextWord();
							if(sel==""){
								loop=false;
							}
							else{
								if(toLower(sel)=="and"){
									loop=true;
									cout<<"连接词：AND"<<endl;
								}
								else if(toLower(sel)=="or"){
									loop=true;
									cout<<"连接词：OR"<<endl;
								}
								else{
									cout<<"Sql ERROR"<<endl;
									loop=false;
								}
							}
						}
						sqlSelect.wheres.push_back(sqlWhere);
					}
				}
				else if(sel==""){
					//no where, do nothing
				}
				else{
					cout<<"Sql ERROR"<<endl;
				}
			}
		}
		else{
			cout<<"Sql ERROR"<<endl;
		}
		err=  ctlg.dbs[ctlg.curDB].verify_select(sqlSelect);
		return err;
	}

	string Interpreter::dropTable(SQLDropTable &sqlDropTable){
		string err = "";
		string tableName;
		sqlDropTable.SQLType=11;
		//start=last+1;
		tableName=readNextWord();
		if(tableName==""){
			cout<<"Sql ERROR"<<endl;
		}
		else{
			cout<<"指令：DROP TABLE"<<endl;
			cout<<"表名："<<tableName<<endl;
			sqlDropTable.tblName=tableName;
		}
		err=  ctlg.dbs[ctlg.curDB].verify_drop_table(sqlDropTable);

		return err;
	}

	string Interpreter::dropIndex(SQLDropIndex &sqlDropIndex){
		string err = "";
		string indexName;
		sqlDropIndex.SQLType=9;
		//start=last+1;
		indexName=readNextWord();
		if(indexName==""){
			cout<<"Sql ERROR"<<endl;
		}
		else{
			cout<<"指令：DROP INDEX"<<endl;
			cout<<"索引名："<<indexName<<endl;
			sqlDropIndex.indexName=indexName;
		}
		err=  ctlg.dbs[ctlg.curDB].verify_drop_index(sqlDropIndex);
		return err;
	} 

	string Interpreter::dropDatabase(SQLDropDatabase &sqlDropDatabase){
		string err = "";
		string dbName;
		sqlDropDatabase.SQLType=10;
		//start=last+1;
		dbName=readNextWord();
		if(dbName==""){
			cout<<"Sql ERROR"<<endl;
		}
		else{
			cout<<"指令：DROP DATABASE"<<endl;
			cout<<"数据库名："<<dbName<<endl;
			sqlDropDatabase.dbName=dbName;
		}
		err=  ctlg.dbs[ctlg.curDB].verify_drop_database(sqlDropDatabase);
		return err;
	}

	string Interpreter::execfile(SQLExecFile &sqlExecFile){
		string err = "";
		string fileName;
		sqlExecFile.SQLType=8;
		//string s = str+' ';
		//start=last+1;
		fileName=readNextWord();
		if(fileName==""){
			cout<<"Sql ERROR"<<endl;
		}
		else{
			cout<<"指令：EXECFILE"<<endl;
			cout<<"脚本文件名："<<fileName<<endl;
			sqlExecFile.fileName=fileName;
		}
		err=  ctlg.dbs[ctlg.curDB].verify_execfile(sqlExecFile);
		if(err=="")
		{
			ifstream  in;
			in.open(fileName);
			string s,s1;
			while(getline(in,s))
			{
				if(s.find(";")!=s.npos)
				{
					this->reset();
					str = s1+s;
					str.assign(str,0,str.length()-1);
					this->set(str);
					this->tellSqlType();
					switch(sqlType){
					case 0:{
						SQLInsert sqlInsert;

						if(insert(sqlInsert)=="")
						{
							exeSQL::exeInsert(ctlg,sqlInsert,hdl);
						}
						else
						{
							cout<<insert(sqlInsert)<<endl;
						}
						break;
						   }
					case 1:{
						SQLSelect sqlSelect;
						if(select(sqlSelect)=="")
						{
							int size = ctlg.dbs[ctlg.curDB].GetTable(sqlSelect.tblName)->attrNum;
							for(int i =0;i<size;++i)
							{
								if(i!=size-1){
									cout<<ctlg.dbs[ctlg.curDB].GetTable(sqlSelect.tblName)->attrs[i].name<<setw(9);
								}
								else{
									cout<<ctlg.dbs[ctlg.curDB].GetTable(sqlSelect.tblName)->attrs[i].name<<endl;
								}

							}
							cout<<endl;
							cout<<"-----------------------------------"<<endl;
							exeSQL::exeSelect(ctlg,sqlSelect,hdl);
							cout<<"-----------------------------------"<<endl;
						}
						else
						{

							cout<<select(sqlSelect)<<endl;
						}

						break;
						   }
					case 2:{
						
						SQLCreateTable sqlCreateTable;
						if(createTable(sqlCreateTable)=="")
						{
							ctlg.dbs[ctlg.curDB].create_table(sqlCreateTable);
						}
						else
						{

							cout<<createTable(sqlCreateTable)<<endl;
						}

						break;
						   }
					case 3:{
						
						SQLCreateDatabase sqlCreateDatabase;
						if(createDatabase(sqlCreateDatabase)=="")
						{
							ctlg.create_database(sqlCreateDatabase);
						}
						else
						{

							cout<<createDatabase(sqlCreateDatabase)<<endl;
						}

						break;
						   }
					case 4:{

						SQLCreateIndex sqlCreateIndex;
						if(createIndex(sqlCreateIndex)=="")
						{
							ctlg.dbs[ctlg.curDB].create_index(sqlCreateIndex);
						}
						else
						{

							cout<<createIndex(sqlCreateIndex)<<endl;
						}

						break;
						   }
					case 6:{
						
						SQLDelete sqlDelete;
						if(deletefrom(sqlDelete)=="")
						{
							exeSQL::exeDelete(ctlg,sqlDelete,hdl);
						}
						else
						{

							cout<<deletefrom(sqlDelete)<<endl;
						}


						
						break;
						   }
					case 7:{
		
						SQLUse sqlUse;
						if(useDB(sqlUse)=="")
						{
							ctlg.use_database(sqlUse);
						}
						else
						{

							cout<<useDB(sqlUse)<<endl;
						}

						break;
						   }

					case 9:{
						
						SQLDropIndex sqlDropIndex;
						if(dropIndex(sqlDropIndex)=="")
						{
							ctlg.dbs[ctlg.curDB].drop_index(sqlDropIndex);
						}
						else
						{

							cout<<dropIndex(sqlDropIndex)<<endl;
						}

						break;
						   }
					case 10:{
						
						SQLDropDatabase sqlDropDatabase;
						if(dropDatabase(sqlDropDatabase)=="")
						{
							ctlg.drop_database(sqlDropDatabase);
						}
						else
						{

							cout<<dropDatabase(sqlDropDatabase)<<endl;
						}

						break;
							}
					case 11:{
					
						SQLDropTable sqlDropTable;
						if(dropTable(sqlDropTable)=="")
						{
							ctlg.dbs[ctlg.curDB].drop_table(sqlDropTable);
						}
						else
						{

							cout<<dropTable(sqlDropTable)<<endl;
						}

						break;
							}

					}
					s1.clear();

				}

				else
				{
					s1=s1+s+"\n";
				}
			}

		}
		return err;
	}

	string Interpreter::deletefrom(SQLDelete &sqlDelete){
		string err = "";
		string del;
		//start=last+1;

		sqlDelete.SQLType=6;
		del=readNextWord();
		
		if(toLower(del)=="from"){
			//start=last+1;
			del=readNextWord();
			if(del==""){
				cout<<"Sql ERROR"<<endl;
			}
			else{
				cout<<"指令：DELETE"<<endl;
				cout<<"表名："<<del<<endl;
				sqlDelete.tblName=del;
				//start=last+1
				del=readNextWord();
				if(toLower(del)=="where"){
					bool loop=true;
					int i=0;
					bool isBtw=false;
					while(loop){
						SQLWhere sqlWhere;
						isBtw=false;
						del=readNextWord();
						if(del==""){
							cout<<"Sql ERROR"<<endl;
						}
						else{						
							cout<<"删除条件"<<++i<<"："<<del<<' ';
							sqlWhere.key=del;
							del=readNextWord();
							if(del==""){
								cout<<"Sql ERROR"<<endl;
							}
							else{
								if(del=="="){
									cout<<"等于 ";
									sqlWhere.SignType=0;
								}
								else if(del=="<"){
									cout<<"小于 ";
									sqlWhere.SignType=2;
								}
								else if(del==">"){
									cout<<"大于 ";
									sqlWhere.SignType=3;
								}
								else if(del=="=<"){//这是是=<
									cout<<"小于等于 ";
									sqlWhere.SignType=4;
								}
								else if(del==">="){
									cout<<"大于等于 ";
									sqlWhere.SignType=5;
								}
								else if(del=="<>"){
									cout<<"不等于 ";
									sqlWhere.SignType=1;
								}
								else if(toLower(del)=="between"){
									SQLWhere sqlWhere2=sqlWhere;
									del=readNextWord();
									if(del==""){
										cout<<"Sql ERROR"<<endl;
									}
									else{
										cout<<"在 "<<del;
										for(unsigned int j=0;j<del.length();j++){
											if(del.at(j)=='.'){
												sqlWhere.dataType=T_FLOAT;
												sqlWhere2.dataType=T_FLOAT;
											}
											else{
												sqlWhere.dataType=T_INT;
												sqlWhere2.dataType=T_INT;
											}
										}
										sqlWhere.SignType=5;
										sqlWhere.value=del;
										del=readNextWord();
										if(toLower(del)=="and"){
											del=readNextWord();
											if(del==""){
												cout<<"Sql ERROR"<<endl;
											}
											else{
												cout<<" 和 "<<del<<" 之间"<<endl;
												sqlWhere2.SignType=4;
												sqlWhere2.value=del;
												isBtw=true;
											}
										}
										else{
											cout<<"Sql ERROR"<<endl;											
										}
									}
									sqlDelete.wheres.push_back(sqlWhere);
									sqlDelete.wheres.push_back(sqlWhere2);
								}
								else{
									cout<<"Sql ERROR"<<endl;
								}
								if(!isBtw){
									del=readNextWord();
									if(del==""){
										cout<<"Sql ERROR"<<endl;
									}
									else{
										char c =del.at(0);
										if(c=='\'' || c=='\"'){
											del.assign(del,1,del.length()-2);
											sqlWhere.dataType=T_CHAR;
										}
										else{
											for(unsigned int j=0;j<del.length();j++){
												sqlWhere.dataType=T_INT;
												if(del.at(j)=='.'){
													sqlWhere.dataType=T_FLOAT;break;
												}
											}											
										}
										cout<<del<<endl;
										sqlWhere.value=del;
									}
								}
							}
							del=readNextWord();
							if(del==""){
								loop=false;
							}
							else{
								if(toLower(del)=="and"){
									loop=true;
									cout<<"连接词：AND"<<endl;
								}
								else if(toLower(del)=="or"){
									loop=true;
									cout<<"连接词：OR"<<endl;
								}
								else{
									cout<<"Sql ERROR"<<endl;
									loop=false;
								}
							}
						}
						sqlDelete.wheres.push_back(sqlWhere);
					}
				}
				else if(del==""){
					//no where, do nothing
				}
				else{
					cout<<"Sql ERROR"<<endl;
				}
			}
		}
		else{
			cout<<"Sql ERROR"<<endl;
		}
		err=  ctlg.dbs[ctlg.curDB].verify_delete(sqlDelete);
		return err;
	}

	SQLQuit Interpreter::quitSql(){
		cout<<"欢迎使用MiniSQL ...再见"<<endl;
		system("pause");
		hdl.quitProg(ctlg.dbs[ctlg.curDB].name);
		exit(0);
	}

	string Interpreter::helpSql(SQLHelp &sqlHelp){
		string err = "";
		
		sqlHelp.SQLType=5;
		cout<<"=================================="<<endl;
		cout<<"目前已实现语句："<<endl;
		cout<<"-- select（单条件）"<<endl;
		cout<<"-- delete（单条件）"<<endl;
		cout<<"-- drop"<<endl;
		cout<<"-- execfile"<<endl;
		cout<<"输入SQL语句时请严格按照语法规范，";
		cout<<"符号与单词之间用空格隔开。"<<endl;
		cout<<"=================================="<<endl;
		err=  ctlg.dbs[ctlg.curDB].verify_help(sqlHelp);
		return err;
	}

	string Interpreter::useDB(SQLUse &sqlUse){
		string err = "";
		string dbName;
		sqlUse.SQLType=7;
		//string s = str+' ';
		//start=last+1;
		dbName=readNextWord();
		if(dbName==""){
			cout<<"Sql ERROR"<<endl;
		}
		else{
			cout<<"指令：USE"<<endl;
			cout<<"数据库名："<<dbName<<endl;
			sqlUse.dbName=dbName;
		}
		err=  ctlg.dbs[ctlg.curDB].verify_use(sqlUse);
		return err;
	}

	string Interpreter::createIndex(SQLCreateIndex &sqlCreateIndex){
		string err = "";
		string ciName;
		//start=last+1;
		sqlCreateIndex.SQLType=4;
		ciName=readNextWord();
		if(ciName==""){
			cout<<"Sql ERROR"<<endl;
		}
		else{
			cout<<"指令：CREATE INDEX"<<endl;
			cout<<"索引名："<<ciName<<endl;
			sqlCreateIndex.indexName=ciName;

			ciName=readNextWord();
			if(toLower(ciName)=="on"){
				//start=last+1;
				ciName=readNextWord();
				if(ciName==""){
					cout<<"Sql ERROR"<<endl;
				}
				else{
					cout<<"表名："<<ciName<<endl;
					sqlCreateIndex.tblName=ciName;
					//start=last+1
					ciName=readNextWord();
					if(ciName=="("){
						ciName=readNextWord();
						if(ciName==""){
							cout<<"Sql ERROR"<<endl;
						}
						else{
							cout<<"列名："<<ciName<<endl;
							sqlCreateIndex.columnName=ciName;
							ciName=readNextWord();
							if(ciName!=")"){
								cout<<"Sql ERROR"<<endl;
							}
						}
					}
				}
			}
			else{
				cout<<"Sql ERROR"<<endl;
			}
		}
		err=  ctlg.dbs[ctlg.curDB].create_index(sqlCreateIndex);
		return err;
	}

	string Interpreter::createDatabase(SQLCreateDatabase &sqlCreateDatabase){
		string err = "";
		string dbName;
		//start=last+1;
		sqlCreateDatabase.SQLType=3;
		dbName=readNextWord();
		if(dbName==""){
			cout<<"Sql ERROR"<<endl;
		}
		else{
			cout<<"指令：CREATE DATABASE"<<endl;
			cout<<"数据库名："<<dbName<<endl;
			sqlCreateDatabase.dbName=dbName;
		}
		//err=  ctlg.dbs[ctlg.curDB].verify_create_database(sqlCreateDatabase);
		return err;
	}

	string Interpreter::createTable(SQLCreateTable &sqlCreateTable){
		string err = "";
		string crt;
		int i=0;
		attribute att[32];
		//start=last+1;
		sqlCreateTable.SQLType=2;
		
		crt=readNextWord();
		if(crt==""){
			cout<<"Sql ERROR"<<endl;
		}
		else{
			cout<<"指令：CREATE TABLE"<<endl;
			cout<<"表名："<<crt<<endl;
			sqlCreateTable.tblName=crt;
			crt=readNextWord();
			if(crt=="("){
				isAtt=true;
				while(isAtt){
					isAtt=false;
					crt=readNextWord();
					if(crt==""){
						cout<<"Sql ERROR"<<endl;
						isAtt=false;
					}
					else if(toLower(crt)=="primary"){
						crt=readNextWord();
						if(toLower(crt)=="key"){
							crt=readNextWord();
							if(crt=="("){
								crt=readNextWord();
								if(crt==""){
									cout<<"Sql ERROR"<<endl;
								}
								else{
									cout<<"主键名："<<crt<<endl;

									for(int j=0;j<i;j++){
										if(att[j].name==crt){
											att[j].attrType=1;
										}
									}
								}
								crt=readNextWord();
								if(crt!=")"){
									cout<<"Sql ERROR"<<endl;
								}
							}
						}
						else{
							cout<<"Sql ERROR"<<endl;
						}
						isAtt=false;
					}
					else{
						cout<<"列名"<<++i<<"："<<crt;
						att[i-1].name=crt;

						isAtt=true;
						crt=readNextWord();
						string ss;
						ss.assign(crt,0,7);
						if(crt==""){
							cout<<"Sql ERROR"<<endl;
							isAtt=false;
						}
						else if(toLower(ss)=="numeric"){
							isAtt=true;
							crt=crt+','+readNextWord();
							cout<<" 类型"<<i<<"：";
							att[i-1]=tellDataType(crt,att[i-1]);
						}
						else{
							cout<<" 类型"<<i<<"：";
							att[i-1]=tellDataType(crt,att[i-1]);
						}						
					}					
				}
				for(int j=0;j<i;j++){
					sqlCreateTable.attrs.push_back(att[j]);
				}
				crt=readNextWord();
				if(crt!=")"){
					cout<<"Sql ERROR"<<endl;
				}
			}
			else{
				cout<<"Sql ERROR"<<endl;
			}
		}
		err=  ctlg.dbs[ctlg.curDB].verify_create_table(sqlCreateTable);
		return err;
	}

	string Interpreter::insert(SQLInsert &sqlInsert){
		string err= "";
		string ins;
		int i=0;
		//start=last+1;

		sqlInsert.SQLType=0;
		ins=readNextWord();
		if(toLower(ins)=="into"){
			//start=last+1;
			ins=readNextWord();
			if(ins==""){
				cout<<"Sql ERROR"<<endl;
			}
			else{
				cout<<"指令：INSERT"<<endl;
				cout<<"表名："<<ins<<endl;
				sqlInsert.tblName=ins;
				//sqlinsert.values.push_back();
				ins=readNextWord();
				if(toLower(ins)=="values"){
					ins=readNextWord();
					if(ins=="("){
						isAtt=true;

						while(isAtt){
							SQLValue sqlValue;
							ins=readNextWord();
							if(ins==""){
								cout<<"Sql ERROR"<<endl;
								isAtt=false;
							}
							else{
								cout<<"属性值"<<++i<<"：";
								char c =ins.at(0);
								if(c=='\'' || c=='\"'){
									ins.assign(ins,1,ins.length()-2);
									;
									sqlValue.dataType=T_CHAR;
								}
								else{
									for(unsigned int j=0;j<ins.length();j++){
										sqlValue.dataType=T_INT;
										if(ins.at(j)=='.'){
											sqlValue.dataType=T_FLOAT;break;
										}
									}									
								}
								cout<<ins<<endl;
								sqlValue.value=ins;																
							}
							//sqlInsert.SQLType=1;
							sqlInsert.values.push_back(sqlValue);
						}
						ins=readNextWord();
						if(ins!=")"){
							cout<<"Sql ERROR"<<endl;
						}
					}
					else{
						cout<<"Sql ERROR"<<endl;
					}
				}
				else{
					cout<<"Sql ERROR"<<endl;
				}
			}
		}
		else{
			cout<<"Sql ERROR"<<endl;
		}
		err=  ctlg.dbs[ctlg.curDB].verify_insert(sqlInsert);
		return err;
	}
