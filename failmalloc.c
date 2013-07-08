#include "Python.h"
#include <stdlib.h>

#define VERSION "0.1"

static int enabled = 0;
static int installed = 0;
static size_t next_error = 0;

struct {
    PyMemAllocator raw;
    PyMemAllocator mem;
    PyMemAllocator obj;
} hook;

static void* hook_malloc(void *ctx, size_t size)
{
    PyMemAllocator *alloc = (PyMemAllocator *)ctx;
    void *ptr;
    if (next_error != 0)
        next_error--;
    if (next_error == 1)
        return NULL;
    ptr = alloc->malloc(alloc->ctx, size);
    return ptr;
}

static void* hook_realloc(void *ctx, void *ptr, size_t new_size)
{
    PyMemAllocator *alloc = (PyMemAllocator *)ctx;
    void *ptr2;
    if (next_error != 0)
        next_error--;
    if (next_error == 1)
        return NULL;
    ptr2 = alloc->realloc(alloc->ctx, ptr, new_size);
    return ptr2;
}

static void hook_free(void *ctx, void *ptr)
{
    PyMemAllocator *alloc = (PyMemAllocator *)ctx;
    alloc->free(alloc->ctx, ptr);
}

static void setup_hooks(void)
{
    PyMemAllocator alloc;

    alloc.malloc = hook_malloc;
    alloc.realloc = hook_realloc;
    alloc.free = hook_free;
    PyMem_GetAllocator(PYMEM_DOMAIN_RAW, &hook.raw);
    PyMem_GetAllocator(PYMEM_DOMAIN_MEM, &hook.mem);
    PyMem_GetAllocator(PYMEM_DOMAIN_OBJ, &hook.obj);

    /* TODO: the hook is not thread-safe */
    alloc.ctx = &hook.raw;
    PyMem_SetAllocator(PYMEM_DOMAIN_RAW, &alloc);

    alloc.ctx = &hook.mem;
    PyMem_SetAllocator(PYMEM_DOMAIN_MEM, &alloc);

    alloc.ctx = &hook.obj;
    PyMem_SetAllocator(PYMEM_DOMAIN_OBJ, &alloc);
}

static void remove_hooks(void)
{
    PyMem_SetAllocator(PYMEM_DOMAIN_RAW, &hook.raw);
    PyMem_SetAllocator(PYMEM_DOMAIN_MEM, &hook.mem);
    PyMem_SetAllocator(PYMEM_DOMAIN_OBJ, &hook.obj);
}

static PyObject*
failmalloc_enable(PyObject *module, PyObject *args)
{
    int range = 1000;

    if (!PyArg_ParseTuple(args, "|i", &range))
        return NULL;
    if (range < 1) {
        PyErr_SetString(PyExc_ValueError,
                        "range must be greater or equal than 1");
        return NULL;
    }

    enabled = 1;
    if (!installed) {
        installed = 1;
        setup_hooks();
    }
    next_error = (size_t)(2.0 + (double)rand() * range / RAND_MAX);
    Py_RETURN_NONE;
}

static PyObject*
failmalloc_disable(PyObject *module)
{
    if (installed) {
        installed = 0;
        remove_hooks();
    }
    enabled = 0;
    Py_RETURN_NONE;
}

PyDoc_STRVAR(module_doc,
"faulthandler module.");

static PyMethodDef module_methods[] = {
    {"enable",
     (PyCFunction)failmalloc_enable, METH_VARARGS,
     PyDoc_STR("enable(range:int = 1000)")},
    {"disable",
     (PyCFunction)failmalloc_disable, METH_NOARGS,
     PyDoc_STR("disable()")},
    {NULL, NULL}  /* sentinel */
};

static struct PyModuleDef module_def = {
    PyModuleDef_HEAD_INIT,
    "faulthandler",
    module_doc,
    0, /* non-negative size to be able to unload the module */
    module_methods,
    NULL,
    NULL,
    NULL,
    NULL
};

PyMODINIT_FUNC
PyInit_failmalloc(void)
{
    PyObject *m, *version;

    m = PyModule_Create(&module_def);
    if (m == NULL)
        return NULL;

    version = PyUnicode_FromString(VERSION);
    if (version == NULL)
        return NULL;

    PyModule_AddObject(m, "__version__", version);
    return m;
}

