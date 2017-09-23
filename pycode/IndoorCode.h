prepare:
	import FlightController
	import sys
	import time

	# Create a Vehicle object
	myCopter = FlightController.Vehicle()

	# Connect and initialize the vehicle, enable SITL here
	if not myCopter.initialize(simulation = True):
		sys.exit(1)



arm:
	# Try arming the vehicle
	timeoutCounter = 0
	while not myCopter.arm():
		timeoutCounter += 1
		if timeoutCounter > 3:
			print "Cannot arm the vehicle after 3 retries."
			sys.exit(1)

TakeOff():
	# Takeoff
	if not myCopter.takeoff(1):
		sys.exit(1)

Land():
	timeoutCounter = 0
	while not myCopter.land():
		timeoutCounter += 1
		if timeoutCounter > 3:
			print "Critical: Cannot land the vehicle after 3 retries."
			sys.exit(1)

Hover():
	# Hover for 10 seconds
	time.sleep(10)

Forward()：
	# Go forward at 1m/s for 5 seconds
	print "Going forward at 1m/s for 5s"
	myCopter.send_nav_velocity(0, -1, 0)
	time.sleep(5)

Backward()：
	# Go backward at 1m/s for 5 seconds
	print "Going forward at 1m/s for 5s"
	myCopter.send_nav_velocity(0, 1, 0)
	time.sleep(5)

Right():
	# Go rightward at 1m/s for 5 seconds
	print "Going rightward at 1m/s for 5s"
	myCopter.send_nav_velocity(1, 0, 0)
	time.sleep(5)

Left():
	# Go leftward at 1m/s for 5 seconds
	print "Going rightward at 1m/s for 5s"
	myCopter.send_nav_velocity(-1, 0, 0)
	time.sleep(5)

Up():
	# Without hovering, go upward at 0.2m/s for 5 seconds
	print "Going upward for 0.2m/s for 5 seconds"
	myCopter.send_nav_velocity(0, 0, -0.2)
	time.sleep(5)

Down():
	# Without hovering, go down at 0.2m/s for 5 seconds
	print "Going upward for 0.2m/s for 5 seconds"
	myCopter.send_nav_velocity(0, 0, 0.2)
	time.sleep(5)