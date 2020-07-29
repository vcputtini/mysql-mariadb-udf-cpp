## libutilities - Native C++ UDF library for MariaDB(tm)/MySQL(tm)

It is a minimal library of functions related
to be used directly in SQL statements in MariaDB(tm)/MySQL(tm).

The library is composed of UDFs written in C ++ and were compiled and tested in the Linux environment.

As for performance, we were unable to test them with thousands or more data. However, given the characteristics inherent to the development model, we believe that the expected high performance will be achieved.

This project is still in its early stages of development and I hope to be able to improve it as much as possible.

An objective that we greatly appreciated during development was
that we can contribute so that more people can understand
how UDF's in C++ can be programmed for use in MariaDB(tm)/MySQL(tm) and take advantage the best of this technique.

## Comments

Issue 1: MariaDB [test]> CREATE OR REPLACE AGGREGATE FUNCTION func-name RETURNS REAL SONAME 'lib-name.so';
ERROR 2013 (HY000): Lost connection to MySQL server during query

This problem is recurrent and can be very annoying to solve. I managed to avoid this problem by deleting the previous library before copying the new one to the "plugin" directory.
I strongly advise you not to test with new libraries in production environments in general.
Before attempting to install a new library, make a backup copy of the one in use.

Issue 2: Sometimes we install a new library and everything seems to go smoothly, but when we try to use the new version of a function, the operation is the same as the old function.

I have had this problem a few times, but until that moment I am not sure why it occurs. Up to this point the only solution that has taken effect has been to restart the database server.

## General Syntax

First argument: field name. Must be on of these: DOUBLE, DECIMAL or INTEGER<br>
Second argument: condition. Are valid: '=', '<>', '>', '>=', '<', '<='<br>
Third argument: value or field name. this argument is sensitive to decimal places. If you enter 10, it will be considered as an integer, if you enter 10.0 as a decimal/double number.<br>

## Aggregations

These aggregations were developed to simplify some SQL statements, eliminating the use of conditions in the WHERE clause as much as possible, but maintaining the high performance of the system.

### count_if(fieldname,condition,value)

Implementation of the count_if() aggregation, which will count the number of lines based on an established condition.

_INTEGER count_if(fieldname, condition, value|fieldname)_

e.g:

SELECT count_if(salary, '>', 10000.0) FROM tsal;

Approach using the WHERE clause:

SELECT COUNT(salary) FROM tsal WHERE salary > 10000.0

### sum_if(fieldname,condition,value)

Implementation of the sum_if() aggregation, which will calculate the sum of the values of the past field as the first argument.

_REAL sum_if(fieldname, condition, value|fieldname)_

e.g:

SELECT sum_if(salary, '>', 10000.0) FROM tsal;

Approach using the WHERE clause:

SELECT SUM(salary) FROM tsal WHERE salary > 10000.0

### avg_if(fieldname,condition,value)

Implementation of the avg_if() aggregation, which will calculate the average of the values of the past field as the first argument.

_REAL avg_if(fieldname, condition, value|fieldname)_

e.g:

SELECT avg_if(salary, '>', 10000.0) FROM tsal;

Approach using the WHERE clause:

SELECT AVG(salary) FROM tsal WHERE salary > 10000.0


## Instructions

### To compile and install liblocation

git clone https://github.com/vcputtini/mysql-mariadb-udf-cpp  <br>
cd libutilities  <br>
make            <br>
make install    <br>  

### Create UDF's

To create (register) the UDF's to used in the database, we will use the DDL's that are encoded in the __docs/create-funcs.sql__ file.

__Line command:__ Using mysql command.

mysql [-p] [-D test] <  docs/impala-create-funcs.sql
