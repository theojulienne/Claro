#include <Python.h>

#include <claro/base.h>
#include <claro/graphics.h>

#ifndef NO_CAIRO
#include <pycairo/pycairo.h>
static Pycairo_CAPI_t *Pycairo_CAPI;
#endif

typedef struct
{
    PyObject_HEAD
    bounds_t * bounds;
} PyClaroBounds;

typedef struct
{
    PyObject_HEAD
    object_t * object;
} PyClaroObject;

typedef struct
{
    PyObject_HEAD
    event_t * event;
} PyClaroEvent;

typedef struct 
{
    char type;
    void * val;
} _event_arg_t;

#ifdef image_type
#undef image_type
#endif

static void Object_wrapinit(PyObject * wrapper, object_t * obj);

static claro_hashtable_t * class_table = NULL;

static PyObject * PyClaroObject_wrap(object_t *obj);

static PyTypeObject PyClaroEventType;
static PyTypeObject PyClaroBoundsType;
static PyTypeObject PyClaroObjectType;
static PyTypeObject PyClaroImageType;

#define DEFINE_WIDGET(type_name, get_type_func, methods, getseters) \
static PyTypeObject type_name##_Type = {  \
    PyObject_HEAD_INIT(NULL) \
    0, \
    "claro."#type_name, \ 
    sizeof(PyClaroObject), \
    0, \
    (destructor)PyClaroObject_dealloc, \ 
    0, \
    0, \
    0, \                      
    0, \                    
    0, \                    
    0, \                   
    0, \                  
    0,    \
    0,     \                   
    0,      \                   
    0,       \                 
    0,        \                 
    0,         \                
    0,         \             
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, \
    #type_name,           \
    0,		             \  
    0,		              \ 
    0,		               \
    0,		               \
    0,		               \
    0,		               \
    methods,                         \
    0,                         \
    getseters,           \
    0,                         \
    0,                         \
    0,                         \
    0,                         \
    0,                         \
    (initproc) type_name##_init,     \
    0,                                 \
    PyClaroObject_new,                 \
};	\
type_name##_Type.tp_base = &PyClaroWidgetType; 	\
if(PyType_Ready(& type_name##_Type) < 0)	\
        return;	\
claro_hashtable_insert(class_table, get_type_func(), & type_name##_Type, FALSE); \
Py_INCREF(& type_name##_Type); \
PyModule_AddObject(m, #type_name, (PyObject *)& type_name##_Type); \

// we can actually bounds to the class_table object with a fake claro type..
static PyObject *
PyClaroBounds_wrap(bounds_t * bounds)
{
    PyClaroBounds * self;

    if(!bounds)
        Py_RETURN_NONE;

    self = PyObject_New(PyClaroBounds, &PyClaroBoundsType);

    if(!self)
    {
        PyErr_SetString(PyExc_RuntimeError, "Couldn't wrap bounds object");        
        return NULL;
    }

    //claro_bounds_ref(bounds);
    self->bounds = bounds;
    return self;
}
    
static void
PyClaroBounds_dealloc(PyClaroBounds * self)
{
    //unref bounds
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
PyClaroBounds_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    return type->tp_alloc(type, 0);
}

static int
PyClaroBounds_init(PyClaroBounds *self, PyObject *args, PyObject *kwds)
{
    int x, y, w, h;

    if(!PyArg_ParseTuple(args, "iiii", &x, &y, &w, &h))
        return -1;
    
    //this should be ref'ed again, because the widget should own a ref too
    self->bounds = new_bounds(x, y, w, h);
    
    return 0;
}

static PyObject *
PyClaroBounds_getx(PyClaroBounds *self, void *closure)
{
    return PyInt_FromLong((long)self->bounds->x);
}

static int
PyClaroBounds_setx(PyClaroBounds *self, PyObject *value, void *closure)
{
    if (!PyInt_Check(value)) 
    {
        PyErr_SetString(PyExc_TypeError, 
                    "The X attribute must be an integer");
        return -1;
    }

    self->bounds->x = (int)PyInt_AS_LONG(value);
    
    return 0;
}

static PyObject *
PyClaroBounds_gety(PyClaroBounds *self, void *closure)
{
    return PyInt_FromLong((long)self->bounds->y);
}

static int
PyClaroBounds_sety(PyClaroBounds *self, PyObject *value, void *closure)
{
    if (!PyInt_Check(value)) 
    {
        PyErr_SetString(PyExc_TypeError, 
                    "The Y attribute must be an integer");
        return -1;
    }

    self->bounds->y = (int)PyInt_AS_LONG(value);
    
    return 0;
}
static PyObject *
PyClaroBounds_getw(PyClaroBounds *self, void *closure)
{
    return PyInt_FromLong((long)self->bounds->w);
}

static int
PyClaroBounds_setw(PyClaroBounds *self, PyObject *value, void *closure)
{
    if (!PyInt_Check(value)) 
    {
        PyErr_SetString(PyExc_TypeError, 
                    "The Width attribute must be an integer");
        return -1;
    }

    self->bounds->w = (int)PyInt_AS_LONG(value);
    
    return 0;
}
static PyObject *
PyClaroBounds_geth(PyClaroBounds *self, void *closure)
{
    return PyInt_FromLong((long)self->bounds->h);
}

static int
PyClaroBounds_seth(PyClaroBounds *self, PyObject *value, void *closure)
{
    if (!PyInt_Check(value)) 
    {
        PyErr_SetString(PyExc_TypeError, 
                    "The Height attribute must be an integer");
        return -1;
    }

    self->bounds->h = (int)PyInt_AS_LONG(value);
    
    return 0;
}

static PyGetSetDef PyClaroBounds_getseters[] = 
{
    {"x", 
     (getter)PyClaroBounds_getx, (setter)PyClaroBounds_setx,
     "X",
     NULL},
    {"y", 
     (getter)PyClaroBounds_gety, (setter)PyClaroBounds_sety,
     "Y",
     NULL},
    {"w", 
     (getter)PyClaroBounds_getw, (setter)PyClaroBounds_setw,
     "Width",
     NULL},
    {"h", 
     (getter)PyClaroBounds_geth, (setter)PyClaroBounds_seth,
     "Height",
     NULL},
    {NULL}  /* Sentinel */
};

static PyTypeObject PyClaroBoundsType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "claro.Bounds",            /*tp_name*/
    sizeof(PyClaroBounds),     /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)PyClaroBounds_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Bounds",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    0,                         /* tp_methods */
    0,                         /* tp_members */
    PyClaroBounds_getseters,           /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyClaroBounds_init,      /* tp_init */
    0,                         /* tp_alloc */
    PyClaroBounds_new,                 /* tp_new */
};

// object_t

static void
PyClaroObject_dealloc(PyClaroObject * self)
{
    //unref 
 //   const char * name = self->object->class_type->info->name;
 //   printf("%s: %s at %p\n", __FUNCTION__, name, self->object);
    self->ob_type->tp_free((PyObject*)self); 
}

static PyObject *
PyClaroObject_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    return type->tp_alloc(type, 0);
}

static int
PyClaroObject_init(PyClaroObject *self, PyObject *args, PyObject *kwds)
{
    PyErr_SetString(PyExc_RuntimeError, "Can't initiate an abstract class.");
    return -1;
}

static PyObject *
PyClaroObject_get__type(PyClaroObject *self, void *closure)
{
    assert(self->object != NULL);

    return PyString_FromString(self->object->type);
}

static PyObject * 
PyClaroObject_getparent(PyClaroObject * self, void * closure)
{
    assert(self->object != NULL);
    object_t * parent = self->object->parent;
    
    if(parent)
        return PyClaroObject_wrap(parent);
    else
        Py_RETURN_NONE;  
}

static PyObject *
PyClaroObject_getchildren(PyClaroObject * self, void * closure)
{
    PyObject * pylist;
    object_t * children;
    int i, len;    

    assert(self->object != NULL);
    children = self->object->children;    
    assert(children != NULL);

    len = claro_list_count(children);
    pylist = PyList_New(len);

    if(!pylist)
        return NULL;    

    for(i = 0; i < len; i++)
    {
        object_t * child = (object_t *)claro_list_get_item(children, i);    
        PyList_SET_ITEM(pylist, i, PyClaroObject_wrap(child));    
    }

    return pylist;
}

static PyGetSetDef PyClaroObject_getseters[] = 
{
    {
        "children", (getter)PyClaroObject_getchildren, NULL,
        "Children", NULL
    },
    {
        "parent", (getter)PyClaroObject_getparent, NULL,
        "Logical Parent", NULL
    },
    {
        "__claro_type__", (getter)PyClaroObject_get__type, NULL,
        "Claro Type", NULL
    },   
    {NULL}  /* Sentinel */
};

static void _PyClaroObject_event_cb(object_t * object, event_t * event, void * data)
{
    PyObject * callback, * args, * result, * obj;
    PyClaroEvent * evt;

    callback = (PyObject *)data;

    obj = PyClaroObject_wrap(object);

    evt = PyObject_New(PyClaroEvent, &PyClaroEventType);
    assert(event != NULL);    
    evt->event = event;

    args = Py_BuildValue("(OO)", obj, (PyObject *)evt);
    result = PyEval_CallObject(callback, args);

    Py_DECREF(args);
    Py_XDECREF(obj);
    Py_DECREF(evt);
    Py_XDECREF(result);
}

static PyObject * PyClaroObject_addhandler(PyClaroObject * object, PyObject * args)
{
    const char * event_name;
    PyObject * callback = NULL;

    if(!PyArg_ParseTuple(args, "sO", &event_name, &callback))
        return NULL;

    if(!PyCallable_Check(callback))
    {
        PyErr_SetString(PyExc_ValueError, "A callable object was not supplied.");
        return NULL;
    }    

    Py_XINCREF(callback);
    object_addhandler_interface(object->object, event_name, _PyClaroObject_event_cb, (void *)callback);
   
    Py_RETURN_NONE;  
}   

static PyMethodDef PyClaroObject_methods[] = {
    {
        "addhandler", (PyCFunction)PyClaroObject_addhandler, METH_VARARGS,
        "Add a callback handler for an event."
    },
    {NULL}  /* Sentinel */
};

static PyTypeObject PyClaroObjectType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "claro.Object",            /*tp_name*/
    sizeof(PyClaroObject),     /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)PyClaroObject_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Object",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    PyClaroObject_methods,                         /* tp_methods */
    0,                         /* tp_members */
    PyClaroObject_getseters,           /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyClaroObject_init,      /* tp_init */
    0,                         /* tp_alloc */
    PyClaroObject_new,                 /* tp_new */
};


