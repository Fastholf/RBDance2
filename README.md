RBDance2
========

This program is designed for robot shows. It can play predefined moves on several robots connected via bluetooth with music. Robots are [Robobuilders](http://www.robobuilder.net/en/). 
Actually you can play the same moves directly from robots. The problem is that their moves won't be synchronous. Which is bad, because these moves are supposed to be dance.
So this program solves this problem.

Build
=====
You will need [Qt](http://qt-project.org/) to build this project.

Settings
========

* First of all you need to configure __settings.txt__ file. 
  * First line is the __root__ folder for other configuration files. So if you are planning to hold them with the program binaries just leave this line empty.
  * Second line is the robot description filename.
  * And the last is the scenatios list filename.
* Next you need to provide __robot description__ file. Each line of the file should have following format: 
`<robot name> <serial number> <port number> <bluetooth name>`.
Where `<robot name>` you pick by yourself, `<serial number>` is the robot's serial number (if you do not know, it just put `0`), `<port number>` is the number of COM port (for example, if robot is connected to port COM32 put 32 for this field) and `<bluetooth name>` is the device name in the list of bluetooth devices for this robot (if you do not know it put `<robot name>`).
* And at last you need to provide __scenarios list__ file. Each line of this file is the path to the scenario file (for example, `MySuperDancesFolder/MySuperDance1/MySuperDance1Scenario.txt`).

Scenario description file
=========================

Scenario description file describes one dance. Each line can be one of the following commands.
* `file <dance file path>`. Where dance file is the motion sequence file created in Motion Builder program which is provided for RoboBuilder robots. This file has *.h extension.
* `role <robot number> <dance file number>`. Where robot number is the index number of robot in the robot description file starting from `0` and dance file number is the index number of dance file provided in this scenario file, starting from `0`.
* `music <music file path>`. Music file path is the path to the file of *.mp3 which you want to play alongside with your dance.
