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
#include <FEHRCS.h>
#include <math.h>

#define WHEEL_RADIUS 1.25; 
#define PI 3.14159265358979;
#define COUNT_TOTAL 318;
#define WHEEL_DISTANCE 6.772;
#define NOTE .22058824

//declare all motors and pins
AnalogInputPin Cds(FEHIO::P1_6); 
FEHMotor RightMotor(FEHMotor::Motor3, 9.0); 
FEHMotor LeftMotor(FEHMotor::Motor1, 9.0); 
DigitalEncoder RightEncoder(FEHIO::P0_3);
DigitalEncoder LeftEncoder(FEHIO::P2_0);
FEHServo HandServo(FEHServo::Servo2);
FEHServo ArmServo(FEHServo::Servo7);
 

//moveForward function
void moveForward(float distance, int percent) {
    float distanceTraveled=0;
    RightEncoder.ResetCounts();
    if (distance > 0) {
        float actPercent = 11.5/(Battery.Voltage())*percent;
        RightMotor.SetPercent(actPercent);
        LeftMotor.SetPercent(actPercent); 
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
        float actPercent = 11.5/(Battery.Voltage())*percent;
        RightMotor.SetPercent(-actPercent);
        LeftMotor.SetPercent(-actPercent);
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
    /* If angle > 0, robot will turn counterclockwise, (direction of the unit circle)*/
    //convert angle from degrees to radians
    angle *= PI;
    angle /= 180;
    if (angle > 0) {
        //initialize values to 0
        float angleMoved = 0; 
        float distanceTraveled = 0; 
        //adjust for battery percentage
        float percent = 11.5/(Battery.Voltage())*35;
        //use only right motor
        RightMotor.SetPercent(percent);
        RightEncoder.ResetCounts();
        float distance;
        //check distance
        while (angleMoved < angle) {
            //update counts
            int counts = 0;
            counts = RightEncoder.Counts();
            //use distance to find angle moved
            distance = 2*PI;
            distance = distance * counts;
            distance = distance * WHEEL_RADIUS;
            distance = distance / COUNT_TOTAL;
            angleMoved = distance / WHEEL_DISTANCE;
        }
        RightMotor.Stop();
    } else {
        //find positive angle
        angle *= -1;
        //initialize values
        float angleMoved = 0; 
        float distanceTraveled = 0;
        //adjust for battery percentage
        float percent = 11.5/(Battery.Voltage())*35; 
        //set left motor, so robot turns clockwise
        LeftMotor.SetPercent(percent);
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

void moveHand(int angle) {
    HandServo.SetDegree(angle);
}

void moveArm(int angle) {
    ArmServo.SetDegree(angle);
}

void playSong() {
    //mamma mia
    Buzzer.Tone(FEHBuzzer::E4, NOTE );
    Buzzer.Tone(FEHBuzzer::D4, NOTE );
    Buzzer.Tone(FEHBuzzer::E4, NOTE );
    Buzzer.Tone(FEHBuzzer::D4, NOTE );
    Sleep(NOTE);
    //here I go again
    Buzzer.Tone(FEHBuzzer::D4, NOTE );
    Buzzer.Tone(FEHBuzzer::D4, NOTE );
    Buzzer.Tone(FEHBuzzer::E4, NOTE );
    Buzzer.Tone(FEHBuzzer::Fs4, NOTE );
    Buzzer.Tone(FEHBuzzer::E4, NOTE );
    Buzzer.Tone(FEHBuzzer::D4, NOTE );
    Sleep(NOTE);
}

void flipLever() {
    moveForward(4, 30);
    moveArm(82);
    moveForward(-4, 30);
    moveArm(95);
    playSong();
    moveForward(4,30);
    moveArm(80);
    moveForward(-4, 30);
    
}

int main() { 
    //set servo maxes and mins  
    ArmServo.SetMin(553);
    ArmServo.SetMax(2333);
    //HandServo.SetMin(500);
    //HandServo.SetMax(2333);

    ArmServo.SetDegree(45);
    HandServo.SetDegree(65);
    //playSong();

    //initialize RCS course
    RCS.InitializeTouchMenu("B1C1qRo4r");
 
    //move forward when light turns on
    float lightSense = Cds.Value();
    LCD.Clear();
    LCD.Write(lightSense);
    Sleep(1.0);



    //if run testing code - true
    //if running a run code - false
    bool test = false;
    int run = 6;

    if (test == true) {

        while (true) {
            float light = Cds.Value();
            LCD.WriteLine(light);
            Sleep(2.0);
            } 
    } else if(run == 6) {
    /*
    Entire Course Run!!!
    */
        
       //wait til light turns on
        while (lightSense > 2.9) {
            lightSense = Cds.Value();
        }
        //hit start button
        moveForward(-3.8, 35);
        moveForward(4, 35);

    /*Luggage Drop Off*/

        //move to luggage drop
        moveForward(9, 35);
        turnRobot(-45);
        moveForward(5, 35);

        //drop luggage
        moveHand(180);
        moveArm(55);
        Sleep(0.5);
        moveArm(30);
        Sleep(0.5);
        moveArm(55);
        Sleep(0.5);
        moveArm(28);

        //moves back to adjust on wall
        moveForward(-6, 35);
        turnRobot(20);
        moveForward(-5, 35);
        turnRobot(-20);
        moveForward(-15, 35);
        moveArm(0);
        moveHand(65);

    /*Ticket Kiosk*/

        //get up the ramp
        moveForward(8, 35);
        turnRobot(-45);
        moveForward(11, 35);
        turnRobot(45);
        moveForward(20, 35);

        //adjust against wall
        turnRobot(90);
        moveForward(-12, 35);

        //move to ticket light
        moveForward(5, 35);
        turnRobot(-45);

        //readjust against the wall
        moveForward(25, 35);
        Sleep(0.5);
        moveForward(-4, 35);

        //get color of the light
        float colorFactor = 12;
        float time = TimeNow()+2;
        //if colorFactor = 12 -> blue (default ig)
        //if colorFactor = 17 -> red
        while (time > TimeNow()) {
            lightSense = Cds.Value();
            if (lightSense > 2.5) {
                moveForward(-0.25, 25);
            } else if (lightSense < .75) {
                LCD.Clear(RED);
                LCD.Write(lightSense);
                colorFactor = 16.5;
            } else if (lightSense >= .75) {
                LCD.Clear(BLUE);
                LCD.Write(lightSense);
            }
        }

        Sleep(0.5);

        //move to correct light
        if (colorFactor == 12) {
            moveForward(-9, 35);
        } else if (colorFactor == 16.5) {
            moveForward(-12.5, 35);
        }
        turnRobot(-45);
        moveForward(colorFactor, 35);
        Sleep(1.0);
        moveForward(-colorFactor-3, 35);
        turnRobot(-90);

    /*Passport Stamp*/

        //adjust by backing into wall
        moveForward(-25, 35);

        //move to passport stamp
        moveForward(5, 35);
        Sleep(0.5);
        turnRobot(90);
        moveForward(3.15, 35);
        if (colorFactor == 16.5) {
            moveForward(4, 35);
        }
        turnRobot(-90);
        moveArm(85);
        Sleep(0.5);
        moveForward(5, 20);
        Sleep(2.0);

        //move hand to flip stamp
        Sleep(0.5);
        moveHand(5);
        Sleep(0.5);
        moveHand(100);

    /*Fuel Lever*/ 

        //move back to adjust against wall
        moveForward(-10, 30);
        moveArm(0);
        turnRobot(45);
        moveForward(-6, 30);
        turnRobot(-45);
        moveForward(-10, 30);

        //move down the ramp
        turnRobot(-90);
        moveForward(23, 40);

        //travel to correct lever and flip
        int lever = RCS.GetCorrectLever();
        if (lever == 3) {
            moveForward(3, 30);
            flipLever();
        } else if (lever == 2) {
            turnRobot(45);
            moveForward(0.5, 30);
            turnRobot(-45);
            moveForward(1.0, 30);
            flipLever();
        } else if (lever == 1) {
            turnRobot(45);
            moveForward(0.8, 30);
            turnRobot(-45);
            moveForward(1.0, 30);
            flipLever();
        }

    /*End Button*/

        //move back
        moveForward(-5, 30);
        turnRobot(90);
        moveForward(10+lever*3, 30);
        turnRobot(-45);
        //hit the end button
        moveForward(10, 30);
    }
}