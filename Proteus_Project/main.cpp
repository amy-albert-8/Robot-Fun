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
#define FORTY_FIVE_DEGREES1 0.785;
#define FORTY_FIVE_DEGREES2 -0.785;



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
        RightMotor.SetPercent(percent+3);
        LeftMotor.SetPercent(percent+5.5); 
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
        LeftMotor.SetPercent(-percent-3);
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
        RightMotor.SetPercent(35);
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
        LeftMotor.SetPercent(35);
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
        lightSense = Cds.Value();
        LCD.Clear();
        LCD.Write(lightSense);
        if (lightSense < 1) {
            LCD.Clear(RED);
            LCD.Write(lightSense);
        } else if (lightSense > 1) {
            LCD.Clear(BLUE);
            LCD.Write(lightSense);
        }


    } else if (backup == false) {
        //wait til light turns on
        while (lightSense > 2.9) {
            lightSense = Cds.Value();
        }
        
        //move out of box and up ramp

        //set angle to -pi/6
        float angle = -PI;
        angle /= 6;
        turnRobot(angle);
        //move forward out of box
        moveForward(1, 25);
        //readjust angle
        turnRobot(-angle+0.1);
        //move forward up the ramp
        moveForward(31, 25);
        Sleep(1.0);

        //move to ticket light

        //set angle to pi/4
        angle = PI;
        angle /= 4;
        turnRobot(angle);
        Sleep(1.0);
        //move forward to light
        moveForward(20, 25);


        //check the light color

        float colorFactor = 7;
        //if colorFactor = 7 -> blue (default ig)
        //if colorFactor = 11 -> red
        lightSense = Cds.Value();
        if (lightSense < 1) {
            LCD.Clear(RED);
            LCD.Write(lightSense);
            colorFactor = 11;
        } else if (lightSense > 1) {
            LCD.Clear(BLUE);
            LCD.Write(lightSense);
        }

        //back up, turn, and move to hit correct ticket
        moveForward(-colorFactor, 25);
        Sleep(1.0);
        turnRobot(-0.785);
        //find cos(theta)
        float forward = sqrt(2)/2;
        forward *= colorFactor;
        //forward = colorFactor*cos(theta)

        /*The assumption is that theta is 45 degrees, may need to update 
        forward if the angle is found to be different.*/

        //hopefully hitting the button
        Sleep(1.0);
        moveForward(forward, 25);
        //backing up from the button
        Sleep(1.0);
        moveForward(-forward, 25);
        Sleep(1.0);
        //turn 45 degrees
        turnRobot(0.785);
        //move back from the button
        moveForward(forward, 25);

        //turn and go down ramp
        turnRobot(1.57);
        Sleep(1.0);
        moveForward(20, 25);
    }
}