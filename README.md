SnowCookie project
=============

Getting repository
-------
https (usual way):
* git clone https://github.com/loganek/snowcookie.git

ssh:
* git clone git@github.com:loganek/snowcookie.git

svn (not recommended):
* svn checkout https://github.com/loganek/snowcookie

Build
-------
* `$ cd snowcookie`
* `$ mkdir build`
* `$ cd build`
* `$ cmake ..`
* `$ make`

Run snow-edison
-------
snow-edison is Edison-based tcp server/data logger.

`$ ./edison-board/snow-edison [port [log_file]]`

Default values:
* port - 12345
* log_file - runnig time, in format "log_Y-m-d-H-M-S.txt"
