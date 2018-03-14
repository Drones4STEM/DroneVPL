# DroneVPL
A software of Visual Programming Language for Drones. Developed with QT4.8, avaliable on windows only.
# Demo
Drag widgets
![drag_widgets](doc/demo_(1).gif)
![drag_widgets_2](doc/demo_(2).gif)
Save and compile
![save_compile](doc/demo_(3).gif)

# Front-end

The front-end program enables user to write the program by dragging and linking widgets. And Other fuctions are provided, including file save, compile.

# Back-end
The back-end program enables user to:
 - save the graphical code in .xml file.
 - open a .xml file with correct format and recosntruct the graphcal code.
 - compile the graphical code and generate corresponding python codes. The latter calls
 interface of DroneController and enables drone to fly automatically.

