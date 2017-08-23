/*
This file records python codes of widgets.

-----------------------Acton----------------------------
Arm()
    print "Arming motors"
    # Copter should arm in GUIDED mode
    vehicle.mode = VehicleMode("GUIDED")
    vehicle.armed = True    

    # Confirm vehicle armed before attempting to take off
    while not vehicle.armed:      
        print " Waiting for arming..."
        time.sleep(1)

TakeOff(double? aTargetAltitude)
    print "Taking off!"
    vehicle.simple_takeoff(aTargetAltitude) # Take off to target altitude

    # Wait until the vehicle reaches a safe height before processing the goto (otherwise the command 
    #  after Vehicle.simple_takeoff will execute immediately).
    while True:
        print " Altitude: ", vehicle.location.global_relative_frame.alt 
        #Break and return from function just below target altitude.        
        if vehicle.location.global_relative_frame.alt>=aTargetAltitude*0.95: 
            print "Reached target altitude"
            break
        time.sleep(1)

Go(double airspeed,double groundspeed) 
	vehicle.airspeed = airspeed

	print "Going towards first point for 30 seconds ..."
	point1 = LocationGlobalRelative(-35.361354, 149.165218, 20)
	vehicle.simple_goto(point1,groundspeed)

Hover(double t)
	time.sleep(t)

Delay()

Land()
	vehicle.mode = VehicleMode("RTL")

-------------------------IO--------------------------
Battery()
	battery = dronekit.Battery
	print battery.voltage
	print battery.current
	print battery.level

Gimbal(pitch,roll,yaw)
	#Point the gimbal straight down
	vehicle.gimbal.rotate(pitch, roll, yaw)



*/