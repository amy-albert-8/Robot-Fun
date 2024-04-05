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
FEHServo HandServo(FEHServo::Servo3);
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
        
    } else if (run == 1) {
        /*Checkpoint 2: Up to the light, hit the ticket and go back down*/
        //wait til light turns on
        while (lightSense > 2.9) {
            lightSense = Cds.Value();
        }
        
        //move out of box and up ramp

        turnRobot(-30);
        //move forward out of box
        moveForward(1.5, 25);
        //readjust angle
        turnRobot(-30);
        //move forward up the ramp
        moveForward(32, 25);
        Sleep(1.0);


        //move to ticket light

        //set angle to pi/4
        turnRobot(45);
        Sleep(1.0);

        //move forward to light
        moveForward(19.5, 25);
        Sleep(1.0);


        //check the light color

        float colorFactor = 7;
        //if colorFactor = 7 -> blue (default ig)
        //if colorFactor = 11 -> red
        Sleep (1.0);
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
        moveForward(-colorFactor-3, 25);
        Sleep(1.0);
        turnRobot(-45);
        //find cos(theta)
        float forward = sqrt(2)/(2.2);
        forward *= colorFactor;
        //forward = colorFactor*cos(theta)

        /*The assumption is that theta is 45 degrees, may need to update 
        forward if the angle is found to be different.*/

        //hopefully hitting the button
        Sleep(1.0);
        moveForward(forward+0.2, 25);
        //backing up from the button
        Sleep(1.0);
        moveForward(-forward, 25);
        Sleep(1.0);
        //turn 45 degrees
        turnRobot(45);
        //move back from the button
        moveForward(forward+0.5, 25);

        //turn and go down ramp
        turnRobot(135);
        Sleep(1.0);
        moveForward(35, 25);
    } else if (run == 2) {

        //wait til light turns on
        while (lightSense > 2.9) {
            lightSense = Cds.Value();
        }
        //hit start button
        moveForward(-4, 30);
        moveForward(4, 30);
        
        //move to levers
        turnRobot(-65);
        Sleep(1.0);
        moveForward(3, 25);
        Sleep(1.0);
        turnRobot(15);

        //moveForward
        Sleep(1.0);
        turnRobot(90);

        //check which lever
        float lever = 0;
        int leverMultiply = RCS.GetCorrectLever()*3;
        lever += leverMultiply;

        //move to that lever
        moveForward(10 + lever, 25);
        turnRobot(90);

        /*If too close to or too far away from the lever, back up or
        drive forward here.*/
        //move to lever

        //TODO: flip lever down
        moveForward(-6, 25);
        Sleep(5.0);
        //TODO: move arm to below lever
        moveForward(6,25);
        //TODO: flip lever up

    } else if (run == 3) {
        /*Attempt to get full credit on Checkpoint 3
        without getting the correct lever, will only hit lever A*/

        //wait til light turns on
        while (lightSense > 2.9) {
            lightSense = Cds.Value();
        }
        
       //move to levers
       turnRobot(-65);
       Sleep(1.0);
       moveForward(3, 25);
       Sleep(1.0);
       turnRobot(20);
       moveForward(-5, 25);
       Sleep(1.0);
       turnRobot(90);

       moveForward(4.5, 25);
       Sleep(1.0);
       //flip lever down
       moveArm(85);
       Sleep(1.0);
       moveForward(-4, 25);
       Sleep(1.0);
       //move arm to below lever position
       moveArm(95);
       Sleep(5.0);
       moveForward(4, 25);
       //flip lever up
       moveArm(65);
    } else if(run == 4) {
        //wait til light turns on
        while (lightSense > 2.9) {
            lightSense = Cds.Value();
        }
        
        //readjust out of box
        turnRobot(-65);
        Sleep(1.0);
        moveForward(4, 35);
        Sleep(1.0);
        turnRobot(15);

        //up the ramp + turn
        moveForward(25, 35);
        turnRobot(90);
        //back up into wall to restraighten
        moveForward(-13, 35);
        //move past luggage drop + turn towards ticket
        moveForward(14, 35);
        turnRobot(-90);
        //move towards ticket + turn towards passport
        moveForward(8.5, 35);
        turnRobot(-96);
        //set arm down
        moveArm(85);
        Sleep (1.0);

        moveForward(0.8,25);

        Sleep(1.0);
        moveHand(5);

        Sleep(1.0);
        moveHand(100);

        Sleep(1.0);
        moveForward(-6, 35);
        moveArm(0);
    } else if (run == 5) {

        //wait til light turns on
        while (lightSense > 2.9) {
            lightSense = Cds.Value();
        }  

         //hit start button
        moveForward(-4, 30);
        moveForward(4, 30);

        //move to luggage drop
        moveForward(9.5, 30);
        turnRobot(-45);
        moveForward(5, 30);


        moveHand(180);
        moveArm(55);
        Sleep(0.5);
        moveArm(30);

        Sleep(1.0);
        //moves back to hit button
        moveForward(-8,30);
        turnRobot(45);
        moveForward(-19, 35);
        
    } else if(run == 6) {
        /*
        Entire Course Run!!!
        */
        

       //wait til light turns on
        while (lightSense > 2.9) {
            lightSense = Cds.Value();
        }
        //hit start button
        moveForward(-4, 30);
        moveForward(4, 30);

    /*Luggage Drop Off*/

        //move to luggage drop
        moveForward(9, 30);
        turnRobot(-45);
        moveForward(5, 30);

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
        moveForward(-6, 30);
        turnRobot(20);
        moveForward(-5,30);
        turnRobot(-20);
        moveForward(-15, 30);
        moveArm(0);
        moveHand(65);

    /*Ticket Kiosk*/

        //get up the ramp
        moveForward(9, 30);
        turnRobot(-45);
        moveForward(11, 30);
        turnRobot(45);
        moveForward(20, 30);

        //adjust against wall
        turnRobot(90);
        moveForward(-10, 30);

        //move to ticket light
        moveForward(5, 30);
        turnRobot(-45);

        //readjust against the wall
        moveForward(25, 30);
        Sleep(0.5);
        moveForward(-4, 30);

        //get color of the light
        float colorFactor = 12;
        float time = TimeNow()+2;
        //if colorFactor = 12 -> blue (default ig)
        //if colorFactor = 17 -> red
        while (time > TimeNow()) {
            lightSense = Cds.Value();
            if (lightSense > 2.5) {
                moveForward(-0.25, 25);
            } else if (lightSense < .7) {
                LCD.Clear(RED);
                LCD.Write(lightSense);
                colorFactor = 17;
            } else if (lightSense >= .7) {
                LCD.Clear(BLUE);
                LCD.Write(lightSense);
            }
        }

        Sleep(0.5);

        //move to correct light
        if (colorFactor == 12) {
            moveForward(-9, 30);
        } else if (colorFactor == 17) {
            moveForward(-12, 30);
        }
        turnRobot(-45);
        moveForward(colorFactor, 30);
        Sleep(1.0);
        moveForward(-colorFactor-3, 30);
        turnRobot(-90);

        /*Passport Stamp*/

        //adjust by backing into wall
        moveForward(-25, 30);

        //move to passport stamp
        moveForward(5, 30);
        Sleep(0.5);
        turnRobot(90);
        moveForward(3.15, 30);
        turnRobot(-90);
        moveArm(85);
        Sleep(0.5);
        moveForward(4.5, 20);
        Sleep(2.0);

        //move hand to flip stamp
        //Sleep(0.5);
        //moveHand(5);
        //Sleep(0.5);
        //moveHand(100);

        /*Fuel Lever*/ 

        //move back to adjust against wall
        moveForward(-10, 30);
        moveArm(0);
        turnRobot(45);
        moveForward(-6, 30);
        turnRobot(45);
        moveForward(-20, 30);

        //move to correct lever
        moveForward(4, 30);
        turnRobot(-90);
        moveForward(15, 30);
        int lever = RCS.GetCorrectLever();
        if (lever == 3) {
            moveForward(5, 30);
            flipLever();
        } else if (lever == 2) {
            turnRobot(90);
            moveForward(3, 30);
            turnRobot(-90);
            moveForward(4, 30);
            flipLever();
        } else if (lever == 1) {
            turnRobot(90);
            moveForward(6, 30);
            turnRobot(-90);
            moveForward(4, 30);
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