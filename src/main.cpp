#include "main.h"

#include<string>  

#define Embutton 'h'
#define PORT_PING 'a'
#define PORT_ECHO 'b'
#define Cameramotor 'c'
#define left_switch 'd'
#define right_switch 'f'
#define start_button 'g'

using namespace std;
using namespace okapi;


Controller controller; //Initializing controller here


ControllerButton camera_left(ControllerDigital::L1); //button
ControllerButton camera_right(ControllerDigital::R1); //button

//definding sensors/motors
pros::ADIMotor camera(Cameramotor); //camera motor using cortex motor
pros::ADIUltrasonic usensor (PORT_PING, PORT_ECHO); //ultrasonic sensor
pros::ADIDigitalIn leftswitch (left_switch);
pros::ADIDigitalIn rightswitch (right_switch);
pros::ADIDigitalIn startbutton(start_button);
pros::Motor leftmotor(1);
pros::Motor rightmotor(11);

/*
CAMERA IF WE HAVE V5 MOTORS
Motor cameramotor('c', false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);//Initializing motors
MotorGroup camera({cameramotor}); //motorgroup
*/


ControllerButton emergency_button(ControllerDigital::B); //emergency button

auto drivetrain = ChassisControllerBuilder()
	.withMotors(1, -11)
	.withDimensions(AbstractMotor::gearset::green, {{4_in, 14_in}, imev5GreenTPR})
	.withGains({0.02, 0, 0}, {0, 0, 0})
	.build();

class turnOut: public ControllerOutput<double> { //subclassing the ControllerOutput class to rotate chassis
  public:

	virtual void controllerSet(double ivalue) override {
    	drivetrain -> getModel() -> rotate(ivalue);
		controllerSet(ivalue);
  	}
};



void on_center_button() {
	static bool pressed = false; 
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */


void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");
	pros::lcd::register_btn1_cb(on_center_button);
	pros::lcd::set_text(1,"Initialization Complete.");
}

bool start() {
	if (startbutton.get_value() == HIGH) {
		return true;
	} else {
		return false;
	}
}


void kill_switch() { //Kill-switch function. Stops all motors.
	pros::ADIDigitalIn embutton (Embutton);
	//|| embutton.get_value() == false
	if (emergency_button.isPressed()) {
		while(true) {
			drivetrain -> stop();
		}
		
	}
}

void stop() {
	while(true) {
		drivetrain -> stop();
	}
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */



void opcontrol() {
	while (true) {
		if (start() == true) {
			while (true) {
		
				if (leftswitch.get_value() == HIGH && rightswitch.get_value() == LOW) { //left
					drivetrain -> getModel() -> tank(0.3, 0);
					//pros::delay(200);
				} 
				if(rightswitch.get_value() == HIGH && leftswitch.get_value() == LOW) { //right
					drivetrain -> getModel() -> tank(0, 0.8);
					//pros::delay(200);
				} 
				if(rightswitch.get_value() == LOW && leftswitch.get_value() == LOW) {
					drivetrain -> getModel() -> tank(0.15, 0.15);
					//pros::delay(200);
				}
				if(rightswitch.get_value() == HIGH && leftswitch.get_value() == HIGH) {
					drivetrain -> getModel() -> tank(0, 0.8);
				}
				
				pros::delay(200);
				kill_switch(); //checking for emergeny button press
				pros::lcd::set_text(2,"I LOVE UR MOM"); 
				pros::delay(2);//Slight delay to not significantly halt operations, but to conserve resources
			}
		}

	}
		

}