/* 
 this is quite hackish- but the best I can do until reference counting is 
 finished and properly tested.

 best way to do this is make sure there is ALWAYS a one-to-one mapping of
 wrappers and real objects. That way the wrapper can hold a ref to the real
 deal, and the python program can hold as many references to the wrapper as
 needed. That way everybody is happy...
*/

static PyObject * PyClaroObject_wrap(object_t *obj)
{
    PyClaroObject *self;
    PyTypeObject * type;

    if (obj == NULL) 
        Py_RETURN_NONE;  

    // already 'wrapped' 
    self = (PyClaroObject *)obj->appdata;

    if (self != NULL) 
    {
	    Py_INCREF(self);
    } 
    else 
    {
        type = claro_hashtable_lookup(class_table, obj->class_type);
        
        if(type == NULL)
	        self = PyObject_New(PyClaroObject, &PyClaroObjectType);
        else
            self = PyObject_New(PyClaroObject, type);

	    if (self == NULL)
	        return NULL;

	    self->object = obj;
	    //claro_object_ref(obj);
        
        obj->appdata = (void *)self;	    

	  //  PyObject_GC_Track((PyObject *)self);
    }

    return (PyObject *)self;
}

// images

static int
PyClaroImage_init(PyClaroObject *self, PyObject *args, PyObject *kwds)
{
	PyObject * parent;
	const char * filename = 0;	
	object_t * obj;
	
	if(!PyArg_ParseTuple(args, "Os", &parent, &filename))
        return -1;
	
	if(parent == Py_None)
		obj = image_load(NULL, filename);
	else
	{	
		if(!PyObject_TypeCheck(parent, &PyClaroObjectType))
		{
			PyErr_SetString(PyExc_TypeError, "Argument 'parent' must be of type 'claro.Object'");					
			return -1;
		}
		
		obj = image_load(((PyClaroObject*)parent)->object, filename);
	}
	
	if(obj == NULL)
	{		
		PyErr_SetString(PyExc_IOError, "Couldn't load image file.");		
		return -1;
	}
	
	Object_wrapinit((PyObject*)self, obj);   
	
    return 0;
}

