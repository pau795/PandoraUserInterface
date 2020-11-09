## Pandora Visualization Interface

This application lets you watch a graphical visualization of a Pandora simulation.

### Setup

This application only works in the Linux platform. It has been tested under the Ubuntu 16.04 distribution. Also, this application is using OpenGL to perform the visualization, so it makes use of the GPU. It is required to have the GPU drivers installed in your machine to run this application.

Initially, you will need the following libraries and packages:
- GCC/G++ compiler:

		$ sudo apt-get install build-essentials
        
- Qt5 Libraries:

		$sudo apt-get install qt5-default
        
 - HDF5 1.8.19 libraries. You can get it [here](https://portal.hdfgroup.org/display/support/HDF5+1.8.19#files). You can also find the install instructions.

Once you have all the required libraries installed, you can proceed to compile the application. Open a console terminal on the repository directory, and write the following commands:

		$ qmake
        $ make

Finally, to run the application, write on the console the following command:

		$ ./PandoraVisualizationInterface
 	