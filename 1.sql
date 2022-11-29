select * from test;
select * from test;
select * from test;
select * from studetinfo;

select * from test;
select * from studetinfo;
insert into test value ("小饼", 21);


#命名规则
#数据库不区分大小写，一般都是用小写
#表名：t_、、、
#列名：f_、、、


#创建数据表
#1.指定默认使用的库：use 库名；
use test;
#创建数据表：create table 表名（列名1 类型 约束 ，列名2 类型 约束，....）；
#建表约束：
#1.主键约束primaty key :值唯一，不允许为空，主键列是唯一列，其他列不允许是主建
#2.唯一unique:之不重复，可以为空，可以多列唯一列
#3.默认值default
#4.非空not null
#5.自增 auto_increment
#6.外键约束foreign key
#练习：创建一个学生信息表，主键列学号，唯一列学生姓名，性别 枚举值，年龄 默认值18
create table studetinfo(
number int primary key,
name varchar(45) unique,
sex enum ('男','女'),
age int default 18
);
insert into studetinfo value (2019036,"亚瑟王", '女',21);
#修改表:alter table表名...;
#增加列: alter table表名add column列名类型约束;#例子:增加一列学校类型int无约束条件
alter table studentinfo add column school int;
#修改列属性: alter table表名modify 列名类型约束;#例子:修改学校列类型varchar(45)
alter table studentinfo modify school varchar(45);
#删除列: alter table表名drop 列名;
#例子:学校列
alter table studentinfo drop school;
#册除表: drop table表名;
drop table studentinfo;
create table studetinfo(
number int primary key,
name varchar(45) unique,
sex enum ('男','女'),
age int default 18
);
#查找数据: select 列名1，列名2 from表名;(*是查所有列)select *from student;
select s, sname from student;
#给查到的列起别名: select 列名1别名，列名2 from表名;select s 学号,sname from student;
#插入: insert into 表名values(列的值);—必须给所有添加值
#给指定列添加值: insert into表名(列名1，列名2) values (列1的值，列1的值);
insert into studetinfo values (1,'张三','男',18);
insert into studetinfo values (2,'saber','女',18);
insert into studetinfo values (3,'亚瑟王','女',18);
insert into studetinfo values (4,'征服王','男',18);
insert into studetinfo values (5,'红A','男',18);
select *from studetinfo where sname like "王";
insert into studetinfo (number,name) values (3,'李四');
#修改数据: update表名set 列名=值;
update studentinfo set age = 19;
#册除数据: delete from表名where条件;
delete from studetinfo where name = '李四';



#条件查询:where条件;#相等:列名=值;
select *from student where Ssex ='男';
#不相等:列名!=值;(只在mysql支持)列名<>值;
select *from sc where score != 80;
select *from sc where score <>80;
#或者:条件1 or条件2;
select* from student where s = '02' or s = '04';
#并且:条件1 and条件2;
select* from sc where c = '01' and score = '80';
#介于之间:between条件1 and条件2;(闭合区间)
select *from sc where score between '60' and '80 ' ;
#范围内: in(范围);
select *from student where s in ( '01', '03','05','07');
#不在范围内: not in(范围);
select * from student where s not in ( '01','03', '05', '07');

#模糊查询: where列名like模糊表达式;
#1%:匹配任意0到多个字符，“张%”，可以查出来张、张三、张三三、张xxxxx#_:匹配任意一个字符，“张_”，可以查出张三、张磊
select*from student where sname like "赵%";
select *from student where sname like "%_";

#聚合函数
#1、count查个数
select count(*) from student;
select count(s) from student;
select count(*) from studentinfo;
select count( sex) from studentinfo;
#2、sum求和
select sum(score) from sc where c = '01';
#3、max最大值
select max( score) from sc where c = '01';
#4、min最小值
select min( score) from sc where c = '01';
#5、avg平均值
select avg(score) from sc where s = '01';

#分组查询:group by 列名;#每个的同学的平均分
select s, avg(score) from sc group by s;
select count(*)from student group by Ssex;
#分组之后加条件:group by 列名having条件;
select s, avg(score) from sc group by s having avg(score)> 60;
#分组排序: order by列名 asc(升序)/desc(降序);(不写排序规则，默认是升序排列)
select s, avg(score) from sc group by s order by avg(score) asc;
select s,avg(score) from sc group by s order by avg(score) desc;


