## Pandora User Interface

This application sets up a working environment for the social simulator Pandora. It opens a working Pandora model, loads its parameters, and lets you design a batch of experiments to simulate. Then you can run the experiments from the interface, to keep track of the simulation execution.

### Setup

This application is designed to be run within a Docker container. Follow the next instructions to 
1. First of all, you need to install Docker in your device. You can get started through [this link](https://www.docker.com/get-started).
2. To make the Docker container to launch the application, we are using a Dockerfile. To create the container, open a console terminal on the repository directory, and write the following command:

		$ docker build . --tag pandora_interface
    
3. Once you have the container set up, you have to run it. Here are the instructions to run in depending on the OS that you are using:
	##### Windows
     
   	1. You will need an X server client to launch the interface. You can get it [here](https://sourceforge.net/projects/vcxsrv/files/latest/download).
   	2. Open a console terminal, and write the following command:
    
    ```
    $ docker run --privileged -it -e DISPLAY=YOUR_LOCAL_IP:0.0 -v ABSOLUTE_PANDORA_PATH:/root/PANDORA pandora_interface
   
    example:
   
    $ docker run --privileged -it -e DISPLAY=192.168.1.128:0.0 -v C:\Users\User1\Documents\PANDORA:/root/PANDORA pandora_interface
    ```

 	##### Linux
    
   	1. Open a console terminal, and write the following commands:
   	
    ```
    $ xhost +local:root
  
    $ sudo docker run -it \
     --env="DISPLAY" \
     --env="QT_X11_NO_MITSHM=1" \
     --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
     -v ABSOLUTE_PANDORA_PATH:/root/PANDORA pandora_interface
    
    example:
    
    $ sudo docker run -it \
     --env="DISPLAY" \
     --env="QT_X11_NO_MITSHM=1" \
     --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
     -v /home/user/Documents/PANDORA:/root/PANDORA pandora_interface
    
    ```
    
 
4. Now you are in the Docker container through the console terminal. To compile the application, write the following commands:

		$ qmake
        $ make
   
5. Finally, to run the application, write the following command from the Docker container console:

		$ ./PandoraUserInterface
