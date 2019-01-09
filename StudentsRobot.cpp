/*
 * StudentsRobot.cpp
 *
 *  Created on: Dec 28, 2018
 *      Author: hephaestus
 */

#include "StudentsRobot.h"

StudentsRobot::StudentsRobot(ServoEncoderPIDMotor * motor1,
		ServoEncoderPIDMotor * motor2, HBridgeEncoderPIDMotor * motor3,
		Servo * servo) {
	Serial.println("StudentsRobot::StudentsRobot called here ");
	this->servo=servo;
	this->motor1=motor1;
	this->motor2=motor2;
	this->motor3=motor3;
	motor1->attach(MOTOR1_PWM, MOTOR1_ENCA, MOTOR1_ENCB);
	motor2->attach(MOTOR2_PWM,  MOTOR2_ENCA, MOTOR2_ENCB);
	motor3->attach(MOTOR3_PWM,MOTOR2_DIR, MOTOR3_ENCA, MOTOR2_ENCB);



	motor1->setSetpoint(motor1->getPosition());
	motor2->setSetpoint(motor2->getPosition());
	motor3->setSetpoint(motor3->getPosition());



	// Set up digital servos
	servo->setPeriodHertz(330);
	servo->attach(SERVO_PIN, 1000, 2000);
}
void StudentsRobot::updateStateMachine() {
	switch (status) {
	case StartupRobot:
		//Do this once at startup
		status = StartRunning;
		Serial.println("StudentsRobot::updateStateMachine StartupRobot here ");
		break;
	case StartRunning:
		Serial.println("Start Running");
		status = Running;
		break;
	case Running:
		// Do something
		break;
	case Halting:
		// save state and enter safe mode
		Serial.println("Halting State machine");
		status = Halt;
		break;
	case Halt:
		// in safe mode
		break;

	}
}


void StudentsRobot::pidLoop() {
	motor1->loop();
	motor2->loop();
	motor3->loop();
}

void StudentsRobot::Approve(float * data) {
	// approve the procession to new state
	Serial.println("StudentsRobot::Approve");

	if (myCommandsStatus == Waiting_for_approval_to_pickup) {
		myCommandsStatus = Waiting_for_approval_to_dropoff;
	} else {
		myCommandsStatus = Ready_for_new_task;
	}
}
void StudentsRobot::ClearFaults(float * data) {
	// clear the faults somehow
	Serial.println("StudentsRobot::ClearFaults");
	myCommandsStatus = Ready_for_new_task;
	status = StartRunning;
}
void StudentsRobot::EStop(float * buffer) {
	// Stop the robot immediatly
	Serial.println("StudentsRobot::EStop");
	myCommandsStatus = Fault_E_Stop_pressed;
	status = Halting;

}
/**
 * PickOrder
 *
 * @param buffer A buffer of floats containing the pick order data
 *
 * buffer[0]  is the material, aluminum or plastic.
 * buffer[1]  is the drop off angle 25 or 45 degrees
 * buffer[2]  is the drop off position 1, or 2
 *
 * This function is called via coms.server() in:
 * @see RobotControlCenter::fastLoop
 */
void StudentsRobot::PickOrder(float * buffer) {
	float pickupMaterial = buffer[0];
	float dropoffAngle = buffer[1];
	float dropoffPosition = buffer[2];
	Serial.println(
			"StudentsRobot::PickOrder Recived from : " + String(pickupMaterial)
					+ " " + String(dropoffAngle) + " "
					+ String(dropoffPosition));
	myCommandsStatus = Waiting_for_approval_to_pickup;
}