static PyTypeObject PyClaroImageType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "claro.Image",            /*tp_name*/
    sizeof(PyClaroObject),     /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)PyClaroObject_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0, /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,    /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Image",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    0,                         /* tp_methods */
    0,                         /* tp_members */
    0,           /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyClaroImage_init,      /* tp_init */
    0,                         /* tp_alloc */
    PyClaroObject_new,                 /* tp_new */
};

//events

static void
PyClaroEvent_dealloc(PyClaroEvent * self)
{
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject *
PyClaroEvent_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    return type->tp_alloc(type, 0);
}

static int
PyClaroEvent_init(PyClaroObject *self, PyObject *args, PyObject *kwds)
{
    PyErr_SetString(PyExc_RuntimeError, "Can't initiate an abstract class.");
    return -1;
}

static PyObject *
PyClaroEvent_getevent_name(PyClaroEvent *self, void *closure)
{
    assert(self->event != NULL);
    return PyString_FromString(event_get_name(self->event));
}

static PyGetSetDef PyClaroEvent_getseters[] = 
{
     {"event_name", 
     (getter)PyClaroEvent_getevent_name, NULL,
     "Event Name",
     NULL},   
    {NULL}  /* Sentinel */
};

static Py_ssize_t PyClaroEvent_len(PyObject * self)
{
    claro_hashtable_t * table;

    assert(((PyClaroEvent*)self)->event != NULL);

    table = ((PyClaroEvent*)self)->event->args;
    
    if(table)
        return claro_hashtable_count(((PyClaroEvent*)self)->event->args);
    else
        return 0;
}

static PyObject * PyClaroEvent_get_item(PyObject * self, PyObject * arg)
{
    const char * key;
    _event_arg_t * val;
    claro_hashtable_t * table;

    assert(((PyClaroEvent*)self)->event != NULL);

    table = ((PyClaroEvent*)self)->event->args;
    
    key = PyString_AsString(arg);
    
    if(!table)
    {
        PyErr_SetString(PyExc_KeyError, "Event argument doesn't exist.");
        return NULL;
    }

    if(!key)
        return NULL;
    
    val = (_event_arg_t *)claro_hashtable_lookup(table, (void *)key); 

    if(!val)
    {
        PyErr_SetString(PyExc_KeyError, "Event argument doesn't exist.");
        return NULL;
    }

    if(val->val == NULL)
        goto fail;

    switch(val->type)
    {
        case 'i':
            return PyInt_FromLong((long)*(int *)val->val);
        case 'd':
            return PyFloat_FromDouble(*(double *)val->val);
        case 'p':
            //printf("%s: %p\n", __FUNCTION__, val->val);
            return PyLong_FromVoidPtr(val->val);                    
    } 

fail:
    Py_RETURN_NONE;        
}

static PyMappingMethods PyClaroEvent_as_mapping = {
     PyClaroEvent_len, PyClaroEvent_get_item, NULL
};

static PyTypeObject PyClaroEventType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "claro.Event",            /*tp_name*/
    sizeof(PyClaroEvent),     /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)PyClaroEvent_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0, /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    &PyClaroEvent_as_mapping,    /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Event",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    0,                         /* tp_methods */
    0,                         /* tp_members */
    PyClaroEvent_getseters,           /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyClaroEvent_init,      /* tp_init */
    0,                         /* tp_alloc */
    PyClaroEvent_new,                 /* tp_new */
};

static PyObject *
PyClaroWidget_getscreen_offset(PyClaroObject * self, void * closure)
{
    int x, y;
    PyObject * tuple;

    assert(self->object);

    widget_screen_offset(self->object, &x, &y);
    
    tuple = PyTuple_New(2);

    PyTuple_SET_ITEM(tuple, 0, PyInt_FromLong((long)x));
    PyTuple_SET_ITEM(tuple, 1, PyInt_FromLong((long)y));
    
    return tuple;
}

static int
PyClaroWidget_setcursor(PyClaroObject *self, PyObject *value, void *closure)
{
    if (!PyInt_Check(value)) 
    {
        PyErr_SetString(PyExc_TypeError, 
                    "The Cursor attribute must be an integer");
        return -1;
    }

    widget_set_cursor(self->object, (int)PyInt_AS_LONG(value));
    
    return 0;
}

static PyGetSetDef PyClaroWidget_getseters[] = 
{
    {
        "screen_offset", (getter)PyClaroWidget_getscreen_offset, NULL,
        "Screen Offset", NULL
    },
    {
        "cursor", NULL, (setter)PyClaroWidget_setcursor, 
        "Cursor", NULL
    }, 
    {NULL}  /* Sentinel */
};

static PyObject * Widget_show(PyClaroObject * self)
{
	widget_show(self->object);
	Py_RETURN_NONE;
}

static PyObject * Widget_hide(PyClaroObject * self)
{
	widget_hide(self->object);
	Py_RETURN_NONE;
}

static PyObject * Widget_enable(PyClaroObject * self)
{
	widget_enable(self->object);
	Py_RETURN_NONE;
}

static PyObject * Widget_disable(PyClaroObject * self)
{
	widget_disable(self->object);
	Py_RETURN_NONE;
}

static PyObject * Widget_focus(PyClaroObject * self)
{
	widget_focus(self->object);
	Py_RETURN_NONE;
}

static PyObject * Widget_close(PyClaroObject * self)
{
	widget_close(self->object);
	Py_RETURN_NONE;
}

static PyObject * Widget_set_notify(PyClaroObject * self, PyObject * args)
{
    int flags = 0;
    if(!PyArg_ParseTuple(args, "i", &flags))
        return NULL;

	widget_set_notify(self->object, flags);
	Py_RETURN_NONE;
}

static PyObject * Widget_set_font(PyClaroObject * self, PyObject * args)
{
    const char * face;
    int size, weight = cFontWeightNormal, slant = cFontSlantNormal, decoration = cFontDecorationNormal;
    if(!PyArg_ParseTuple(args, "si|iii", &face, &size, &weight, &slant, &decoration))
        return NULL;

    printf("%s: %s\n", __FUNCTION__, face);
	widget_set_font(self->object, face, size, weight, slant, decoration);
	Py_RETURN_NONE;
}

