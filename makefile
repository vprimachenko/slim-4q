all:
	gcc main.cxx libhid/win32/device.cc libhid/win32/enumerator.cc libhid/win32/hid.cc -I libhid/include/    
