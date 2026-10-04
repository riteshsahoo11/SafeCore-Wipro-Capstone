<h1> SafeCore </h1>

Linux-Based Industrial Safety Controller

SafeCore is a Linux-based software project that simulates an industrial safety system without requiring physical hardware.

The goal is to monitor machine conditions and later detect unsafe situations automatically.
<br>

🔍 Requirements

       🔻C and C++ only
       🔻Linux / Kali Linux
       🔻Linux device-driver concepts
       🔻Software-based architecture
       🔻No physical hardware required
       🔻Simulated sensor values
       🔻Git-based development

<br>

🔮 Planning

       SafeCore is being developed in four milestones:

      🔻 Milestone 1: Linux virtual device driver
      🔻 Milestone 2: Sensor monitoring and safety decisions
      🔻 Milestone 3: Fault handling and safe-state recovery
      🔻 Milestone 4: Final integration, testing and deployment

<br>
⚙️ Tech Stack
 Operating System:
       
       Kali Linux
Languages:

       C
       C++
       Linux Technologies
       Linux kernel module
Virtual character device:

       /dev
       ioctl()
       copy_to_user()
       copy_from_user()
       Mutex
       Kernel logging
Development Tools:

       GCC
       G++
       Make
       Git
       GitHub
<br>

💮 Designing

                        SAFECORE 
           +-------------+-------------+ 
           |                           | 
        KERNEL SPACE               USER SPACE 
           |                           |
       Virtual Device Driver       C++ Controller  
           |                           |
           +-------- /dev/safecore ----+
                           |
                         ioctl()
                           ↓
                      Sensor Data
                           ↓
                     Safety Engine
                           ↓
            +--------------+--------------+ 
            ↓              ↓              ↓ ▼ ▼ ▼ 
            NORMAL      WARNING        CRITICAL
                                          ↓
                                      SAFE_STATE
                                          ↓
                                      SAFE_READY
                                          ↓
                                     Manual Reset
                                          ↓
                                        NORMAL
                                           

<b>A sensor fault also sends the machine to SAFE_STATE.</b>

    🔻The driver provides the sensor data.
    🔻The C++ Safety Engine checks the data and decides the current condition.
 <br>
 
<h1> Milestone 1 — Linux Device Foundation </h1>
              
<h1>What I completed</h1>

       In this milestone, I built the basic Linux foundation of SafeCore before adding the actual safety logic.

       ✨Created the project structure and GitHub repository.
       ✨Set up the project to run on Kali Linux.
       ✨Created a virtual Linux device called /dev/safecore.
       ✨Implemented a basic Linux character device driver in C.
       ✨Added an interface for sending and receiving sensor data between user space and the kernel.
       ✨Added ioctl() commands to set and get sensor values.
       ✨Created a small test program to check whether the driver is working correctly.
       ✨Added scripts to load and unload the driver.
       ✨Tested communication between the test program and the Linux driver.
<br>
Sensor data currently used :
       
       <ul>
       <li>Temperature</li>
       <li>Motor RPM</li>
       <li>Vibration</li>
       <li>Sensor Validity</li>
       </ul>

<br>

Result

       Milestone 1 successfully established the Linux device-driver foundation of SafeCore.

       The system can now create the virtual device and exchange sensor data between user space and the Linux kernel.

       <h1>TEST OUTPUT For Milestone 1</h1>

    ─$ sudo ./tests/driver_smoke
    Temperature : 60 C
    Motor RPM   : 1200
    Vibration   : 2
    Valid       : 1

    Driver test PASSED

<br><hr>
<h1>Milestone 2 — Sensor Monitoring</h1>

The C++ part of SafeCore was added in this milestone.

The system now checks:

    Temperature
    Motor RPM
    Vibration

Current simulation limits:
<table>
  <thead>
    <tr>
      <th>Parameter</th>
      <th>Normal</th>
      <th>Warning</th>
      <th>Critical</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Temperature</td>
      <td>&lt; 70°C</td>
      <td>70–85°C</td>
      <td>&gt; 85°C</td>
    </tr>
    <tr>
      <td>Motor RPM</td>
      <td>&lt; 1500</td>
      <td>1500–1800</td>
      <td>&gt; 1800</td>
    </tr>
    <tr>
      <td>Vibration</td>
      <td>&lt; 4</td>
      <td>4–7</td>
      <td>&gt; 7</td>
    </tr>
  </tbody>
</table>

These values are only used for project simulation.

The user can also enter sensor values manually:

set <temperature> <rpm> <vibration>

Example:

    set 95 1200 2

    Result:

    State  : CRITICAL
    Reason : Temperature is above the critical limit

<h3>SafeCore can now receive simulated sensor values, evaluate them and report whether the machine condition is NORMAL, WARNING or CRITICAL.</h3>
<br><hr>

<h1>Milestone 3 -</h1> <b>Move the system into a safe state during critical conditions, handle sensor faults and support controlled recovery.</b>

Current states

       🔻NORMAL
       🔻WARNING
       🔻SAFE_STATE
       🔻SAFE_READY
Behaviour

<ul>
       <li>Normal readings keep the system in NORMAL.</li>
       <li>Warning readings move the system to WARNING.</li>
       <li>Critical readings move the system to SAFE_STATE.</li>
       <li>After the fault is cleared, the system moves to SAFE_READY.</li>
       <li>A reset is required before returning to NORMAL.</li>
       </ul>