static PyMethodDef PyClaroWidget_methods[] = {
    {
        "show", (PyCFunction)Widget_show, METH_NOARGS,
        "Show"
    },
    {
        "set_font", (PyCFunction)Widget_set_font, METH_VARARGS,
        "Set font"
    },
    {
        "hide", (PyCFunction)Widget_hide, METH_NOARGS,
        "Hide"
    },
    {
        "enable", (PyCFunction)Widget_enable, METH_NOARGS,
        "Enable"
    },
    {
        "disable", (PyCFunction)Widget_disable, METH_NOARGS,
        "Disable"
    },
    {
        "focus", (PyCFunction)Widget_focus, METH_NOARGS,
        "Focus"
    },
    {
        "close", (PyCFunction)Widget_close, METH_NOARGS,
        "Close"
    },
    {
        "set_notify", (PyCFunction)Widget_set_notify, METH_VARARGS,
        "Set notify events"
    },
    {NULL}  /* Sentinel */
};


static PyTypeObject PyClaroWidgetType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "claro.Widget",            /*tp_name*/
    sizeof(PyClaroObject),     /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)PyClaroObject_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0, /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,    /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Widget",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    PyClaroWidget_methods,      /* tp_methods */
    0,                         /* tp_members */
    PyClaroWidget_getseters,           /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyClaroObject_init,      /* tp_init */
    0,                                 /* tp_alloc */
    PyClaroObject_new,                 /* tp_new */
};

// layout

static int
PyClaroLayout_init(PyClaroObject *self, PyObject *args, PyObject *kwds)
{
	PyObject * parent;
	const char * layout_spec = 0;
    PyObject * bounds;
    int min_w = 0, min_h = 0;	
	object_t * obj;
	
    CLFEXP layout_t *layout_create(object_t *parent, const char *layout_spec, bounds_t bounds, int min_w, int min_h);

	if(!PyArg_ParseTuple(args, "OsO!ii", &parent, &layout_spec, &PyClaroBoundsType, &bounds, &min_w, &min_h))
        return -1;
	
	if(parent == Py_None)
		obj = layout_create(NULL, layout_spec, *((PyClaroBounds*)bounds)->bounds, min_w, min_h);
	else
	{	
		if(!PyObject_TypeCheck(parent, &PyClaroObjectType))
		{
			PyErr_SetString(PyExc_TypeError, "Argument 'parent' must be of type 'claro.Object'");					
			return -1;
		}
		
		obj = layout_create(((PyClaroObject*)parent)->object, layout_spec, *((PyClaroBounds*)bounds)->bounds, min_w, min_h);
	}
	
	if(obj == NULL)
	{		
		PyErr_SetString(PyExc_IOError, "Couldn't create layout.");		
		return -1;
	}
	
	Object_wrapinit((PyObject*)self, obj);   
	
    return 0;
}

static PyObject * PyClaroLayout_get_bounds(PyClaroObject * self, PyObject * args)
{
    const char * cell_name = NULL;
    bounds_t * bounds = NULL;
    if(!PyArg_ParseTuple(args, "s", &cell_name))
        return NULL;
    bounds = lt_bounds((layout_t *)self->object, cell_name);
    if(bounds)
    {
        return PyClaroBounds_wrap(bounds);
    }    
    else
        Py_RETURN_NONE; 
}

static PyMethodDef PyClaroLayout_methods[] = {
    {
        "get_bounds", (PyCFunction)PyClaroLayout_get_bounds, METH_VARARGS,
        "Gets a bounds object for a specific cell"
    },
    {NULL}  /* Sentinel */
};

static PyTypeObject PyClaroLayoutType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "claro.Layout",            /*tp_name*/
    sizeof(PyClaroObject),     /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)PyClaroObject_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0, /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,    /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Layout",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    PyClaroLayout_methods,      /* tp_methods */
    0,                         /* tp_members */
    0,           /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)PyClaroLayout_init,      /* tp_init */
    0,                                 /* tp_alloc */
    PyClaroObject_new,                 /* tp_new */
};


//widgets

static void Object_wrapinit(PyObject * wrapper, object_t * obj)
{
	((PyClaroObject*)wrapper)->object = obj;
	
	//wrapper owns a reference to the real object
	//claro_object_ref(obj);
    
	obj->appdata = (void *)wrapper;	 	
}

static PyObject * Window_hide(PyClaroObject * self)
{
	window_hide(self->object);
	Py_RETURN_NONE;
}

static PyObject * Window_show(PyClaroObject * self)
{
	window_show(self->object);
	Py_RETURN_NONE;
}

static PyMethodDef Window_methods[] = {
    {
        "show", (PyCFunction)Window_show, METH_NOARGS,
        "Show"
    },
    {
        "hide", (PyCFunction)Window_hide, METH_NOARGS,
        "Hide"
    },
    {NULL}  /* Sentinel */
};

int Window_settitle(PyClaroObject * self, PyObject * value, void * closure)
{
	window_set_title(self->object, PyString_AsString(value));
	return 0;		
}

static PyGetSetDef Window_getseters[] = 
{
	{
		"title", NULL, (setter)Window_settitle,
     	"Title", NULL
	},   
    
	{NULL}  /* Sentinel */
};

static int
Window_init(PyClaroObject *self, PyObject *args, PyObject *kwds)
{
	PyObject * parent = NULL, * bounds = NULL;
	int flags = 0;	
	const char * title = NULL;
	object_t * obj;
	
	if(!PyArg_ParseTuple(args, "OO!|is", &parent,
		&PyClaroBoundsType, &bounds, &flags, &title))
        return -1;
	
	if(parent == Py_None)
		obj = window_widget_create(NULL, ((PyClaroBounds*)bounds)->bounds, flags);
	else
	{	
		if(!PyObject_TypeCheck(parent, &PyClaroObjectType))
		{
			PyErr_SetString(PyExc_TypeError, "Argument 'parent' must be of type 'claro.Object'");					
			return -1;
		}
		
		obj = window_widget_create(((PyClaroObject*)parent)->object, 
			((PyClaroBounds*)bounds)->bounds, flags);
	}
		
	if(obj == NULL)
	{		
		PyErr_SetString(PyExc_RuntimeError, "Couldn't create Window object.");		
		return -1;
	}
	
	if(title != NULL)
		window_set_title(obj, title);	
	
	Object_wrapinit((PyObject*)self, obj);   
	
    return 0;
}

