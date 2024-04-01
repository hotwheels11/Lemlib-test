#include "main.h"
#include "lemlib/api.hpp"
#include "autoSelect/selection.h"
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

hh

pros::Motor LB(1, pros::E_MOTOR_GEARSET_06, true); // port 1, blue gearbox, not reversed
pros::Motor LM(3, pros::E_MOTOR_GEARSET_06, true); // port 2, blue gearbox, not reversed
pros::Motor LF(11, pros::E_MOTOR_GEARSET_06, true); // port 3, blue gearbox, not reversed
pros::Motor RB(8, pros::E_MOTOR_GEARSET_06, false); // port 4, blue gearbox, reversed
pros::Motor RM(10, pros::E_MOTOR_GEARSET_06, false); // port 4, blue gearbox, reversed
pros::Motor RF(17, pros::E_MOTOR_GEARSET_06, false); // port 4, blue gearbox, reversed
pros::MotorGroup Left_Drive({LB,LM,LF});
pros::MotorGroup Right_Drive({RB,RM,RF});

// inertial sensor
pros::Imu inertial_sensor(19); // port 19

// odometry struct
lemlib::OdomSensors sensors {
    nullptr, // vertical tracking wheel 1
    nullptr, // vertical tracking wheel 2
    nullptr, // horizontal tracking wheel 1
    nullptr, // we don't have a second tracking wheel, so we set it to nullptr
    &inertial_sensor // inertial sensor
};

lemlib::Drivetrain drivetrain {
    &Left_Drive, // left drivetrain motors
    &Right_Drive, // right drivetrain motors
    10, // track width
    3.25, // wheel diameter
    360, // wheel rpm
	3 //Chase Power
};

// CHASSIS PID
lemlib::ControllerSettings lateralController(
	8, // proportional gain (kP)
    0, // integral gain (kI)
    30, // derivative gain (kD)
    3, // anti windup
    1, // small error range
    100, // small error range timeout
    3, // large error range
    500, // large error range timeout
    20 // maximum acceleration (slew)
);

lemlib::ControllerSettings angularController(
	4, // proportional gain (kP)
    0, // integral gain (kI)
    40, // derivative gain (kD)
    3, // anti windup
    1, // small error range
    100, // small error range timeout
    3, // large error range
    500, // large error range timeout
    0 // maximum acceleration (slew)
);


// create the chassis
lemlib::Chassis chassis(drivetrain, lateralController, angularController, sensors);
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	selector::init();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
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
void autonomous() {
	if(selector::auton == 1){ //run auton for Front Red 
	chassis.setPose(100,100,100);
	}
	chassis.setPose(0,0,0);
}

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
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	while (true) {
		chassis.tank(master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_RIGHT_Y), 5);

		pros::delay(20);
	}
}
