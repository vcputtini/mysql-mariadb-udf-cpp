## Native C++ UDF's for MariaDB(tm)/MySQL(tm).

Our goal is to create and make available native C++ UDFs/UDAs for use
on MySQL(tm)/MariaDB(tm) databases.
Thereby helping to spread this wonderful feature that allows us to solve
problems of all kinds with performance and security.

As much as possible we will provide functions for use and learning.

# Before continuing, pay attention to these details:
- The libraries must be compiled in a compatible environment both with respect to the Linux distro in use as well as the version of MySQL or MariaDB.<br>
- Never install new libraries directly in the production environment.<br>
- We advise that after installing the libraries in the 'plugin' directory, the database server must be restarted.<br>

# Libraries

- liblocation:<br>
It is a minimal library of functions related to Geo localization to be used directly in SQL statements in MariaDB/MySQL.
Please see liblocation/README.md for details.

- libutilities:<br>
It provides a set of utility functions for general use. Such as: count_if(), sum_if() and avg_if(). Please see libutilities/README.md for details.

- liblogsquid:<br>
Provides UDFs to directly handle Squid's log line.
At the moment just handles built-in logformats.
Please see libsquid/README.md for details.