static int
Canvas_init(PyClaroObject *self, PyObject *args, PyObject *kwds)
{
	PyObject * parent = NULL, * bounds = NULL;
	int flags = 0;	
	object_t * obj;
	
	if(!PyArg_ParseTuple(args, "OO!|i", &parent,
		&PyClaroBoundsType, &bounds, &flags))
        return -1;
	
	if(parent == Py_None)
		obj = canvas_widget_create(NULL, ((PyClaroBounds*)bounds)->bounds, flags);
	else
	{	
		if(!PyObject_TypeCheck(parent, &PyClaroObjectType))
		{
			PyErr_SetString(PyExc_TypeError, "Argument 'parent' must be of type 'claro.Object'");					
			return -1;
		}
		
		obj = canvas_widget_create(((PyClaroObject*)parent)->object, 
			((PyClaroBounds*)bounds)->bounds, flags);
	}
		
	if(obj == NULL)
	{		
		PyErr_SetString(PyExc_RuntimeError, "Couldn't create Canvas object.");		
		return -1;
	}
		
	Object_wrapinit((PyObject*)self, obj);   
	
    return 0;
}

static PyObject * Canvas_redraw(PyClaroObject * self)
{
	canvas_redraw(self->object);
	Py_RETURN_NONE;
}

static PyMethodDef Canvas_methods[] = {
    {
        "redraw", (PyCFunction)Canvas_redraw, METH_NOARGS,
        "Redraw"
    },
    {NULL}  /* Sentinel */
};

PyObject * Canvas_getcr(PyClaroObject * self, void * closure)
{
	canvas_widget_t * canvas = (canvas_widget_t*)self->object;
	
	if(canvas->cr == NULL)
		Py_RETURN_NONE;
	else	
		return PycairoContext_FromContext(cairo_reference(canvas->cr), NULL, NULL);		
}

static PyGetSetDef Canvas_getseters[] = 
{
	{
		"cr", (getter)Canvas_getcr, NULL,
     	"Cairo Context", NULL
	},   
    
	{NULL}  /* Sentinel */
};

//menubar

static int
MenuBar_init(PyClaroObject *self, PyObject *args, PyObject *kwds)
{
	PyObject * parent;
	int flags = 0;	
	object_t * obj;
	
	if(!PyArg_ParseTuple(args, "O|i", &parent, &flags))
        return -1;
	
	if(parent == Py_None)
		obj = menubar_widget_create(NULL, flags);
	else
	{	
		if(!PyObject_TypeCheck(parent, &PyClaroObjectType))
		{
			PyErr_SetString(PyExc_TypeError, "Argument 'parent' must be of type 'claro.Object'");					
			return -1;
		}
		
		obj = menubar_widget_create(((PyClaroObject*)parent)->object, flags);
	}
		
	if(obj == NULL)
	{		
		PyErr_SetString(PyExc_RuntimeError, "Couldn't create MenuBar object.");		
		return -1;
	}
	
	Object_wrapinit((PyObject*)self, obj);   
	
    return 0;
}

static PyObject * MenuBar_add_key_binding(PyClaroObject * self, PyObject * args)
{
    PyObject * list_item = NULL;
    PyTypeObject * list_type = (PyTypeObject *)
        claro_hashtable_lookup(class_table, (void *)list_item_get_type());    
    const char * utf8_key = NULL;
    int modifier = 0;

    if(!PyArg_ParseTuple(args, "O!si", list_type, &list_item, &utf8_key, &modifier))
        return NULL;  
      
    menubar_add_key_binding(self->object, (list_item_t *)((PyClaroObject*)list_item)->object, 
        utf8_key, modifier);

    Py_RETURN_NONE;
}

static PyObject * MenuBar_append_item(PyClaroObject * self, PyObject * args)
{
    PyObject * pylist_item = NULL, * pyimage = NULL;
    PyTypeObject * list_type = (PyTypeObject *)claro_hashtable_lookup(class_table, (void *)list_item_get_type());
    PyTypeObject * image_type = (PyTypeObject *)claro_hashtable_lookup(class_table, (void *)image_get_type());      
    char * title = NULL;
    list_item_t * list_item = NULL, * res = NULL;
    image_t * image = NULL;
    

    if(!PyArg_ParseTuple(args, "OOs", &pylist_item, &pyimage, &title))
        return NULL;  

    if(PyObject_TypeCheck(pylist_item, list_type)) 
        list_item = (list_item_t *)((PyClaroObject *)pylist_item)->object;

    if(PyObject_TypeCheck(pyimage, image_type)) 
        image = (image_t *)((PyClaroObject *)pyimage)->object;

    res = menubar_append_item(self->object, list_item, image, title);
    if(res == NULL)
        Py_RETURN_NONE;
    else
        return PyClaroObject_wrap((object_t *)res);
}

static PyMethodDef MenuBar_methods[] = {
    {
        "add_key_binding", (PyCFunction)MenuBar_add_key_binding, METH_VARARGS,
        "Redraw"
    },
    {
        "append_item", (PyCFunction)MenuBar_append_item, METH_VARARGS,
        "Append an item"
    },
    {NULL}  /* Sentinel */
};

static int
ListItem_init(PyClaroObject *self, PyObject *args, PyObject *kwds)
{
    PyErr_SetString(PyExc_RuntimeError, "Can't initiate an abstract class.");
    return -1;
}

static int
ListBox_init(PyClaroObject *self, PyObject *args, PyObject *kwds)
{
    PyObject * parent = NULL, * bounds = NULL;
	int flags = 0;	
	object_t * obj;
	
	if(!PyArg_ParseTuple(args, "OO!|i", &parent,
		&PyClaroBoundsType, &bounds, &flags))
        return -1;
	
	if(parent == Py_None)
		obj = listbox_widget_create(NULL, ((PyClaroBounds*)bounds)->bounds, flags);
	else
	{	
		if(!PyObject_TypeCheck(parent, &PyClaroObjectType))
		{
			PyErr_SetString(PyExc_TypeError, "Argument 'parent' must be of type 'claro.Object'");					
			return -1;
		}
		
		obj = listbox_widget_create(((PyClaroObject*)parent)->object, 
			((PyClaroBounds*)bounds)->bounds, flags);
	}
		
	if(obj == NULL)
	{		
		PyErr_SetString(PyExc_RuntimeError, "Couldn't create ListBox object.");		
		return -1;
	}
		
	Object_wrapinit((PyObject*)self, obj);   
	
    return 0;
}

static PyObject * ListBox_append_row(PyClaroObject * self, PyObject * args)
{
	const char * text = NULL;

	if(!PyArg_ParseTuple(args, "s", &text))
		return NULL;
    
    return PyClaroObject_wrap((object_t *)listbox_append_row(self->object, (char *)text));
}

static PyObject * ListBox_insert_row(PyClaroObject * self, PyObject * args)
{
	const char * text = NULL;
    int pos = 0;

	if(!PyArg_ParseTuple(args, "is", &pos, &text))
		return NULL;
    
    return PyClaroObject_wrap((object_t *)listbox_insert_row(self->object, pos, (char *)text));
}