#联合查询:查询结果1 union all查询结果2;#查询男生和女生分别有多少人
#1、查询男生有多少人
select count(*) from student where Ssex ='男';
#2、查询女生有多少人
select count(*)from student where Ssex ='女';
#3、结果联合起来
select count(*) from student where Ssex ='男'
union all
select count(*) from student where Ssex = '女';

#________________________________________________
#1、查询每门课程被选修的学生数//
select c ,count(s) from sc group by c;
#2、检索至少选修两门课程的学生学号//
select s ,count(c) from sc group by s having count(c)>=2;
#3、求这些学号对应的个人信息//
select s ,Sname,Sage,Ssstudentex from student;
#4、查询选修了全部课程的学生信息//
select s,count(c) from sc group by s having count(c)=3;
#5、查询没有学全所有课程的同学的信息//
select s,count(c) from sc group by s having count(c)<3;
#6、查询学过"张三"老师授课的同学的信息//
select s from sc where c='01' group by s;
#7、查询没学过"张三"老师授课的同学的信息c!='01'也行***
select * from student where s not in(select s from sc where c='01' group by s);
#8、查询每个同学01课程的成绩，包括个人信息//
select s ,score from sc where c='01';
#9、查询01课程分数>02课程分数的个人信息

select a.s from (select s ,score from sc where c ='01')as a,(select s ,score from sc where c ='02')as b where a.s=b.s and a.score>b.score;
#10、查询同时存在"01"课程和"02"课程的学生信息//
select s from sc where c ='01'and'02'  group by s;

#1、查询每门课程被选修的学生数
select c, count(* )from sc group by c;
#2、检索至少选修两门课程的学生号
#1>查询每个学生选修的课程数
select s, count(*)from sc group by s;
#2>至少选修两门课程
select s, count(*) num from sc group by s having num >= 2;
#3、求这些学号对应的个人信息
select *from student where s in (
select s from sc group by s having count(*) >= 2);
#4、查询选修了全部课程的学生信息
#1>每个学生选修了几门课
select s, count(*)from sc group by s;
#2>一共有多少门课
select count(*) from course;
#3>选修了全部课程的学生学号
select s from sc group by s having count(*) = (select count(*) from course);
#4>查询选修了全部课程的学生信息
select * from student where s in (
select s from sc group by s having count(*) = (select count(*) from course));
#5、查询没有学全所有课程的学生信息
select *from student where s not in (
select s from sc group by s having count(*) = (select count(*) from course));
#6、查询学过"张三”老师课程的学生信息#1>查询张三老师的id
select T from teacher where Tname ='张三';
#2>根据张三老师的id查询课程的id(用in因为张三老师可能不只教授一门课程)
select c from course where T in (select T from teacher where Tname = '张三');
#3>根据课程的id查询选修这门课程的学生编号
select s from sc where c in (
select C from course where T in (
select T from teacher where Tname = '张三'));
#4>根据学生编号查询学生信息
select *from student where s in (select s from sc where c in (
select c from course where T in (
select T from teacher where Tname ='张三')));
#7、查询没学过“f张三”老师课程的学生信息
select *from student where s not in (select s from sc where c in (
select c from course where T in (
select T from teacher where Tname = '张三')));
#8、查询每个学生01课程的成绩,包括学生信息
#1>查询01课程的成绩
select score from sc where C = '01';
#2>查询选修e1课程的学生id
select s from sc where c = '01' ;
#3>根据学生id查学生信息
select * from student where s in (select s from sc where c = '01');
#4>把第一步骤查出的列拼进结果
select *,
(select score from sc where c = '01' and sc.s = student.s)01课程的成绩
from student where s in (select s from sc where c = '01 ');
#9、查询81课程分数>02课程分数的学生信息
#1>查询每个同学e1课程分数和e2课程分数
select *,
(select score from sc where C = '01' and sc.s = student.S) 01score,
(select score from sc where C = '02' and sc.s = student.S) 02score from student;
#2>01课程分数>02课程分数
select *,
(select score from sc where C = '01' and sc.s = student.s) 01score,
(select score from sc where c = '02' and sc.s = student.s) 02score from student having 01score > 02score;
#1日、查询|同时存在01课程和e2课程的学生信息
select *,
(select score from sc where C = '01' and sc.s = student.S) 01score,
(select score from sc where c = '02' and sc.s = student.s) 02score from student having 01score is not null and 02score is not null;

