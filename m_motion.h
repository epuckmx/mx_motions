/* 
 * File:   m_motion.h
 * Author: lcgs
 *
 * Created on May 2, 2015, 1:41 PM
 */

#ifndef M_MOTION_H
#define	M_MOTION_H

extern double posX, posY, dir;
extern int odometerL, odometerR, goalL, goalR;

void init_steps();
void dash(int speed);
void turn(int speedL, int speedR);
void newTarget();
void updateOdometer();
unsigned char move(int distance, int speed);
unsigned char rotate(double angle, int speed);
unsigned char fineMove(int distance);
unsigned char fineRotate(double angle);



#endif	/* M_MOTION_H */

