//定义各种要用到的常数///////////
#ifndef _CONSTANT_H
#define  _CONSTANT_H
////三种数据类型的定义//////////
#define		T_INT	0
#define		T_FLOAT	1
#define		T_CHAR	2
/////十种SQL语句类型的定义///////
#define		INSERT		0
#define		SELECT			1
#define		CREATE_TABLE	2
#define		CREATE_DATABASE	3
#define		CREATE_INDEX	4
#define		HELP			5
#define		DELETE			6
#define		USE				7
#define		EXECFILE		8
#define		DROP_INDEX		9
#define		DROP_DATABASE	10
#define		DROP_TABLE		11
#define		QUIT			12

//////比较符号///////////
//=	<>	<	>	<=	>=
#define SIGN_EQ 0
#define SIGN_NE 1
#define SIGN_LT 2
#define SIGN_GT 3
#define SIGN_LE 4
#define SIGN_GE 5
////////////filetype/////////
#define FILE_RECORD 0
#define FILE_INDEX  1
#endif 