#------------------------------------------------------------------------------------------------------------------------------------------
#1.查询高于60分的学生信息
select * from student st inner join sc on st.s=sc.S where score>60;

select student.*, score from student inner join sc on sc.s = student.s where score > 60;

#2.查询每个学生的总成绩大于200
select * ,sum(score) num from student st  inner join sc on st.s=sc.S group by st.s  having num>200;

select student.*, sum( score) total from student inner join sc on sc.s = student.s
group by student.s having total > 200;

#3.查询每个学生的总成绩和学生信息
select * ,sum(score) num from student st  inner join sc on st.s=sc.S group by st.s;

select student.*,sum( score)总成绩 from student
inner join sc on sc.s = student.s group by student.S;

#4.查询总成绩最低alter
select * ,sum(score) num from student st  inner join sc on st.s=sc.S group by st.s order by num asc limit 0, 1;

select student.*, sum( score) total from student 
inner join sc on sc.s = student.s
group by student.s order by total asc limit 0,1;

#5.查询学过张三老师的学生信息
select * from student st inner join sc on st.s=sc.S inner join course on sc.C=course.C inner join teacher on course.T =teacher.T where teacher.Tname='张三';
select st.*, score,cname, tname from student st inner join sc on sc.s = st.s
inner join course on course.c = sc.c
inner join teacher on course.T = teacher.T where Tname = '张三';

#去重:select distinct列名from表名;#1、去重选课表的学生列
select distinct s from sc;
#2、去重选课表的学生列和课程列
select distinct s, c from sc;


#视图:
#为了简化复杂的sql查询语句提出的概念。
#视图是一个表或者多个表导出的虚拟表，不是真实存在，不需要满足范式的要求。
#视图的优缺点:
#优点: sql语句在网络中传输，使用视图可以减少流量，更安全
#缺点:使用视图并不会提高效率，还是执行的原来的sql语句
#1、创建视图: create view视图名as ( sql语句);
create view myview as(
select *,
(select score from sc where c = '01' and sc.s = student.S) 01score,
( select score from sc where c = '02' and sc.s = student.s) 02score 
from student
);
#2、使用视图:select * from视图名;
select *from myview where 01score >02score;
#3、删除视图: drop view视图名;
drop view myview;


#函数
#创建函数
#语法:
#delimiter //   —声明//作为语句结束标志
#create function函数名(参数1类型，参数2类型，...)          —创建函数
#returns返回值类型   —声明返回值类型
#begin               —声明函数体开始
#       函数内容
#end //              —声明函数体结束
#delimiter ;        —重新声明;作为语句结束标志

#例子:定义一个求和函数
drop function if exists mysum;
delimiter //
create function mysum( a int, b int)
returns int
begin
	#声明临时变量
	declare c int default 0;
	#设置数值
	set c = a + b + 100;
return c;
end //
delimiter ;
#调用函数: select函数名(参数列表);
select mysum(2，4);
#册除函数:
#直接删除函数: drop function函数名;
#如果函数存在，再删除: drop function if exists 函数名;
drop function if exists mysum;

#变量
#局部变量

#会话变量:当前会话范围内都有效
set @a = 10;

#系统变量（全局变量):程序安装的时候写在环境变量#只有root用户才有权限操作
#查看所有的全局变量
show global variables;
select @@auto_increment__offset;

#选择
#1、if选择语法:
#if (a > b) then执行语句;
#可以是多条执行语句
#elseif ( a = b) then执行语句;
#else执行语句;
#end if;
#例子:定义一个函数，判断传入的参数是否大于0
drop function if exists myfun;
delimiter //
create function myfun(a int)
returns varchar(45)
begin
	#定义结果返回值
	declare result varchar(45) default ' ';
    if (a > 0) then
		set result = '大于0';
		set result = result +'kkkkkk';
        set result = result + 'sjka';
        elseif (a = 0) then set result ='等于0';
	else set result = '小于0';
	end if;
	return result;
 end //
delimiter ;
select myfun(10);


