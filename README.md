 ## FlightGear-Interpreter

An Interpreter project for a FlightGear flight simulator. The program connects to the simulator and flys the aircraft based on the user's commands.

**Prerequisites**

Linux environment to run the code

**Installing**

Download and install the simulator on your computer -[https://www.flightgear.org/download/](https://www.flightgear.org/download/) 

<p> Add the generic_small.xml file to the /data/Protocol directory where you installed the simulator
Config the following settings in the 'Settings' tab in the simulator:

`--telnet=socket,in,10,127.0.0.1,5402,tcp` 

`--generic=socket,out,10,127.0.0.1,5400,tcp,generic_small` 

This will open two communication sockets - 'in' where you send commands to the simulator, and 'out' where you receive data from it. </p>

# Running

a. Exceute the code using the terminal or any c++ work environment.<br>
```
g++ -std=c++11 *.cpp -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -o a.out -pthread
```
Then, run:<br>
```
./a.out fly.txt 
```

The program will wait for a connection from the simulator.

b. Click the 'Fly!' icon in the simulator in the bottom left corner, and wait for the simulator to load. It will connect to the interpreter in the meantime.<br>

When fly.txt is exeshuted you will see the plane takeoff and fly.<br>

# About the code:
You may want to change the fly.txt script provided and create your own script.<br>
The code is designed to read lines of commands entered in a terminal from the use once the program has started running.<br> 
The commands can be entered one at a time or by using a pre-made script (make sure each command starts in a new line).<br>
In your cusotmized script you must first open the client and server connection:<br>
openDataServer(5400)<br>
connectControlClient("127.0.0.1",5402)<br>
Then, you will start intializing your variables, loops and commands:<br>

# Initialize your variables:
```
var warp -> sim("/sim/time/warp")
var magnetos -> sim("/controls/switches/magnetos")
var throttle -> sim("/controls/engines/current-engine/throttle")
var mixture -> sim("/controls/engines/current-engine/mixture")
var masterbat -> sim("/controls/switches/master-bat")
var masterlat -> sim("/controls/switches/master-alt")
var masteravionics -> sim("/controls/switches/master-avionics")
var brakeparking -> sim("/sim/model/c172p/brake-parking")
var primer -> sim("/controls/engines/engine/primer")
var starter -> sim("/controls/switches/starter")
var autostart -> sim("/engines/active-engine/auto-start")
var breaks -> sim("/controls/flight/speedbrake")
var heading <- sim("/instrumentation/heading-indicator/indicated-heading-deg")
var airspeed <- sim("/instrumentation/airspeed-indicator/indicated-speed-kt")
var roll <- sim("/instrumentation/attitude-indicator/indicated-roll-deg")
var pitch <- sim("/instrumentation/attitude-indicator/internal-pitch-deg")
var rudder -> sim("/controls/flight/rudder")
var aileron -> sim("/controls/flight/aileron")
var elevator -> sim("/controls/flight/elevator")
var alt <- sim("/instrumentation/altimeter/indicated-altitude-ft")
var rpm <- sim("/engines/engine/rpm")
Print("waiting 2 minutes for gui")
Sleep(5000)
Print("let's start")
warp = 32000
Sleep(1000)
magnetos = 3
throttle = 0.2
mixture = 0.949
masterbat = 1
masterlat = 1
masteravionics = 1
brakeparking = 0
primer = 3
starter = 1
autostart = 1
Print("engine is warming...")
Print(rpm)
while rpm <= 750 {
	Print(rpm)
}
Sleep(5000)
Print("let's fly")
var h0 = heading
breaks = 0
throttle = 1
while alt < 1000 {
	rudder = (h0 - heading)/80
	aileron = -roll / 70
	elevator = pitch / 50
	Sleep(250)
	Print(alt)
}
Print("done")
```

The program reads the code, parses it and sends the commands to the simulator, changing the behavior of the aircraft accordingly. <br>In this example, the interpreter enters the loop, adjusts variables, prints the current altitude of the aircraft and halts the reading for 250 miliseconds. <br>When the altitude is over 1000, the loop will terminate and the next lines will be parsed.<br>

When the program reads the keyword "done", it will stop sending commands to the simulator, terminating the communication with it and ending the program.<br>
