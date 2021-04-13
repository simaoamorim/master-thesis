#ifndef __DFR0592_ADDRS
#define	__DFR0592_ADDRS

#define	_CONTROL_MODE_DC_MOTOR		0x00
#define	_CONTROL_MODE_STEPPER		0x01

#define	_REG_SLAVE_ADDR			0x00
#define	_REG_PID			0x01
#define	_REG_PVD			0x02
#define	_REG_CTRL_MODE			0x03
#define	_REG_ENCODER1_EN		0x04
#define	_REG_ENCODER1_SPPED		0x05
#define	_REG_ENCODER1_REDUCTION_RATIO	0x07
#define	_REG_ENCODER2_EN		0x09
#define	_REG_ENCODER2_SPEED		0x0a
#define	_REG_ENCODER2_REDUCTION_RATIO	0x0c
#define	_REG_MOTOR_PWM			0x0e
#define	_REG_MOTOR1_ORIENTATION		0x0f
#define	_REG_MOTOR1_SPEED		0x10
#define	_REG_MOTOR2_ORIENTATION		0x12
#define	_REG_MOTOR2_SPEED		0x13

#define	_REG_DEF_PID			0xdf
#define	_REG_DEF_VID			0x10

#define	CW				0x01
#define	CCW				0x02
#define	STOP				0x05
#define	ALL				0xffffffff

#endif