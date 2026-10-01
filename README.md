<h1> SafeCore </h1>

Linux-Based Industrial Safety Controller

SafeCore is a Linux-based software project that simulates an industrial safety system without requiring physical hardware.

The goal is to monitor machine conditions and later detect unsafe situations automatically.
<br>
<h1> Milestone 1 — Linux Device Foundation </h1>
What I completed

In this milestone, I built the basic Linux foundation of SafeCore before adding the actual safety logic.

Created the project structure and GitHub repository.
Set up the project to run on Kali Linux.
Created a virtual Linux device called /dev/safecore.
Implemented a basic Linux character device driver in C.
Added an interface for sending and receiving sensor data between user space and the kernel.
Added ioctl() commands to set and get sensor values.
Created a small test program to check whether the driver is working correctly.
Added scripts to load and unload the driver.
Tested communication between the test program and the Linux driver.
<br>
Sensor data currently used :
<ul>
<li>Temperature</li>
<li>Motor RPM</li>
<li>Vibration</li>
<li>Sensor Validity</li>
</ul>

Basic architecture
User Program or User Space
     |
     v
/dev/safecore
     |
     v
Linux Kernel Driver
     |
     v
Virtual Sensor Data
Result

Milestone 1 successfully established the Linux device-driver foundation of SafeCore.

The system can now create the virtual device and exchange sensor data between user space and the Linux kernel.
