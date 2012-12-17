/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 KBEngine.

KBEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KBEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with KBEngine.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef __KBENGINE_PY_WATCHER_H__
#define __KBENGINE_PY_WATCHER_H__
#include "scriptobject.hpp"
#include "helper/watcher.hpp"

namespace KBEngine{ namespace script{

class Script;

/*
	ʹwatcher�ܹ����ӵ�py�ű��е�����
*/
template <class T>
class PyWatcherObject : public WatcherObject
{
public:
	PyWatcherObject(std::string path, PyObject* pyCallable):
	  WatcherObject(path),
	  pyCallable_(pyCallable)
	{
	}

	virtual ~PyWatcherObject()
	{
		Py_DECREF(pyCallable_);
	}

	void addToInitStream(MemoryStream* s){
		(*s) << path() << name() << id_ << type<T>() << getVal();
	};

	void addToStream(MemoryStream* s){
		(*s) << id_ << getVal();
	};

	T getVal()
	{
		T v;

		PyObject* pyObj1 = PyObject_CallFunction(pyCallable_, const_cast<char*>(""));
		if(!pyObj1)
		{
			PyErr_Format(PyExc_Exception, "PyWatcherObject::addToStream: callFunction is error! path=%s name=%s.\n",
				path(), name());
			PyErr_PrintEx(0);
		}
		else
		{
			readVal(pyObj1, v);
			SCRIPT_ERROR_CHECK();
			Py_DECREF(pyObj1);
		}

		return v;
	}

	void readVal(PyObject* pyVal, T& v)
	{
	}
private:
	PyObject* pyCallable_;
};

template <>
void PyWatcherObject<uint8>::readVal(PyObject* pyVal, uint8& v)
{
	v = (uint8)PyLong_AsLong(pyVal);
}

template <>
void PyWatcherObject<uint16>::readVal(PyObject* pyVal, uint16& v)
{
	v = (uint16)PyLong_AsLong(pyVal);
}

template <>
void PyWatcherObject<uint32>::readVal(PyObject* pyVal, uint32& v)
{
	v = (uint32)PyLong_AsLong(pyVal);
}

template <>
void PyWatcherObject<uint64>::readVal(PyObject* pyVal, uint64& v)
{
	v = (uint64)PyLong_AsLong(pyVal);
}

template <>
void PyWatcherObject<int8>::readVal(PyObject* pyVal, int8& v)
{
	v = (int8)PyLong_AsLong(pyVal);
}

template <>
void PyWatcherObject<int16>::readVal(PyObject* pyVal, int16& v)
{
	v = (int16)PyLong_AsLong(pyVal);
}

template <>
void PyWatcherObject<int32>::readVal(PyObject* pyVal, int32& v)
{
	v = (int32)PyLong_AsLong(pyVal);
}

template <>
void PyWatcherObject<int64>::readVal(PyObject* pyVal, int64& v)
{
	v = (int64)PyLong_AsLong(pyVal);
}

template <>
void PyWatcherObject<bool>::readVal(PyObject* pyVal, bool& v)
{
	v = PyLong_AsLong(pyVal) > 0;
}

template <>
void PyWatcherObject<float>::readVal(PyObject* pyVal, float& v)
{
	v = (float)PyFloat_AsDouble(pyVal);
}

template <>
void PyWatcherObject<double>::readVal(PyObject* pyVal, double& v)
{
	v = PyFloat_AsDouble(pyVal);
}

template <>
void PyWatcherObject<std::string>::readVal(PyObject* pyVal, std::string& v)
{
	wchar_t* wstr = PyUnicode_AsWideCharString(pyVal, NULL);					
	char* p = strutil::wchar2char(wstr);	
	v = p;
	PyMem_Free(wstr);	
	free(p);
}

bool initializePyWatcher(Script* pScript);

}
}
#endif