static PyObject * ListBox_remove_row(PyClaroObject * self, PyObject * args)
{
    PyObject * row = NULL;
    PyTypeObject * list_type = (PyTypeObject *)claro_hashtable_lookup(class_table, (void *)list_item_get_type());

	if(!PyArg_ParseTuple(args, "O!", list_type, &row))
		return NULL;
    
    listbox_remove_row(self->object, (list_item_t *)((PyClaroObject *)row)->object);

    Py_RETURN_NONE;
}

static PyObject * ListBox_move_row(PyClaroObject * self, PyObject * args)
{
    PyObject * row = NULL;
    PyTypeObject * list_type = (PyTypeObject *)claro_hashtable_lookup(class_table, (void *)list_item_get_type());
    int pos = 0;

	if(!PyArg_ParseTuple(args, "O!i", list_type, &row, &pos))
		return NULL;
    
    listbox_move_row(self->object, (list_item_t *)((PyClaroObject *)row)->object, pos);

    Py_RETURN_NONE;
}

static PyObject * ListBox_select_item(PyClaroObject * self, PyObject * args)
{
    PyObject * row = NULL;
    PyTypeObject * list_type = (PyTypeObject *)claro_hashtable_lookup(class_table, (void *)list_item_get_type());

	if(!PyArg_ParseTuple(args, "O!", list_type, &row))
		return NULL;
    
    listbox_select_item(self->object, (list_item_t *)((PyClaroObject *)row)->object);

    Py_RETURN_NONE;
}

static PyMethodDef ListBox_methods[] = {
    {
        "append_row", (PyCFunction)ListBox_append_row, METH_VARARGS,
        "Append a row"
    },
    {
        "insert_row", (PyCFunction)ListBox_insert_row, METH_VARARGS,
        "Insert a row"
    },
    {
        "remove_row", (PyCFunction)ListBox_remove_row, METH_VARARGS,
        "Remove a row"
    },
    {
        "move_row", (PyCFunction)ListBox_move_row, METH_VARARGS,
        "Move a row"
    },
    {
        "select_item", (PyCFunction)ListBox_select_item, METH_VARARGS,
        "Select an item"
    },
    {NULL}  /* Sentinel */
};

PyObject * ListBox_getselected(PyClaroObject * self, void * closure)
{
	object_t * item = listbox_get_selected(self->object);

	if(item == NULL)
		Py_RETURN_NONE;
	else	
		return PyClaroObject_wrap(item);
}

PyObject * ListBox_getrows(PyClaroObject * self, void * closure)
{
    return PyInt_FromLong((long)listbox_get_rows(self->object));
}

static PyGetSetDef ListBox_getseters[] = 
{
	{
		"selected", (getter)ListBox_getselected, NULL,
     	"Selected item", NULL
	},   
    {
		"rows", (getter)ListBox_getrows, NULL,
     	"Number of rows", NULL
	},
	{NULL}  /* Sentinel */
};

static int
Combo_init(PyClaroObject *self, PyObject *args, PyObject *kwds)
{
    PyObject * parent = NULL, * bounds = NULL;
	int flags = 0;	
	object_t * obj;
	
	if(!PyArg_ParseTuple(args, "OO!|i", &parent,
		&PyClaroBoundsType, &bounds, &flags))
        return -1;
	
	if(parent == Py_None)
		obj = combo_widget_create(NULL, ((PyClaroBounds*)bounds)->bounds, flags);
	else
	{	
		if(!PyObject_TypeCheck(parent, &PyClaroObjectType))
		{
			PyErr_SetString(PyExc_TypeError, "Argument 'parent' must be of type 'claro.Object'");					
			return -1;
		}
		
		obj = combo_widget_create(((PyClaroObject*)parent)->object, 
			((PyClaroBounds*)bounds)->bounds, flags);
	}
		
	if(obj == NULL)
	{		
		PyErr_SetString(PyExc_RuntimeError, "Couldn't create ListBox object.");		
		return -1;
	}
		
	Object_wrapinit((PyObject*)self, obj);   
	
    return 0;
}

static PyObject * Combo_append_row(PyClaroObject * self, PyObject * args)
{
	const char * text = NULL;

	if(!PyArg_ParseTuple(args, "s", &text))
		return NULL;
    
    return PyClaroObject_wrap((object_t *)combo_append_row(self->object, (char *)text));
}

static PyObject * Combo_insert_row(PyClaroObject * self, PyObject * args)
{
	const char * text = NULL;
    int pos = 0;

	if(!PyArg_ParseTuple(args, "is", &pos, &text))
		return NULL;
    
    return PyClaroObject_wrap((object_t *)combo_insert_row(self->object, pos, (char *)text));
}

static PyObject * Combo_remove_row(PyClaroObject * self, PyObject * args)
{
    PyObject * row = NULL;
    PyTypeObject * list_type = (PyTypeObject *)claro_hashtable_lookup(class_table, (void *)list_item_get_type());

	if(!PyArg_ParseTuple(args, "O!", list_type, &row))
		return NULL;
    
    combo_remove_row(self->object, (list_item_t *)((PyClaroObject *)row)->object);

    Py_RETURN_NONE;
}

static PyObject * Combo_move_row(PyClaroObject * self, PyObject * args)
{
    PyObject * row = NULL;
    PyTypeObject * list_type = (PyTypeObject *)claro_hashtable_lookup(class_table, (void *)list_item_get_type());
    int pos = 0;

	if(!PyArg_ParseTuple(args, "O!i", list_type, &row, &pos))
		return NULL;
    
    combo_move_row(self->object, (list_item_t *)((PyClaroObject *)row)->object, pos);

    Py_RETURN_NONE;
}

static PyObject * Combo_select_item(PyClaroObject * self, PyObject * args)
{
    PyObject * row = NULL;
    PyTypeObject * list_type = (PyTypeObject *)claro_hashtable_lookup(class_table, (void *)list_item_get_type());

	if(!PyArg_ParseTuple(args, "O!", list_type, &row))
		return NULL;
    
    combo_select_item(self->object, (list_item_t *)((PyClaroObject *)row)->object);

    Py_RETURN_NONE;
}