#2、 case选择语法:
#1>
#case变量when值1 then执行语句;
#        when值2 then执行语句;
#        when值3 then执行语句;
#end case;
#例子:定义一个函数，判断传入的参数是否等于0
drop function if exists myfun;
delimiter //
create function myfun(a int)
returns varchar(45)
begin
#定义结果返回值
	declare result varchar(45) default ' ';
	case a 
		when 0 then set result ='等于0';
		when 10 then set result = '等于10' ;
	end case;
	return result;
end //
delimiter ;
select myfun(10);

#2>
#case    when条件1 then执行语句;
#        when条件2 then执行语句;
#        when条件3 then执行语句;ilend case;
#例子:定义一个函数，判断传入的参数是否等于0
drop function if exists myfun;
delimiter //
create function myfun(a int)returns varchar(45)
begin
	#定义结果返回值
	declare result varchar(45) default '';
	case when a = 0 then set result ='等于o';
		 when a > 0 then set result = '大于0';
		 when a < 0 then set result = '小于0';
	end case;
	return result;
end //
delimiter ;
select myfun(10);


#选择
#1、if选择语法:
#if (a > b) then执行语句;
#可以是多条执行语句
#elseif (a = b) then执行语句;
#telse执行语句;
#end if;
#例子:定义一个函数，判断传入的参数是否大于0
drop function if exists myfun;
delimiter //
create function myfun(a int)returns varchar(45)
begin
	#定义结果返回值
	declare result varchar(45) default '';
    if (a > 0) then
		set result = '大于0';
		set result = concat(result, '，第二次大于0');
		set result = concat(result, ', shdiahsdjka ' );
	elseif (a = 0) then set result ='等于0';
	else set result = '小于0';
	end if;
	return result;
end //
delimiter ;
select myfun(10);


#循环语法:
#while条件
#do
#     多条执行语句;
#end while;
#例子:计算1+2+3...+n
drop function if exists mysum;
delimiter //
create function mysum(n int)
returns int
begin
	#定义结果返回值
	declare result int default 0;
    declare i int default 0;
	while i < n
	do
		set result = result + i;
        set i = i + 1;
	end while;
return result;
end //
delimiter ;
select mysum(10);


#存储过程:为了完成特定功能的一组sql语句集，存储在数据库中，一次编译后永久有效。
#存储过程没有返回值，如果需要返回，可以在参数里面声明OUT|INOUT|IN
#存储过程的优点:
#1、减少网络流量，减少与客户机的交互，更安全
#2、增强了代码的重用性和共享性
#3、使用灵活，加快系统运行速度


#创建存储过程
#delimiter //
#create procedure过程名(参数1类型，参数2类型,...)
#begin
#    执行语句;
#end   //
#delimiter ;
#例子:创建一个查询学生信息表的存储过程
delimiter //
create procedure mypro()
begin
	select* from student;
end  //
delimiter ;
#执行存储过程: call过程名(参数列表);
call mypro();
#删除存储过程: drop procedure过程名;
drop procedure mypro;

#触发器
drop trigger if exists mydelete;

delimiter //
create trigger mydelete
after delete
on student 
for each row
begin
	delete from sc where sc.s=old.S;
end //
delimiter ;

delete from student where student.s='01';

drop trigger if exists myine;
delimiter //
create trigger myine
after insert
on student 
for each row
begin
	insert into sc value(new.s,'01',null);
end //
delimiter ;

insert into student (s,sname) value ('20','saber');

drop trigger if exists myupd;
delimiter //
create trigger myupd
after update
on student 
for each row
begin
	update  sc set s=new.s where s=old.s;
end //
delimiter ;

update student  set s='10' where s='20';


#事务
create table bank
(
	name varchar(45),
    money double not null,
    check(money>=0)       #mysql里面check是无效的
);

insert into bank values('侍郎',1000000);
insert into bank values('saber',777777);
select *from bank;

update bank set money=money-100 where name='侍郎';
update bank set money=money+100 where name='saber';


#事务语法
#开启事务：start transaction;
#执行语句1
#执行语句2
#commit;#提交
#rollback;#回滚

start transaction;

update bank set money=money-100 where name='侍郎';
update bank set money=money+100 where name='saber';
select *from bank;
commit;#提交

rollback;#回滚
