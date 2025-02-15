#include "Variables.h"
#include "AnalogIO.h"
#include "Calibrate.h"
#include "Display.h"
#include <EEPROM.h>

const int calibrationEEPROMAddress = 0;

unsigned int octave_values[9] = { 0,   500,  1000, 1500, 2000, 2500, 3000, 3500, 4000 };
//int calibration_values[9]   = { 0,   0,    0,    0,    0,    0,    0,    0,    0    };
int calibration_values[9]     = { 0,   -4,   -6,   -12,  -15,  -17,  -18,  -21,  -22  }; //programming the chip overwrites the EEPROM, so storing good default here for convenience


void initializeCalibrationMode() {
	control_mode = CALIBRATE_MODE;
	display_param = CALIBRATION_PARAM;
	readCalibrationValues();
	selected_step = 0;
	updateCalibration();
}

void updateCalibration() {
	int i = 0;
	while (i < 16) {
		led_matrix[i] = 0;
		i++;
	}
	led_matrix[selected_step] = 1;
	num_display = calibration_values[selected_step];
	setCalibratedOutput(selected_step * 12);
	setDisplayNum();
}

int setCalibratedOutput(double pitch) {
	if (pitch > 96) {
		pitch = 96;
	}
	int octave1 = pitch / 12; //nearest C at or below pitch
	int octave2 = octave1 + ((int(pitch) % 12) > 0 ? 1 : 0); //nearest C above pitch, or equal if first is C
	double point1 = octave_values[octave1] + calibration_values[octave1];
	double point2 = octave_values[octave2] + calibration_values[octave2];

	unsigned int calibratedPitch;
	if (octave2 == octave1) {
		calibratedPitch = point1;
	} else {
		calibratedPitch = point1 + double(pitch - octave1 * 12.0) * double(point2 - point1) / double(12);
		//use display for output
		//num_display = calibratedPitch / 10;
		//setDisplayNum();
	}

	setOutput(0, GAIN_2, 1, calibratedPitch);
	return calibratedPitch;
}

void incrementCalibration(int amt) {
	if (abs(calibration_values[selected_step] + amt) < 100) { //calibration values stored as 0.0 - 2.0 but displayed as -99 to +99
		calibration_values[selected_step] += amt;
		updateCalibration();
	} 
}

void readCalibrationValues() {
	for (int i = 0; i < 9; i++) {
		calibration_values[i] = EEPROM.read(i) - 100; //convert 0-255 to +/-99
		if (abs(calibration_values[i]) > 99) { //discard garbage
			calibration_values[i] = 0;
		}
	}
}

void writeCalibrationValues() {
	for (int i = 0; i < 9; i++) {
		EEPROM.update(i, calibration_values[i] + 100); //convert +/-99 to 0-255
	}
}