static PyMethodDef Combo_methods[] = {
    {
        "append_row", (PyCFunction)Combo_append_row, METH_VARARGS,
        "Append a row"
    },
    {
        "insert_row", (PyCFunction)Combo_insert_row, METH_VARARGS,
        "Insert a row"
    },
    {
        "remove_row", (PyCFunction)Combo_remove_row, METH_VARARGS,
        "Remove a row"
    },
    {
        "move_row", (PyCFunction)Combo_move_row, METH_VARARGS,
        "Move a row"
    },
    {
        "select_item", (PyCFunction)Combo_select_item, METH_VARARGS,
        "Select an item"
    },
    {NULL}  /* Sentinel */
};

PyObject * Combo_getselected(PyClaroObject * self, void * closure)
{
	object_t * item = combo_get_selected(self->object);

	if(item == NULL)
		Py_RETURN_NONE;
	else	
		return PyClaroObject_wrap(item);
}

PyObject * Combo_getrows(PyClaroObject * self, void * closure)
{
    return PyInt_FromLong((long)combo_get_rows(self->object));
}

static PyGetSetDef Combo_getseters[] = 
{
	{
		"selected", (getter)Combo_getselected, NULL,
     	"Selected item", NULL
	},   
    {
		"rows", (getter)Combo_getrows, NULL,
     	"Number of rows", NULL
	},
	{NULL}  /* Sentinel */
};

static int
Label_init(PyClaroObject *self, PyObject *args, PyObject *kwds)
{
    PyObject * parent = NULL, * bounds = NULL;
	int flags = 0;	
	object_t * obj;
	
	if(!PyArg_ParseTuple(args, "OO!|i", &parent,
		&PyClaroBoundsType, &bounds, &flags))
        return -1;
	
	if(parent == Py_None)
		obj = label_widget_create(NULL, ((PyClaroBounds*)bounds)->bounds, flags);
	else
	{	
		if(!PyObject_TypeCheck(parent, &PyClaroObjectType))
		{
			PyErr_SetString(PyExc_TypeError, "Argument 'parent' must be of type 'claro.Object'");					
			return -1;
		}
		
		obj = label_widget_create(((PyClaroObject*)parent)->object, 
			((PyClaroBounds*)bounds)->bounds, flags);
	}
		
	if(obj == NULL)
	{		
		PyErr_SetString(PyExc_RuntimeError, "Couldn't create Label object.");		
		return -1;
	}
		
	Object_wrapinit((PyObject*)self, obj);   
	
    return 0;
}

static int
Label_settext(PyClaroObject *self, PyObject *value, void *closure)
{   
    if (!PyString_Check(value)) 
    {
        PyErr_SetString(PyExc_TypeError, 
                    "The Text attribute must be a string");
        return -1;
    }

    label_set_text(self->object, PyString_AS_STRING(value));
    
    return 0;
}

static PyGetSetDef Label_getseters[] = 
{
	{
		"text", NULL, (setter)Label_settext,
     	"Text", NULL
	},  
    { NULL }
};

static int
Button_init(PyClaroObject *self, PyObject *args, PyObject *kwds)
{
    PyObject * parent = NULL, * bounds = NULL;
	int flags = 0;	
	object_t * obj;
	
	if(!PyArg_ParseTuple(args, "OO!|i", &parent,
		&PyClaroBoundsType, &bounds, &flags))
        return -1;
	
	if(parent == Py_None)
		obj = button_widget_create(NULL, ((PyClaroBounds*)bounds)->bounds, flags);
	else
	{	
		if(!PyObject_TypeCheck(parent, &PyClaroObjectType))
		{
			PyErr_SetString(PyExc_TypeError, "Argument 'parent' must be of type 'claro.Object'");					
			return -1;
		}
		
		obj = button_widget_create(((PyClaroObject*)parent)->object, 
			((PyClaroBounds*)bounds)->bounds, flags);
	}
		
	if(obj == NULL)
	{		
		PyErr_SetString(PyExc_RuntimeError, "Couldn't create Button object.");		
		return -1;
	}
		
	Object_wrapinit((PyObject*)self, obj);   
	
    return 0;
}

static int
Button_settext(PyClaroObject *self, PyObject *value, void *closure)
{   
    if (!PyString_Check(value)) 
    {
        PyErr_SetString(PyExc_TypeError, 
                    "The Text attribute must be a string");
        return -1;
    }

    button_set_label(self->object, PyString_AS_STRING(value));
    
    return 0;
}

static PyGetSetDef Button_getseters[] = 
{
	{
		"text", NULL, (setter)Button_settext,
     	"Text", NULL
	},  
    { NULL }
};

static int
TextBox_init(PyClaroObject *self, PyObject *args, PyObject *kwds)
{
    PyObject * parent = NULL, * bounds = NULL;
	int flags = 0;	
	object_t * obj;
	
	if(!PyArg_ParseTuple(args, "OO!|i", &parent,
		&PyClaroBoundsType, &bounds, &flags))
        return -1;
	
	if(parent == Py_None)
		obj = textbox_widget_create(NULL, ((PyClaroBounds*)bounds)->bounds, flags);
	else
	{	
		if(!PyObject_TypeCheck(parent, &PyClaroObjectType))
		{
			PyErr_SetString(PyExc_TypeError, "Argument 'parent' must be of type 'claro.Object'");					
			return -1;
		}
		
		obj = textbox_widget_create(((PyClaroObject*)parent)->object, 
			((PyClaroBounds*)bounds)->bounds, flags);
	}
		
	if(obj == NULL)
	{		
		PyErr_SetString(PyExc_RuntimeError, "Couldn't create TextBox object.");		
		return -1;
	}
		
	Object_wrapinit((PyObject*)self, obj);   
	
    return 0;
}

static PyObject * TextBox_gettext(PyClaroObject * self, void * closure)
{
    char * text = textbox_get_text(self->object);
    if(!text)
        Py_RETURN_NONE;
    else
        return PyString_FromString(text);
}

static PyGetSetDef TextBox_getseters[] = 
{
	{
		"text", (getter)TextBox_gettext, NULL,
     	"Text", NULL
	},  
    { NULL }
};

static PyObject * ListItem_gettext(PyClaroObject * self, void * closure)
{
    char * text = ((list_item_t *)self->object)->data[0];
    if(!text)
        Py_RETURN_NONE;
    else
        return PyString_FromString(text);
}

static PyGetSetDef ListItem_getseters[] = 
{
	{
		"text", (getter)ListItem_gettext, NULL,
     	"Text", NULL
	},  
    { NULL }
};

static PyObject * _claro_run(PyObject * self)
{
    claro_run();

    if(PyErr_Occurred())
        return NULL;

    Py_RETURN_NONE;    
} 

