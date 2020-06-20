//
// Created by le0n on 20-06-18.
//

#ifndef DAS_CONSTANTS_H
#define DAS_CONSTANTS_H

#include <vector>

enum class Keys : unsigned char {
	LCtrl = 0,
	LShift = 1,
	CapsLock = 2,
	Tab = 3,
	Tilde = 4,
	Esc = 5,

	LWin = 6,
	A = 8,
	Q = 9,
	_1 = 10,

	LAlt = 12,
	Z = 13,
	S = 14,
	W = 15,
	_2 = 16,
	F1 = 17,

	X = 19,
	D = 20,
	E = 21,
	_3 = 22,
	F2 = 23,

	C = 25,
	F = 26,
	R = 27,
	_4 = 28,
	F3 = 29,

	V = 31,
	G = 32,
	T = 33,
	_5 = 34,
	F4 = 35,

	Space = 36,
	B = 37,
	H = 38,
	Y = 39,
	_6 = 40,
	F5 = 41,

	N = 43,
	J = 44,
	U = 45,
	_7 = 46,
	F6 = 47,

	M = 49,
	K = 50,
	I = 51,
	_8 = 52,
	F7 = 53,

	Comma= 55,
	L = 56,
	O = 57,
	_9 = 58,
	F8 = 59,

	RAlt = 60,
	Dot = 61,
	Semicolon = 62,
	P = 63,
	_0 = 64,
	F9 = 65,

	RWin = 66,
	Slash = 67,
	Quote = 68,
	BracketOpen = 69,
	Minus = 70,
	F10 = 71,

	ContextMenu = 72,
	BracketClose = 75,
	Equals = 76,
	F11 = 77,

	RCtrl = 78,
	RShift = 79,
	Enter = 80,
	Backslash = 81,
	Backspace = 82,
	F12 = 83,

	Left = 84,
	Del = 87,
	Ins = 88,
	PrtSc = 89,

	Down = 90,
	Up = 91,
	End = 93,
	Home = 94,
	ScrLK = 95,

	Right = 96,
	PgDn = 99,
	PgUp = 100,
	Pause = 101,

	Num0 = 102,
	Num1 = 103,
	Num4 = 104,
	Num7 = 105,
	NumLock = 106,

	Num2 = 109,
	Num5 = 110,
	Num8 = 111,
	NumSlash = 112,

	NumDot = 114,
	Num3 = 115,
	Num6 = 116,
	Num9 = 117,
	NumTimes = 118,

	NumEnter = 122,
	NumPlus = 123,
	NumMinus = 124,

	Sleep = 126,
	Brightness = 127,
	PlayPause = 128,
	Next = 129,

	TheQ = 130,
	Dummy = 255
};

//std::vector<uint8_t> OK_RESPONSE{0x03, 0x78, 0x00, 0x96, 0x00, 0x00, 0x00};
//std::vector<uint8_t> NOT_READY{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//std::vector<uint8_t> FLUSH{0x78, 0x0A};


#endif //DAS_CONSTANTS_H
