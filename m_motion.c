/*
 * File:   m_motion.c
 * Author: lcgs
 *
 * Created on May 2, 2015, 1:41 PM
 */

#include <stdio.h>
#include <math.h>
#include "e_motors.h"
//Position and Direction cartesian
double posX, posY, dir;
//Calculated position an direction
double cPosX, cPosY, cDir;

//Straight line, goal
int odometerL, odometerR, goalL, goalR;
unsigned char reached, checked, lastAction, turned;
int stepError = 10;
int angleError = 2;

int lastR, lastL;

//Initializes all of the variables to 0.
void init_steps(){
    e_set_steps_left(0);
    e_set_steps_right(0);
    e_set_speed_left(0);
    e_set_speed_right(0);
    odometerL = 0;
    odometerR = 0;
    goalL = 0;
    goalR = 0;
    posX = 0.0;
    posY = 0.0;
    dir = 0.0;
    cPosX = 0.0;
    cPosY = 0.0;
    cDir = 0.0;
    checked = 0;
    reached = 0;
    turned = 0;
    lastAction = 0;
    lastR = 0;
    lastL = 0;

}

//Updates the current position based on the previous order
void updatePosition(unsigned char theLastAction){
    //If last action was dash
    if (theLastAction == 1){

    }else if(theLastAction == 2){// If last action was turn

    }

    lastL = e_get_steps_left();
    lastR = e_get_steps_right();

}

//Calculates the next position based on the current order
void calculatePosition(unsigned char theAction){
    //If last action was dash
    if (theAction == 1){

    }else if(theAction == 2){// If last action was turn

    }

    lastL = e_get_steps_left();
    lastR = e_get_steps_right();
}

void newTarget(){
    checked = 0;
    reached = 0;
    turned = 0;
    updatePosition(lastAction);
    lastAction = 0;
}

//Makes go forward and backwards at speed -1000 to 1000
void dash(int speed){
    updatePosition(lastAction);
    if (speed > 1000){
        speed = 1000;
    } else if (speed < -1000){
        speed = -1000;
    }
    e_set_speed_left(speed);
    e_set_speed_right(speed);

    calculatePosition(1);
    lastAction = 1;
}

//Updates the variable that contains the total number of steps travelled
void updateOdometer(){
    int valueL = e_get_steps_left();
    int valueR = e_get_steps_right();
    
    odometerL = odometerL + fabs(valueL - odometerL);
    odometerR = odometerR + fabs(valueR - odometerR);
}



//Makes follow a distance in mm using the steps-ratio relation
//1000 steps completes a revolution, wheel diameter 41 mm
//1000 steps = 41*pi mm ==> 1 step = 0.1288052988 or 1 mm = 7.7636557604 steps
//Note: Only works sequentially, and only checks for left wheel
unsigned char move(int distance, int speed){
    updateOdometer();
    if (!checked){
        checked = 1;
        double value = (double)distance *  7.7636557604;
        double intpart;
        //Rounding to integer
        if ((modf(value,&intpart)) < 0.5){
            goalL = (int)intpart;
        } else{
            goalL = (int)ceil(value);
        }
        goalL = e_get_steps_left() + goalL;
    }
    int currentError = goalL - e_get_steps_left();
    if ((currentError < stepError)&&(currentError > -stepError)){
        reached = 1;
        dash(0);
    } else if(currentError < -stepError){
        reached = 0;
        dash(-speed);
    } else {
        dash(speed);
        reached = 0;
    }
    return reached;
}

void turn(int speedL, int speedR){
    updatePosition(lastAction);

    if (speedR > 1000){
        speedR = 100;
    } else if (speedR < -1000){
        speedR = -1000;
    }
    if (speedL > 1000){
        speedL = 100;
    } else if (speedL < -1000){
        speedL = -1000;
    }

    e_set_speed_left(speedL);
    e_set_speed_right(speedR);
    
    calculatePosition(2);
    lastAction = 2;
}


//Makes the epuck rotate the amount of indicated degrees using the formula:
//1 step =  0.278490566 degrees or 3.5907859083 steps = 1 degree
unsigned char rotate(double angle, int speed){

    if (!checked){
        checked = 1;
        double value = angle *  3.5907859083;
        double intpart;
        //Rounding to integer
        if ((modf(value,&intpart)) < 0.5){
            goalL = (int)intpart;
        } else{
            goalL = (int)ceil(value);
        }
        goalL = e_get_steps_left() + goalL;
        goalR = e_get_steps_right() - goalL;
    }

    //Be careful, only checks for left wheel, assume that is rotating
    //over itself
    int currentError = goalL - e_get_steps_left();
    if ((currentError < stepError)&&(currentError > -stepError)){
        turned = 1;
        turn(0,0);
    } else if(currentError < -stepError){
        turned = 0;
        turn(-speed,speed);
    } else {
        turn(speed,-speed);
        turned = 0;
    }

    return turned;

}

//Make epuck move at variable speed increasing accuracy
unsigned char fineMove(int distance){
    updateOdometer();
    if (!checked){
        checked = 1;
        double value = (double)distance *  7.7636557604;
        double intpart;
        //Rounding to integer
        if ((modf(value,&intpart)) < 0.5){
            goalL = (int)intpart;
        } else{
            goalL = (int)ceil(value);
        }
        goalL = e_get_steps_left() + goalL;
    }
    int currentError = goalL - e_get_steps_left();

    int speed = 0;
    if (currentError > 100){
        speed = 1000;
    } else if(currentError < -100){
        speed = 1000;
    }else {
        speed = 100;
    }


    if ((currentError < stepError)&&(currentError > -stepError)){
        reached = 1;
        dash(0);
    } else if(currentError < -stepError){
        reached = 0;
        dash(-speed);
    } else {
        dash(speed);
        reached = 0;
    }
    return reached;
}


//Make epuck turn at variable speed increasing accuracy
unsigned char fineRotate(double angle){

    if (!checked){
        checked = 1;
        double value = angle *  3.5907859083;
        double intpart;
        //Rounding to integer
        if ((modf(value,&intpart)) < 0.5){
            goalL = (int)intpart;
        } else{
            goalL = (int)ceil(value);
        }
        goalL = e_get_steps_left() + goalL;
        goalR = e_get_steps_right() - goalL;
    }

    //Be careful, only checks for left wheel, assume that is rotating
    //over itself
    int currentError = goalL - e_get_steps_left();
    int speed = 0;
    if (currentError > 100){
        speed = 1000;
    } else if(currentError < -100){
        speed = 1000;
    }else {
        speed = fabs(currentError) + 10;
    }

    if ((currentError < angleError)&&(currentError > -angleError)){
        turned = 1;
        turn(0,0);
    } else if(currentError < -angleError){
        turned = 0;
        turn(-speed,speed);
    } else {
        turn(speed,-speed);
        turned = 0;
    }

    return turned;

}

//Left hand rule turn, turn to left or right until the number of steps
//of left wheels are reached.
void turnStep(int maxSpd, int goal, int error){
    if (maxSpd > 1000){
        maxSpd = 1000;
    } else if(maxSpd <0){
        maxSpd = 0;
    }

    int currentError = goal - ((e_get_steps_left() - e_get_steps_right()) / 2);
    int thrUp = error;
    int thrDn = -error;

    if ((currentError < thrUp)&&(currentError > thrDn)){
        e_set_speed_left(0);
        e_set_speed_right(0);
    } else if (currentError > thrUp){
        e_set_speed_left(maxSpd);
        e_set_speed_right(-maxSpd);
    } else {
        e_set_speed_left(-maxSpd);
        e_set_speed_right(maxSpd);
    }
}





