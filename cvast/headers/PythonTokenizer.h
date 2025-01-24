#ifndef PYTHON_TOKENIZER_H
#define PYTHON_TOKENIZER_H

#include <Python.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>

class PythonTokenizer {
private:
    std::vector<std::string> keywords;
    std::vector<std::string> tokens;
    std::vector<std::string> symbols;
    std::string filePath;

public:
    PythonTokenizer(
        const std::vector<std::string> &keywords,
        const std::vector<std::string> &tokens,
        const std::vector<std::string> &symbols,
        std::string filePath)
        : keywords(keywords), tokens(tokens), symbols(symbols), filePath(std::move(filePath)) {}

    void tokenize(std::vector<std::string> &tokenizedOutput,
                  std::vector<std::map<std::string, std::string>> &tokenizedDict,
                  std::vector<std::string> &tokenizedOutputWithSpaces) const {
        // Initialize the Python interpreter
        Py_Initialize();

        // Add the root path to sys.path
        // PyObject *sysPath = PySys_GetObject("path");
        // PyObject *path = PyUnicode_FromString("/Users/silicon/PycharmProjects/Vast"); // Get dynamically
        // PyList_Append(sysPath, path);
        // Py_DECREF(path);

        // Import the Python module
        PyRun_SimpleString(
            "import sys\n"
            "import os\n"
            "sys.path.append(os.path.abspath(os.path.join(os.getcwd(), '..')))"
        );

        PyObject *pModule = PyImport_ImportModule("tokenize_lexer");
        if (!pModule) {
            PyErr_Print();
            throw std::runtime_error("Failed to import module 'tokenize_lexer'.");
        }

        // Get the class 'ConvertToToken' from the module
        PyObject *pClass = PyObject_GetAttrString(pModule, "ConvertToToken");
        if (!pClass || !PyCallable_Check(pClass)) {
            PyErr_Print();
            Py_XDECREF(pModule);
            throw std::runtime_error("Failed to retrieve 'ConvertToToken' class.");
        }

        // Create Python lists for keywords, tokens, and symbols
        PyObject *pyKeywords = PyList_New(static_cast<Py_ssize_t>(keywords.size()));
        for (size_t i = 0; i < keywords.size(); i++) {
            PyList_SetItem(pyKeywords, static_cast<Py_ssize_t>(i), PyUnicode_FromString(keywords[i].c_str()));
        }

        PyObject *pyTokens = PyList_New(static_cast<Py_ssize_t>(tokens.size()));
        for (size_t i = 0; i < tokens.size(); i++) {
            PyList_SetItem(pyTokens, static_cast<Py_ssize_t>(i), PyUnicode_FromString(tokens[i].c_str()));
        }

        PyObject *pySymbols = PyList_New(static_cast<Py_ssize_t>(symbols.size()));
        for (size_t i = 0; i < symbols.size(); i++) {
            PyList_SetItem(pySymbols, static_cast<Py_ssize_t>(i), PyUnicode_FromString(symbols[i].c_str()));
        }

        PyObject *pyFilePath = PyUnicode_FromString(filePath.c_str());

        // Pack arguments into a tuple
        PyObject *args = PyTuple_Pack(4, pyKeywords, pyFilePath, pyTokens, pySymbols);
        Py_DECREF(pyKeywords);
        Py_DECREF(pyFilePath);
        Py_DECREF(pyTokens);
        Py_DECREF(pySymbols);

        // Create an instance of the class
        PyObject *pInstance = PyObject_CallObject(pClass, args);
        Py_DECREF(args);
        if (!pInstance) {
            PyErr_Print();
            Py_XDECREF(pClass);
            Py_XDECREF(pModule);
            throw std::runtime_error("Failed to create instance of 'ConvertToToken'.");
        }

        // Call the 'tokenize' method
        PyObject *pResult = PyObject_CallMethod(pInstance, "tokenize", nullptr);
        if (!pResult) {
            PyErr_Print();
            Py_DECREF(pInstance);
            Py_DECREF(pClass);
            Py_DECREF(pModule);
            throw std::runtime_error("Failed to call 'tokenize' method.");
        }

        // Process the result tuple
        PyObject *pyTokenizedOutput = PyTuple_GetItem(pResult, 0);
        PyObject *pyTokenizedDict = PyTuple_GetItem(pResult, 1);
        PyObject *pyTokenizedOutputWithSpaces = PyTuple_GetItem(pResult, 2);

        // Convert Python lists to C++ structures
        if (PyList_Check(pyTokenizedOutput)) {
            for (Py_ssize_t i = 0; i < PyList_Size(pyTokenizedOutput); i++) {
                PyObject *item = PyList_GetItem(pyTokenizedOutput, i);
                tokenizedOutput.emplace_back(PyUnicode_AsUTF8(item));
            }
        }

        if (PyList_Check(pyTokenizedDict)) {
            for (Py_ssize_t i = 0; i < PyList_Size(pyTokenizedDict); i++) {
                PyObject *dictItem = PyList_GetItem(pyTokenizedDict, i);
                if (PyDict_Check(dictItem)) {
                    std::map<std::string, std::string> entry;
                    PyObject *value = PyDict_GetItemString(dictItem, "value");
                    PyObject *line = PyDict_GetItemString(dictItem, "line");
                    PyObject *type = PyDict_GetItemString(dictItem, "type");
                    if (value) entry["value"] = PyUnicode_AsUTF8(value);
                    if (line) entry["line"] = std::to_string(PyLong_AsLong(line));
                    if (type) entry["type"] = PyUnicode_AsUTF8(type);
                    tokenizedDict.push_back(entry);
                }
            }
        }

        if (PyList_Check(pyTokenizedOutputWithSpaces)) {
            for (Py_ssize_t i = 0; i < PyList_Size(pyTokenizedOutputWithSpaces); i++) {
                PyObject *item = PyList_GetItem(pyTokenizedOutputWithSpaces, i);
                tokenizedOutputWithSpaces.emplace_back(PyUnicode_AsUTF8(item));
            }
        }

        // Cleanup
        Py_DECREF(pResult);
        Py_DECREF(pInstance);
        Py_DECREF(pClass);
        Py_DECREF(pModule);

        // Finalize the Python interpreter
        Py_Finalize();
    }
};

#endif // PYTHON_TOKENIZER_H
