//
// Created by Valerij Primachenko on 20-07-01.
//
#include "plugins.h"

#include <iostream>
#include <filesystem>
#define HAVE_SNPRINTF
#include <Python.h>
#include <thread>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "deps/magic_enum.hpp"
#include "resource.h"


typedef void (*Py_Initialize_t)();

std::shared_ptr<DAS::Keyboard> py_keyboard;

static PyMethodDef TickMethods[] = {
		{"log",       [](PyObject *self, PyObject *args) -> PyObject * {
			const char *command;
			int sts;
			if (!PyArg_ParseTuple(args, "s", &command))
				return NULL;
			std::cout << command << std::flush;
			Py_INCREF(Py_None);
			return Py_None;
		}, METH_VARARGS, "logs some text."},
		{"set_color", [](PyObject *self, PyObject *args) -> PyObject * {
			const char *pid;
			const char *_zone_id;
			const char *_effect;
			unsigned char r;
			unsigned char g;
			unsigned char b;
			int sts;
			if (!PyArg_ParseTuple(args, "sssbbb", &pid, &_zone_id, &_effect, &r, &g, &b))
				return NULL;
//			printf("%s %s %02x%02x%02x\n", command1, command2, r, g, b);

			auto zone_id = magic_enum::enum_cast<DAS::Key>(_zone_id);
			auto effect = magic_enum::enum_cast<DAS::Effect>(_effect);

			if (zone_id.has_value() && effect.has_value()) {
				py_keyboard->add_override(pid, zone_id.value(), {effect.value(), r, g, b});
			}

			Py_INCREF(Py_None);
			return Py_None;
		}, METH_VARARGS, "sets color of a key"},
		{"reset_color", [](PyObject *self, PyObject *args) -> PyObject * {
			const char *pid;

			int sts;
			if (!PyArg_ParseTuple(args, "s", &pid))
				return NULL;
//			printf("%s %s %02x%02x%02x\n", command1, command2, r, g, b);

			py_keyboard->remove_override(pid);

			Py_INCREF(Py_None);
			return Py_None;
		}, METH_VARARGS, "sets color of a key"},
		{NULL, NULL, 0, NULL}
};
static PyModuleDef Tickmodule = {
		.m_base = PyModuleDef_HEAD_INIT,
		.m_name = "keyboard_api",
		.m_doc = NULL,//Tick_doc
		.m_size = -1,
		.m_methods = TickMethods,
		.m_slots = NULL,
		.m_traverse = NULL,
		.m_clear = NULL,
		.m_free = NULL
};


void run_python() {
	Py_DontWriteBytecodeFlag = 1;
//	Py_NoSiteFlag = 1;
//	Py_NoUserSiteDirectory = 1;
//	Py_IgnoreEnvironmentFlag = 1;
//	Py_VerboseFlag = 1;

	Py_SetPath((
		std::filesystem::absolute("./python38.zip").wstring()
		+L";"+
		std::filesystem::absolute(".").wstring()
	).c_str());

	PyImport_AppendInittab("keyboard_api",[](){
		return PyModule_Create(&Tickmodule);
	});
	Py_SetProgramName(L"DAS");
	Py_Initialize();

	HRSRC myResource = ::FindResource(NULL, MAKEINTRESOURCE(DAS_PY), RT_RCDATA);
	HGLOBAL myResourceData = ::LoadResource(NULL, myResource);
	void* pMyBinaryData = ::LockResource(myResourceData);

	auto obj = Py_CompileString(static_cast<char *>(pMyBinaryData), "das.py", Py_file_input);
	auto pModule = PyImport_ExecCodeModule("das", obj);


//	auto pName = PyUnicode_FromString("das");
//	auto pModule = PyImport_Import(pName);

	if (PyErr_Occurred()) { PyErr_Print();}
	Py_DECREF(pModule);
	Py_DECREF(obj);
}

std::thread python_thread;
void start_python(std::shared_ptr<DAS::Keyboard> keyboard) {
	py_keyboard = keyboard;
	python_thread = std::thread ([&] {
		run_python();
	});
}

void stop_python(){
	Py_Finalize();
}
