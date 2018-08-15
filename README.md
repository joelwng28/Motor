# Motor
Embedded Systems Project
Lab 10 Report
Samuel Zhang (shz96), Zi Zhou Wang (zw3948)
Saadallah
4/14/2018

Requirements document
1. Overview
  1.1. Objectives: Why are we doing this project? What is the purpose?
The objectives of this project are to design, build and test a brushed DC motor controller. The motor should spin at a constant speed and the operator can specify the desired set point. Educationally, students are learning how to interface a DC motor, how to measure speed using input capture, and how to implement a digital controller running in the background.
 
  1.2. Process: How will the project be developed?
The project will be developed using the LaunchPad. There will be two switches that the operator will use to specify the desired speed of the motor. The system will be built on a solderless breadboard and run on the usual USB power. The system may use the on board switches or off-board switches. A hardware/software interface will be designed that allows software to control the DC motor. There will be at least five hardware/software modules: tachometer input, switch input, motor output, LCD output, and the motor controller.  The process will be to design and test each module independently from the other modules. After each module is tested, the system will be built and tested.
 
  1.3. Roles and Responsibilities: Who will do what?  Who are the clients?
EE445L students are the engineers and the TA is the client. Students will work on all portions of the project together such that both partners understand the entire development of the system.
 
  1.4. Interactions with Existing Systems: How will it fit in?
            	The system will use the microcontroller board, a solderless breadboard, and the DC motor shown in Figure 10.1. The wiring connector for the DC motor is described in the PCB Artist file Lab10E_Artist.sch. It will be powered using the USB cable. You must use a +5V power from the lab bench, but please do not power the motor with a voltage above +5V. Do not connect this bench supply to Vbus (LaunchPad +5V). However, you must have a common ground.
 
  1.5. Terminology: Define terms used in the document.
For the terms integral controller, PWM, board support package, back emf, torque, time constant, and hysteresis, see textbook for definitions.
 
  1.6. Security: How will intellectual property be managed?
The system may include software from TivaWare and from the book. No software written for this project may be transmitted, viewed, or communicated with any other EE445L student past, present, or future (other than the lab partner of course). It is the responsibility of the team to keep its EE445L lab solutions secure.
 
2. Function Description
  2.1. Functionality: What will the system do precisely?
If all buttons are released, then the motor should spin at a constant speed. If switch 1 is pressed and released, the desired speed should increase by 5 rps, up to a maximum of 40 rps. If switch 2 is pressed and released, the desired speed should decrease by 5 rps, down to a minimum of 0 rps.
Both the desired and actual speeds should be plotted on the color LCD as a function of time similar to Figure 10.4 .
 
  2.2. Scope: List the phases and what will be delivered in each phase.
Phase 1 is the preparation; phase 2 is the demonstration; and phase 3 is the lab report. Details can be found in the lab manual.
  2.3. Prototypes: How will intermediate progress be demonstrated?
A prototype system running on the LaunchPad and solderless breadboard will be demonstrated. Progress will be judged by the preparation, demonstration and lab report.
 
  2.4. Performance: Define the measures and describe how they will be determined.
The system will be judged by three qualitative measures. First, the software modules must be easy to understand and well-organized. Second, the system must employ an integral controller running in the background. There should be a clear and obvious abstraction, separating the state estimator, user interface, the controller and the actuator output. Backward jumps in the ISR are not allowed. Third, all software will be judged according to style guidelines. Software must follow the style described in Section 3.3 of the book. There are three quantitative measures. First, the average speed error at a desired speed of 60 rps will be measured. The average error should be less than 5 rps. Second, the step response is the time it takes for the new speed to hit 60 rps after the set point is changed from 40 to 60 rps.  Third, you will measure power supply current to run the system. There is no particular need to minimize controller error, step response, or system current in this system.
 
  2.5. Usability: Describe the interfaces. Be quantitative if possible.
There will be two switch inputs. The tachometer will be used to measure motor speed. The DC motor will operate under no load conditions,
 
  2.6. Safety: Explain any safety requirements and how they will be measured.
    	Figure 10.2 shows that under a no load condition, the motor current will be less than 100 mA. However, under heavy friction this current could be 5 to 10 times higher. Therefore, please run the motors unloaded. Connecting or disconnecting wires on the protoboard while power is applied will damage the microcontroller. Operating the circuit without a snubber diode will also damage the microcontroller.
 
3. Deliverables
  3.1. Reports: How will the system be described?
A lab report described below is due by the due date listed in the syllabus. This report includes the final requirements document.
 
  3.2. Audits: How will the clients evaluate progress?
The preparation is due at the beginning of the lab period on the date listed in the syllabus.
 
  3.3. Outcomes: What are the deliverables? How do we know when it is done?
There are three deliverables: preparation, demonstration, and report. 

Hardware Design



Measurement Data

Procedure 1) Give the voltage, current, and resistance measurements

Procedure 2) IBE and ICE while spinning.
Ibe = 0.00378 A, Ice = 0.084 A
Procedure 3) Two screen shots of the hardware in operation.


Procedure 4) Specify the maximum time to execute once instance of the ISR
9 microseconds

Procedure 4) Specify the average controller error
0.2 seconds
Procedure 4) Specify the approximate response time
1.2 seconds 
Procedure 5) Measurements of current required to run the system, with and without the motor spinning
113 mA for system running
31 mA for system not running

Analysis and Discussion 
1) What is torque? What are its units?
Torque - The tendency of a force to rotate an object about an axis, fulcrum, or pivot. Just as a force is a push or a pull, a torque can be thought of as a twist to an object. It is units of Newton-meters or N•m
2) Draw an electrical circuit model for the DC motor coil, and explain the components. Use this circuit model to explain why the current goes up when friction is applied to the shaft
The DC motor is modeled as a resistor in series with an inductor. When a load is placed on the motor, the shaft spins slower due to friction. As a consequence, the coil that is engaged in the motor is energized for a longer period of time. Because inductors act as electrical shorts the longer they are powered with a DC supply, the inductor draws more current and thus the motor draws more current.



3) Explain what parameters were important for choosing a motor drive interface chip (e.g., TIP120 or 2N2222). How does your circuit satisfy these parameters?
Current to drive the motor was the most important parameter. We had to chose the TIP120 because the 2N2222 did not provide enough current gain for the motor. The interface from the TM4C provides enough base current to the TIP120 to drive the motor.
4) You implemented an integral controller because it is simple and stable. What other controllers could you have used? For one other type of controller how would it have been superior to your integral controller.
Direct Motor Control. We could have just calculated the PWM nescessary for each speed and set it accordingly. It would have removed jitter from the motor speed.
5) It the motor is spinning at a constant rate, give a definition of electrical power in terms of parameters of this lab? Research the term “mechanical power”. Give a definition of mechanical power. Are the electrical power and mechanical power related?
The motor is not quite spinning at a constant rate but is close. Electrical power in this lab is Volts * Amps and is given in unit Watts. For a motor, it's power is given as Torque * Angular Velocity and is also given in unit Watts. Because both electrical power and mechanical power is given in Watts, the two are linearly related with some slight power loss due to friction.



