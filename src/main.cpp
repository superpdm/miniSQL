#include "Record.h"
#include "catalog.h"
#include "Interpreter.h"
using namespace std;

char * AllBuffer[10000];
ostream & operator<<(ostream& out,const TKEY& k) 
{ 
	switch(k.keyType)
	{
	case T_INT:
		printf("%d ",*((int*)k.key));break;
	case T_FLOAT:
		printf("%.2lf ",*((float*)k.key));break;
	case T_CHAR:
		printf("%s ",k.key);break;
	}
	return out; 
} 

catalog ctlg;

Handle hdl;
	
// 	void where()
// 	{
// 		SQLSelect sqlSelect;
// 		sqlSelect.tblName="newtable";
// 
// 		SQLWhere sqlwhere;
// 		sqlwhere.dataType=T_FLOAT;
// 		sqlwhere.key="colA";
// 		sqlwhere.SignType=SIGN_GE;
// 		sqlwhere.value="4.5";
// 		sqlSelect.wheres.push_back(sqlwhere);
// 
// 		sqlwhere.dataType=T_INT;
// 		sqlwhere.key="colB";
// 		sqlwhere.SignType=SIGN_LE;
// 		sqlwhere.value="60";
// 		sqlSelect.wheres.push_back(sqlwhere);
// 
// 		sqlwhere.dataType=T_INT;
// 		sqlwhere.key="colB";
// 		sqlwhere.SignType=SIGN_NE;
// 		sqlwhere.value="50";
// 		sqlSelect.wheres.push_back(sqlwhere);
// 
// 		exeSQL::exeSelect(ctlg,sqlSelect,hdl);
// 	}
// 	void select()
// 	{
// 		SQLSelect sqlSelect;
// 		sqlSelect.tblName="newtable";
// 		exeSQL::exeSelect(ctlg,sqlSelect,hdl);
// 	}
void insert()
{
	SQLInsert sqlinsert;
	sqlinsert.tblName="tt";

	char s[10];
	for(int i=0;i<10000;i++)
	{
		sqlinsert.values.clear();


		
		SQLValue value;

		value.dataType=T_INT;
		sprintf(s,"%d",i);
		value.value=s;
		sqlinsert.values.push_back(value);

		value.dataType=T_CHAR;
		value.value="stfds";
		sqlinsert.values.push_back(value);
		
		value.dataType=T_FLOAT;
		sprintf(s,"%lf",i*0.01);
		value.value=s;
		sqlinsert.values.push_back(value);

		sqlinsert.tblName="tt";

		exeSQL::exeInsert(ctlg,sqlinsert,hdl);
	}

}
// 	void create()
// 	{
// 		SQLCreateDatabase createdb;
// 		createdb.dbName="ab";
// 		ctlg.create_database(createdb);
// 		ctlg.curDB=0;
// 
// 		database &curDB=ctlg.dbs[ctlg.curDB];
// 		SQLCreateTable sqlcreatetable;
// 		attribute a;
// 		a.name="colA";
// 		a.attrType=a.dataType=T_FLOAT;
// 		a.len=4;
// 		sqlcreatetable.attrs.push_back(a);
// 		a.name="colB";
// 		a.dataType=T_INT;
// 		sqlcreatetable.attrs.push_back(a);
// 		sqlcreatetable.SQLType=CREATE_TABLE;
// 		sqlcreatetable.tblName="newtable";
// 
// 		curDB.create_table(sqlcreatetable);
// 	}
// 	void deleteR()
// 	{
// 		SQLDelete sqldelete;
// 		sqldelete.tblName="newtable";
// 
// 		SQLWhere sqlwhere;
// 		sqlwhere.dataType=T_FLOAT;
// 		sqlwhere.key="colA";
// 		sqlwhere.SignType=SIGN_GE;
// 		sqlwhere.value="2.0";
// 		sqldelete.wheres.push_back(sqlwhere);
// 
// 		sqlwhere.dataType=T_INT;
// 		sqlwhere.key="colB";
// 		sqlwhere.SignType=SIGN_LE;
// 		sqlwhere.value="30";
// 		sqldelete.wheres.push_back(sqlwhere);
// 
// 		exeSQL::exeDelete(ctlg,sqldelete,hdl);
// 	}
int main()
{
	
	ctlg.curDB=0;
	string sql;
	Interpreter Itp;
//	insert();
	while(1){
		sql.clear();
		Itp.reset();
		cout<<"SQL --> ";
		getline(cin,sql,';');
		Itp.set(sql);
		cin.ignore();
		Itp.tellSqlType();
		Itp.controlor();
	}
/*	select();*/
	//hdl.quitProg("ab");
	system("pause");
	return 0;
}