static bool_t is_running;

static PyObject * _claro_loop(PyObject * self)
{
    is_running = TRUE;

    while(is_running)
    {
        claro_run();
    
        if(PyErr_Occurred())
            return NULL;
    }

    Py_RETURN_NONE;    
} 

static PyObject * _claro_shutdown(PyObject * self)
{
    is_running = FALSE;

    if(PyErr_Occurred())
        return NULL;

    Py_RETURN_NONE;    
} 

static PyObject * _stock_get_image(PyObject * self, PyObject * args)
{
	const char * stock_id = NULL;
	int size = 0;
	object_t * image = NULL;
	
	if(!PyArg_ParseTuple(args, "si", &stock_id, &size))
		return NULL;
	
	image = (object_t *)stock_get_image(stock_id, size);
	
	if(image == NULL)
		Py_RETURN_NONE;
	else
		return PyClaroObject_wrap(image);
}

static PyMethodDef _claro_methods[] =
{
    {"run", (PyCFunction)_claro_run, METH_NOARGS, "Run an iteration of the mainloop" },
    {"loop", (PyCFunction)_claro_loop, METH_NOARGS, "Starts the mainloop" },
    {"shutdown", (PyCFunction)_claro_shutdown, METH_NOARGS, "Shuts down the mainloop" },
	{"stock_get_image", (PyCFunction)_stock_get_image, METH_VARARGS, "Gets a stock image"},
    {   NULL    }
};

static bool_t _type_equal(const void * p1, const void * p2)
{
    return p1 == p2;
}

static unsigned int _type_hash(const void * p)
{
    return (unsigned int)p;
}

PyMODINIT_FUNC
init_claro(void)
{
    PyObject * m, * py_claro;

	Pycairo_IMPORT;
    
    claro_base_init();
    claro_graphics_init();

    class_table = claro_hashtable_create(_type_hash, _type_equal, NULL, NULL);

    if (PyType_Ready(&PyClaroBoundsType) < 0 || 
        PyType_Ready(&PyClaroObjectType) < 0 || 
        PyType_Ready(&PyClaroEventType) < 0)
        return;

    PyClaroWidgetType.tp_base = &PyClaroObjectType;
    if(PyType_Ready(&PyClaroWidgetType) < 0)
        return;

	PyClaroLayoutType.tp_base = &PyClaroObjectType;
    if(PyType_Ready(&PyClaroLayoutType) < 0)
        return;

    PyClaroImageType.tp_base = &PyClaroObjectType;
    if(PyType_Ready(&PyClaroImageType) < 0)
        return;

    claro_hashtable_insert(class_table, object_get_type(), &PyClaroObjectType, FALSE);
    claro_hashtable_insert(class_table, widget_get_type(), &PyClaroWidgetType, FALSE);
	claro_hashtable_insert(class_table, image_get_type(), &PyClaroImageType, FALSE);
    claro_hashtable_insert(class_table, layout_get_type(), &PyClaroLayoutType, FALSE);

    m = Py_InitModule3("_claro", _claro_methods, "Claro Graphics");

    if(m == NULL)
        return;

    py_claro = PyClaroObject_wrap(claro);

    Py_INCREF(&PyClaroBoundsType);
    Py_INCREF(&PyClaroObjectType);
    Py_INCREF(&PyClaroEventType);
    Py_INCREF(&PyClaroImageType);
    Py_INCREF(&PyClaroWidgetType);
    Py_INCREF(&PyClaroLayoutType);

    PyModule_AddObject(m, "Bounds", (PyObject *)&PyClaroBoundsType);
    PyModule_AddObject(m, "Object", (PyObject *)&PyClaroObjectType);
    PyModule_AddObject(m, "Event", (PyObject *)&PyClaroEventType);
    PyModule_AddObject(m, "Widget", (PyObject *)&PyClaroWidgetType);
    PyModule_AddObject(m, "Image", (PyObject *)&PyClaroImageType);
    PyModule_AddObject(m, "Layout", (PyObject *)&PyClaroLayoutType);

    PyModule_AddObject(m, "__claro__", py_claro);
	
#define CONSTANT(constant) PyModule_AddIntConstant(m, #constant, constant)

	CONSTANT(cWidgetNoBorder);
	CONSTANT(cWidgetCustomDraw);

	CONSTANT(cNotifyMouse);
	CONSTANT(cNotifyKey);
	
	CONSTANT(cStockMenu);
	CONSTANT(cStockTb);
	
/**
	we need to talk about these- they dont make sense
	CONSTANT(cModifierKeyControl);
	CONSTANT(cModifierKeyAlternate);
	CONSTANT(cModifierKeyCommand);
	CONSTANT(cModifierKeyShift);
	CONSTANT(cModifierKeyAlternate);
**/	

	CONSTANT(cCursorNormal);
	CONSTANT(cCursorTextEdit);
	CONSTANT(cCursorWait);
	CONSTANT(cCursorPoint);
	
	CONSTANT(cModifierShift);
	CONSTANT(cModifierCommand);

    CONSTANT(cWindowModalDialog);
    CONSTANT(cWindowCenterParent);
    CONSTANT(cWindowNoResizing);

    CONSTANT(cTextBoxTypePassword);

    CONSTANT(cFontSlantItalic);
    CONSTANT(cFontSlantNormal);
    CONSTANT(cFontWeightNormal);
    CONSTANT(cFontWeightBold);
    CONSTANT(cFontDecorationNormal);
    CONSTANT(cFontDecorationUnderline);

#undef CONSTANT
	
	DEFINE_WIDGET(Window, window_widget_get_type, Window_methods, Window_getseters);
	DEFINE_WIDGET(Canvas, canvas_widget_get_type, Canvas_methods, Canvas_getseters);
	DEFINE_WIDGET(MenuBar, menubar_widget_get_type, MenuBar_methods, NULL);
    DEFINE_WIDGET(ListItem, list_item_get_type, NULL, ListItem_getseters);
    DEFINE_WIDGET(ListBox, listbox_widget_get_type, ListBox_methods, ListBox_getseters);
    DEFINE_WIDGET(Label, label_widget_get_type, NULL, Label_getseters);
    DEFINE_WIDGET(TextBox, textbox_widget_get_type, NULL, TextBox_getseters);
    DEFINE_WIDGET(Combo, combo_widget_get_type, Combo_methods, Combo_getseters);
    DEFINE_WIDGET(Button, button_widget_get_type, NULL, Button_getseters);
}

