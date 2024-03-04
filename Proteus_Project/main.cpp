#include <FEHLCD.h> 
#include <FEHIO.h> 
#include <FEHUtility.h> 
#include <FEHMotor.h> 
#include <FEHServo.h> 
#include <FEHAccel.h> 
#include <FEHBattery.h> 
#include <FEHBuzzer.h> 
#include <FEHSD.h> 
#include <string.h> 
#include <stdio.h> 
#include <math.h>

#define WHEEL_RADIUS 1.25; 
#define PI 3.14159265358979;
#define COUNT_TOTAL 318;
#define WHEEL_DISTANCE 6.772;
#define ROBOT_POWER 25;
#define NEGATIVE_ROBOT_POWER -25;



//declare all motors and pins
AnalogInputPin Cds(FEHIO::P3_1); 
FEHMotor RightMotor(FEHMotor::Motor2, 9.0); 
FEHMotor LeftMotor(FEHMotor::Motor0, 9.0); 
DigitalEncoder RightEncoder(FEHIO::P2_3);
DigitalEncoder LeftEncoder(FEHIO::P2_0);
 

//check distance traveled
float checkDistance() {
    float distance;
    int counts = 0;
    counts = RightEncoder.Counts();
    distance = 2*PI;
    distance *= counts;
    distance *= WHEEL_RADIUS;
    distance /= COUNT_TOTAL;
    return distance;
}

//moveForward function
void moveForward(float distance, int percent) {
    float distanceTraveled=0;
    RightEncoder.ResetCounts();
    if (distance > 0) {
        RightMotor.SetPercent(percent);
        LeftMotor.SetPercent(percent); 
        while (distanceTraveled < distance) { 
            int counts = 0;
            counts = RightEncoder.Counts();
            distanceTraveled = 2*PI;
            distanceTraveled = distanceTraveled * counts;
            distanceTraveled = distanceTraveled * WHEEL_RADIUS;
            distanceTraveled = distanceTraveled / COUNT_TOTAL;
        }
        RightMotor.Stop();
        LeftMotor.Stop();
    } else {
        distance *= -1;
        RightMotor.SetPercent(-percent);
        LeftMotor.SetPercent(-percent);
        while (distanceTraveled < distance) {
            while (distanceTraveled < distance) { 
            int counts = 0;
            counts = RightEncoder.Counts();
            distanceTraveled = 2*PI;
            distanceTraveled = distanceTraveled * counts;
            distanceTraveled = distanceTraveled * WHEEL_RADIUS;
            distanceTraveled = distanceTraveled / COUNT_TOTAL;
            }
            RightMotor.Stop();
            LeftMotor.Stop();
        }
    }
}

//turnRobot function
void turnRobot(float angle) {
    if (angle > 0) {
        float angleMoved = 0; 
        float distanceTraveled = 0; 
        RightMotor.SetPercent(25);
        RightEncoder.ResetCounts();
        float distance;
        while (angleMoved < angle) {
            int counts = 0;
            counts = RightEncoder.Counts();
            distance = 2*PI;
            distance = distance * counts;
            distance = distance * WHEEL_RADIUS;
            distance = distance / COUNT_TOTAL;
            angleMoved = distance / WHEEL_DISTANCE;
        }
        RightMotor.Stop();
    } else {
        angle *= -1;
        float angleMoved = 0; 
        float distanceTraveled = 0; 
        LeftMotor.SetPercent(25);
        LeftEncoder.ResetCounts();
        float distance;
        while (angleMoved < angle) {
            int counts = 0;
            counts = LeftEncoder.Counts();
            distance = 2*PI;
            distance = distance * counts;
            distance = distance * WHEEL_RADIUS;
            distance = distance / COUNT_TOTAL;
            angleMoved = distance / WHEEL_DISTANCE;
        }
        LeftMotor.Stop();
    }
}

int main() {
    float distanceStart = 31;
    float ticketAngle = PI; 
    ticketAngle /= 2;
    float ticketDistance = 10;
    float buttonDistance = 10;
    float startAngle = -PI;
    startAngle /= 6;
    
    //move forward when light turns on
    float lightSense = Cds.Value();
    LCD.Clear();
    LCD.Write(lightSense);
    Sleep(1.0);

    //if run testing code - true
    //if running a run code - false
    bool test = false;
    bool backup = false;

    if (test == true) {
        //test backwards and turn clockwise 180
        float turnAngle = PI;
        turnAngle /= 2;
        moveForward(14, 25);
        turnRobot(turnAngle);
        moveForward(10, 25);
        turnRobot(-turnAngle);
        moveForward(4);


    } else if (backup == false) {
        //wait til light turns on
        while (lightSense > 2.9) {
            lightSense = Cds.Value();
        }
        
        //move forward up ramp
        turnRobot(startAngle);
        moveForward(1, 25);
        turnRobot(-startAngle);
        moveForward(distanceStart, 25);
        Sleep(1.0);
        //turn robot towards ticket
        turnRobot(ticketAngle);
        Sleep(1.0);

        //move forward to touch ticket machine
        moveForward(ticketDistance, 25);
        Sleep(1.0);
        turnRobot(-ticketAngle);
        Sleep(1.0);
        moveForward(6, 25);
        Sleep(1.0);
        float backupDistance = -10;
        moveForward(-6, 25);
        Sleep(1.0);

        turnRobot(ticketAngle);
        Sleep(1.0);
        moveForward(15, 25);
        Sleep(1.0);
        turnRobot(ticketAngle);
        Sleep(1.0);
        moveForward(35, 25);
        Sleep(1.0);
    } else if (backup == true) {
        moveForward(-distanceStart, 25);
        Sleep(1.0);
        turnRobot(ticketAngle);
        Sleep(1.0);
        moveForward(-ticketDistance, 25);
        Sleep(1.0);
        turnRobot(ticketAngle);
        Sleep(1.0);
        moveForward(-buttonDistance, 25);
        Sleep(1.0);
        moveForward(5, 25);
        Sleep(1.0);
        turnRobot(ticketAngle);
        Sleep(1.0);
        moveForward(-15, 25);
        Sleep(1.0);
        turnRobot(ticketAngle);
        Sleep(1.0);
        moveForward(-35, 25);
        Sleep(1.0);
        
    }
}