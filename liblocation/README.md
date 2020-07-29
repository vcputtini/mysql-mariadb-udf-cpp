## liblocation - Native C++ UDF library for MariaDB(tm)/MySQL(tm)

It is a minimal library of functions related to Geo localization
to be used directly in SQL statements in MariaDB(tm)/MySQL(tm).

The library is composed of UDFs written in C ++ and were compiled and tested in the Linux environment.
The functions are mathematically correct and can be used in the most varied cases to which they apply.

As for performance, we were unable to test them with thousands or more geographic data. However, given the characteristics inherent to the development model, we believe that the expected high performance will be achieved.

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


## Aggregations
The sum of the distances between the points, resulting in the total walking distance.

_DOUBLE geo_sumdist(lat_a DOUBLE, lon_a DOUBLE, lat_b DOUBLE, lon_b DOUBLE, VARCHAR()/CHAR())_

The 'unit' argument refers to the unit of measurement in which the distance will be calculated, which can be either _'km'_ or _'mi'_.

Minimal table:

-- lat_a and lat_b [-]000.0000000000

create table coords (<br>
  pto varchar(5) not null,<br>
  lat_a double(15,10) not null,<br>
  lat_b double(15,10) not null,<br>
);<br>

insert into coords (pto, lat_a, lon_a) values ('p00',-31.721238,-55.980449);<br>
insert into coords (pto, lat_a, lon_a) values ('p01',-31.744598,-55.985942);<br>
insert into coords (pto, lat_a, lon_a) values ('p02',-31.786047,-55.984569);<br>

MariaDB [test]> select geo_sumdist(lat_a, lon_a,'km') from coords;<br>
+---------------------------+<br>
| geo_sumdist(lat_a, lon_a) |<br>
+---------------------------+<br>
|              7.2596970221 |<br>
+---------------------------+<br>

_NOTE_: The sum value can be returned in either Kilometers (km) or Miles (mi).<br>
        If the user does not inform which unit of measurement is used,<br>
        the function returns the value in km.<br>
_NOTE_: It can be different from the distance calculated from the<br>
         coordinates between the start point A and end point B directly.


## Functions
Calculates the distance between two point given their geographical coordinates, latitude and longitude. Use Haversine formula. The coordinate fields must be informed decimal degrees.<br>
_DOUBLE geo_distance(lat_a DOUBLE, lon_a DOUBLE, lat_b DOUBLE, lon_b DOUBLE, unit VARCHAR()/CHAR());_

Calculate the azimuth from latitude and longitude.The coordinate fields must be informed decimal degrees.<br>
_DOUBLE geo_azimuth(lat_a DOUBLE, lon_a DOUBLE, lat_b DOUBLE, lon_b DOUBLE);_

Convenience function to convert between some measurement units.<br>
Arguments:
cnv: kmh_to_mih | mih_to_kmh | mi_to_meters | meters_to_mi | feet_to_meteres | meters_to_feet.
unit_val: Value to be converted<br>
e.g:<br>
Field: veloc<br>
Value: 100.0<br>
select unit_convert(veloc, "kmh_to_mih"); or select unit_convert(veloc, 2, "kmh_to_mih");<br>
Returns:  62.1371 or 62.13<br>

_DOUBLE unit_convert(unit_val DOUBLE, [dec-places INT,] cnv VARCHAR());_<br>

## Instructions


### To compile and install liblocation

git clone https://github.com/vcputtini/mysql-mariadb-udf-cpp  <br>
cd liblocation  <br>
make            <br>
make install    <br>  

### Create UDF's

To create (register) the UDF's to used in the database, we will use the DDL's that are encoded in the __docs/create-funcs.sql__ file.

__Line command:__ Using mysql command.

mysql [-p] [-D test] <  docs/impala-create-funcs.sql
