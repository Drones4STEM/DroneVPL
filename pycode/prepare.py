import FlightController
import sys
import time

# Create a Vehicle object, local port 9000 of cmavnode
myCopter = FlightController.Vehicle(FCAddress='127.0.0.1:9000')

# Connect and initialize the vehicle
if not myCopter.initialize():
	sys.exit(1)


#arm
# Try arming the vehicle
timeoutCounter = 0
while not myCopter.arm():
	timeoutCounter += 1
	if timeoutCounter > 3:
		print "Cannot arm the vehicle after 3 retries."
		sys.exit(1)