# miniSQL
a mini-SQL database, supporting operations like create(drop) database and table, create(drop) index, select(select with index,insert,delete) records.
# 使用方法
将src源文件导入工程中编译即可。注意：本项目使用到了BOOST库作序列化，所以必须要有BOOST库才能成功编译。
# 目标
设计并实现一个精简型单用户SQL引擎(DBMS)MiniSQL，允许用户通过字符界面输入SQL语句实现表的建立/删除；索引的建立/删除以及表记录的插入/删除/查找。
#需求概述
数据类型

只要求支持三种基本数据类型：int，char(n)，float，其中char(n)满足 1 <= n <= 255 。

表定义

一个表最多可以定义32个属性，各属性可以指定是否为unique；支持单属性的主键定义。

索引的建立和删除

对于表的主属性自动建立B+树索引，对于声明为unique的属性可以通过SQL语句由用户指定建立/删除B+树索引（因此，所有的B+树索引都是单属性单值的）。

查找记录

可以通过指定用and连接的多个条件进行查询，支持等值查询和区间查询。

插入和删除记录

支持每次一条记录的插入操作；支持每次一条或多条记录的删除操作。

#SQL语句说明
MiniSQL支持标准的SQL语句格式，每一条SQL语句以分号结尾，一条SQL语句可写在一行或多行。为简化编程，要求所有的关键字都为小写。在以下语句的语法说明中，用黑体显示的部分表示语句中的原始字符串，如create就严格的表示字符串“create”，否则含有特殊的含义，如 表名 并不是表示字符串 “表名”，而是表示表的名称。
创建表语句

该语句的语法如下：

create table 表名 (
	列名 类型 ,
	列名 类型 ,
	
	列名 类型 ,
	primary key ( 列名 )
);

其中类型的说明见第二节“功能需求”。

若该语句执行成功，则输出执行成功信息；若失败，必须告诉用户失败的原因。

示例语句：

create table student (
		sno char(8),
		sname char(16) unique,
		sage int,
		sgender char (1),
		primary key ( sno )
);

删除表语句

该语句的语法如下：

drop table 表名 ;

若该语句执行成功，则输出执行成功信息；若失败，必须告诉用户失败的原因。

示例语句：

drop table student;

创建索引语句

该语句的语法如下：

create index 索引名 on 表名 ( 列名 );

若该语句执行成功，则输出执行成功信息；若失败，必须告诉用户失败的原因。

示例语句：

create index stunameidx on student ( sname );

删除索引语句

该语句的语法如下：

drop index 索引名 ;

若该语句执行成功，则输出执行成功信息；若失败，必须告诉用户失败的原因。

示例语句：

drop index stunameidx;

选择语句

该语句的语法如下：

select * from 表名 ;

或：
select * from 表名 where 条件 ;

其中“条件”具有以下格式：列 op 值 and 列 op 值 … and 列 op 值。

op是算术比较符：=	<>	<	>	<=	>=

若该语句执行成功且查询结果不为空，则按行输出查询结果，第一行为属性名，其余每一行表示一条记录；若查询结果为空，则输出信息告诉用户查询结果为空；若失败，必须告诉用户失败的原因。
示例语句：

select * from student;
select * from student where sno = ‘88888888’;
select * from student where sage > 20 and sgender = ‘F’;

插入记录语句

该语句的语法如下：

insert into 表名 values ( 值1 , 值2 , … , 值n );

若该语句执行成功，则输出执行成功信息；若失败，必须告诉用户失败的原因。

示例语句：

insert into student values (‘12345678’,’wy’,22,’M’);

删除记录语句

该语句的语法如下：

delete from 表名 ;

或：

delete from 表名 where 条件 ;

若该语句执行成功，则输出执行成功信息，其中包括删除的记录数；若失败，必须告诉用户失败的原因。

退出MiniSQL系统语句

该语句的语法如下：

quit;

执行SQL脚本文件语句

该语句的语法如下：

execfile 文件名 ;

SQL脚本文件中可以包含任意多条上述8种SQL语句，MiniSQL系统读入该文件，然后按序依次逐条执行脚本中的SQL语句。


在设计中，除了基本的需求之外，我们新增了四条语句：

创建数据库语句

CREATE DATABASE 数据库名；

删除数据库语句

DROP DATABASE 数据库名；

使用数据库语句

USE 数据库名；

显示帮助信息语句

HELP;
