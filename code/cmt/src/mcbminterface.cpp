#include "mcbminterface.h"
#include "Eigen/Core"
#include "exception.h"

#include <iostream>

using namespace Eigen;

MCBM::Parameters PyObject_ToParameters(MCBMObject* self, PyObject* parameters) {
	MCBM::Parameters params;

	// read parameters from dictionary
	if(parameters && parameters != Py_None) {
		if(!PyDict_Check(parameters))
			throw Exception("Parameters should be stored in a dictionary.");

		PyObject* verbosity = PyDict_GetItemString(parameters, "verbosity");
		if(verbosity)
			if(PyInt_Check(verbosity))
				params.verbosity = PyInt_AsLong(verbosity);
			else if(PyFloat_Check(verbosity))
				params.verbosity = static_cast<int>(PyFloat_AsDouble(verbosity));
			else
				throw Exception("verbosity should be of type `int`.");

		PyObject* max_iter = PyDict_GetItemString(parameters, "max_iter");
		if(max_iter)
			if(PyInt_Check(max_iter))
				params.maxIter = PyInt_AsLong(max_iter);
			else if(PyFloat_Check(max_iter))
				params.maxIter = static_cast<int>(PyFloat_AsDouble(max_iter));
			else
				throw Exception("max_iter should be of type `int`.");
		
		PyObject* threshold = PyDict_GetItemString(parameters, "threshold");
		if(threshold)
			if(PyFloat_Check(threshold))
				params.threshold = PyFloat_AsDouble(threshold);
			else if(PyInt_Check(threshold))
				params.threshold = static_cast<double>(PyFloat_AsDouble(threshold));
			else
				throw Exception("threshold should be of type `float`.");

		PyObject* num_grad = PyDict_GetItemString(parameters, "num_grad");
		if(num_grad)
			if(PyInt_Check(num_grad))
				params.numGrad = PyInt_AsLong(num_grad);
			else if(PyFloat_Check(num_grad))
				params.numGrad = static_cast<int>(PyFloat_AsDouble(num_grad));
			else
				throw Exception("num_grad should be of type `int`.");

		PyObject* batch_size = PyDict_GetItemString(parameters, "batch_size");
		if(batch_size)
			if(PyInt_Check(batch_size))
				params.batchSize = PyInt_AsLong(batch_size);
			else if(PyFloat_Check(batch_size))
				params.batchSize = static_cast<int>(PyFloat_AsDouble(batch_size));
			else
				throw Exception("batch_size should be of type `int`.");

//		PyObject* callback = PyDict_GetItemString(parameters, "callback");
//		if(callback)
//			if(PyCallable_Check(callback))
//				params.callback = new CallbackTrain(self, callback);
//			else if(callback != Py_None)
//				throw Exception("callback should be a function or callable object.");

		PyObject* cb_iter = PyDict_GetItemString(parameters, "cb_iter");
		if(cb_iter)
			if(PyInt_Check(cb_iter))
				params.cbIter = PyInt_AsLong(cb_iter);
			else if(PyFloat_Check(cb_iter))
				params.cbIter = static_cast<int>(PyFloat_AsDouble(cb_iter));
			else
				throw Exception("cb_iter should be of type `int`.");

		PyObject* train_priors = PyDict_GetItemString(parameters, "train_priors");
		if(train_priors)
			if(PyBool_Check(train_priors))
				params.trainPriors = (train_priors == Py_True);
			else
				throw Exception("train_priors should be of type `bool`.");

		PyObject* train_weights = PyDict_GetItemString(parameters, "train_weights");
		if(train_weights)
			if(PyBool_Check(train_weights))
				params.trainWeights = (train_weights == Py_True);
			else
				throw Exception("train_weights should be of type `bool`.");

		PyObject* train_features = PyDict_GetItemString(parameters, "train_features");
		if(train_features)
			if(PyBool_Check(train_features))
				params.trainFeatures = (train_features == Py_True);
			else
				throw Exception("train_features should be of type `bool`.");

		PyObject* train_predictors = PyDict_GetItemString(parameters, "train_predictors");
		if(train_predictors)
			if(PyBool_Check(train_predictors))
				params.trainPredictors = (train_predictors == Py_True);
			else
				throw Exception("train_predictors should be of type `bool`.");

		PyObject* train_input_bias = PyDict_GetItemString(parameters, "train_input_bias");
		if(train_input_bias)
			if(PyBool_Check(train_input_bias))
				params.trainInputBias = (train_input_bias == Py_True);
			else
				throw Exception("train_input_bias should be of type `bool`.");

		PyObject* train_output_bias = PyDict_GetItemString(parameters, "train_output_bias");
		if(train_output_bias)
			if(PyBool_Check(train_output_bias))
				params.trainOutputBias = (train_output_bias == Py_True);
			else
				throw Exception("train_output_bias should be of type `bool`.");

		PyObject* regularize_features = PyDict_GetItemString(parameters, "regularize_features");
		if(regularize_features)
			if(PyFloat_Check(regularize_features))
				params.regularizeFeatures = PyFloat_AsDouble(regularize_features);
			else if(PyInt_Check(regularize_features))
				params.regularizeFeatures = static_cast<double>(PyFloat_AsDouble(regularize_features));
			else
				throw Exception("regularize_features should be of type `float`.");

		PyObject* regularize_predictors = PyDict_GetItemString(parameters, "regularize_predictors");
		if(regularize_predictors)
			if(PyFloat_Check(regularize_predictors))
				params.regularizePredictors = PyFloat_AsDouble(regularize_predictors);
			else if(PyInt_Check(regularize_predictors))
				params.regularizePredictors = static_cast<double>(PyFloat_AsDouble(regularize_predictors));
			else
				throw Exception("regularize_predictors should be of type `float`.");
	}

	return params;
}



PyObject* MCBM_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
	PyObject* self = type->tp_alloc(type, 0);

	if(self)
		reinterpret_cast<MCBMObject*>(self)->mcbm = 0;

	return self;
}


const char* MCBM_doc =
	"An implementation of a mixture of conditional Boltzmann machines.\n"
	"\n"
	"The distribution defined by the model is\n"
	"\n"
	"$$p(\\mathbf{y} \\mid \\mathbf{x}) \\propto \\sum_{c} \\exp\\left(\\eta_c + \\sum_i \\beta_{ci} \\left(\\mathbf{b}_i^\\top \\mathbf{x}\\right)^2 + \\mathbf{w}_c^\\top \\mathbf{x} + \\mathbf{y}_c^\\top \\mathbf{A}_c \\mathbf{x} + v_c y\\right),$$\n"
	"\n"
	"where $\\mathbf{x} \\in \\{0, 1\\}^N$ and $y \\in \\{0, 1\\}$.\n"
	"\n"
	"To create an MCBM with $N$-dimensional inputs and, for example, 8 components and 100 features $\\mathbf{b}_i$, use\n"
	"\n"
	"\t>>> mcbm = MCBM(N, 8, 100)\n"
	"\n"
	"To access the different parameters, you can use\n"
	"\n"
	"\t>>> mcbm.priors\n"
	"\t>>> mcbm.weights\n"
	"\t>>> mcbm.features\n"
	"\t>>> mcbm.predictors\n"
	"\t>>> mcbm.input_bias\n"
	"\t>>> mcbm.output_bias\n"
	"\n"
	"which correspond to $\\eta_{c}$, $\\beta_{ci}$, $\\mathbf{b}_i$, $\\mathbf{A}_c$, $\\mathbf{w}_c$,"
	"and $v_c$, respectively.\n"
	"\n"
	"@type  dim_in: integer\n"
	"@param dim_in: dimensionality of input\n"
	"\n"
	"@type  num_components: integer\n"
	"@param num_components: number of components\n"
	"\n"
	"@type  num_features: integer\n"
	"@param num_features: number of features used to approximate input covariance matrices";

int MCBM_init(MCBMObject* self, PyObject* args, PyObject* kwds) {
	const char* kwlist[] = {"dim_in", "num_components", "num_features", 0};

	int dim_in;
	int num_components = 8;
	int num_features = 0;

	// read arguments
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "i|ii", const_cast<char**>(kwlist),
		&dim_in, &num_components, &num_features))
		return -1;

	if(!num_features)
		num_features = dim_in;

	// create actual MCBM instance
	try {
		self->mcbm = new MCBM(dim_in, num_components, num_features);
	} catch(Exception exception) {
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return -1;
	}

	return 0;
}



void MCBM_dealloc(MCBMObject* self) {
	// delete actual instance
	delete self->mcbm;

	// delete MCBMObject
	self->ob_type->tp_free(reinterpret_cast<PyObject*>(self));
}



PyObject* MCBM_dim_in(MCBMObject* self, PyObject*, void*) {
	return PyInt_FromLong(self->mcbm->dimIn());
}



PyObject* MCBM_dim_out(MCBMObject* self, PyObject*, void*) {
	return PyInt_FromLong(self->mcbm->dimOut());
}



PyObject* MCBM_num_components(MCBMObject* self, PyObject*, void*) {
	return PyInt_FromLong(self->mcbm->numComponents());
}



PyObject* MCBM_num_features(MCBMObject* self, PyObject*, void*) {
	return PyInt_FromLong(self->mcbm->numFeatures());
}



PyObject* MCBM_priors(MCBMObject* self, PyObject*, void*) {
	PyObject* array = PyArray_FromMatrixXd(self->mcbm->priors());

	// make array immutable
	reinterpret_cast<PyArrayObject*>(array)->flags &= ~NPY_WRITEABLE;

	return array;
}



int MCBM_set_priors(MCBMObject* self, PyObject* value, void*) {
	value = PyArray_FROM_OTF(value, NPY_DOUBLE, NPY_IN_ARRAY);

	if(!value) {
		PyErr_SetString(PyExc_TypeError, "Priors should be of type `ndarray`.");
		return -1;
	}

	try {
		self->mcbm->setPriors(PyArray_ToMatrixXd(value));
	} catch(Exception exception) {
		Py_DECREF(value);
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return -1;
	}

	Py_DECREF(value);

	return 0;
}



PyObject* MCBM_weights(MCBMObject* self, PyObject*, void*) {
	PyObject* array = PyArray_FromMatrixXd(self->mcbm->weights());

	// make array immutable
	reinterpret_cast<PyArrayObject*>(array)->flags &= ~NPY_WRITEABLE;

	return array;
}



int MCBM_set_weights(MCBMObject* self, PyObject* value, void*) {
	value = PyArray_FROM_OTF(value, NPY_DOUBLE, NPY_IN_ARRAY);

	if(!value) {
		PyErr_SetString(PyExc_TypeError, "Weights should be of type `ndarray`.");
		return -1;
	}

	try {
		self->mcbm->setWeights(PyArray_ToMatrixXd(value));
	} catch(Exception exception) {
		Py_DECREF(value);
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return -1;
	}

	Py_DECREF(value);

	return 0;
}



PyObject* MCBM_features(MCBMObject* self, PyObject*, void*) {
	PyObject* array = PyArray_FromMatrixXd(self->mcbm->features());

	// make array immutable
	reinterpret_cast<PyArrayObject*>(array)->flags &= ~NPY_WRITEABLE;

	return array;
}



int MCBM_set_features(MCBMObject* self, PyObject* value, void*) {
	value = PyArray_FROM_OTF(value, NPY_DOUBLE, NPY_IN_ARRAY);

	if(!value) {
		PyErr_SetString(PyExc_TypeError, "Features should be of type `ndarray`.");
		return -1;
	}

	try {
		self->mcbm->setFeatures(PyArray_ToMatrixXd(value));
	} catch(Exception exception) {
		Py_DECREF(value);
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return -1;
	}

	Py_DECREF(value);

	return 0;
}



PyObject* MCBM_predictors(MCBMObject* self, PyObject*, void*) {
	PyObject* array = PyArray_FromMatrixXd(self->mcbm->predictors());

	// make array immutable
	reinterpret_cast<PyArrayObject*>(array)->flags &= ~NPY_WRITEABLE;

	return array;
}



int MCBM_set_predictors(MCBMObject* self, PyObject* value, void*) {
	value = PyArray_FROM_OTF(value, NPY_DOUBLE, NPY_IN_ARRAY);

	if(!value) {
		PyErr_SetString(PyExc_TypeError, "Predictors should be of type `ndarray`.");
		return -1;
	}

	try {
		self->mcbm->setPredictors(PyArray_ToMatrixXd(value));
	} catch(Exception exception) {
		Py_DECREF(value);
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return -1;
	}

	Py_DECREF(value);

	return 0;
}



PyObject* MCBM_input_bias(MCBMObject* self, PyObject*, void*) {
	PyObject* array = PyArray_FromMatrixXd(self->mcbm->inputBias());

	// make array immutable
	reinterpret_cast<PyArrayObject*>(array)->flags &= ~NPY_WRITEABLE;

	return array;
}



int MCBM_set_input_bias(MCBMObject* self, PyObject* value, void*) {
	value = PyArray_FROM_OTF(value, NPY_DOUBLE, NPY_IN_ARRAY);

	if(!value) {
		PyErr_SetString(PyExc_TypeError, "Bias vectors should be of type `ndarray`.");
		return -1;
	}

	try {
		self->mcbm->setInputBias(PyArray_ToMatrixXd(value));
	} catch(Exception exception) {
		Py_DECREF(value);
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return -1;
	}

	Py_DECREF(value);

	return 0;
}



PyObject* MCBM_output_bias(MCBMObject* self, PyObject*, void*) {
	PyObject* array = PyArray_FromMatrixXd(self->mcbm->outputBias());

	// make array immutable
	reinterpret_cast<PyArrayObject*>(array)->flags &= ~NPY_WRITEABLE;

	return array;
}



int MCBM_set_output_bias(MCBMObject* self, PyObject* value, void*) {
	value = PyArray_FROM_OTF(value, NPY_DOUBLE, NPY_IN_ARRAY);

	if(!value) {
		PyErr_SetString(PyExc_TypeError, "Bias vectors should be of type `ndarray`.");
		return -1;
	}

	try {
		self->mcbm->setOutputBias(PyArray_ToMatrixXd(value));
	} catch(Exception exception) {
		Py_DECREF(value);
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return -1;
	}

	Py_DECREF(value);

	return 0;
}



const char* MCBM_sample_doc =
	"sample(self, input)\n"
	"\n"
	"Generates outputs for given inputs.\n"
	"\n"
	"@type  input: ndarray\n"
	"@param input: inputs stored in columns\n"
	"\n"
	"@rtype: ndarray\n"
	"@return: sampled outputs";

PyObject* MCBM_sample(MCBMObject* self, PyObject* args, PyObject* kwds) {
	const char* kwlist[] = {"input", 0};

	PyObject* input;

	if(!PyArg_ParseTupleAndKeywords(args, kwds, "O", const_cast<char**>(kwlist), &input))
		return 0;

	input = PyArray_FROM_OTF(input, NPY_INT64, NPY_F_CONTIGUOUS | NPY_ALIGNED);

	if(!input) {
		PyErr_SetString(PyExc_TypeError, "Data has to be stored in a NumPy array.");
		return 0;
	}

	try {
		PyObject* result = PyArray_FromMatrixXi(self->mcbm->sample(PyArray_ToMatrixXi(input)));
		Py_DECREF(input);
		return result;
	} catch(Exception exception) {
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return 0;
	}

	return 0;
}



const char* MCBM_loglikelihood_doc =
	"loglikelihood(self, input, output)\n"
	"\n"
	"Computes the conditional log-likelihood for the given data points in nats.\n"
	"\n"
	"@type  input: ndarray\n"
	"@param input: inputs stored in columns\n"
	"\n"
	"@type  output: ndarray\n"
	"@param output: outputs stored in columns\n"
	"\n"
	"@rtype: ndarray\n"
	"@return: log-likelihood of the model evaluated for each data point";

PyObject* MCBM_loglikelihood(MCBMObject* self, PyObject* args, PyObject* kwds) {
	const char* kwlist[] = {"input", "output", 0};

	PyObject* input;
	PyObject* output;

	// read arguments
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "OO", const_cast<char**>(kwlist), &input, &output))
		return 0;

	// make sure data is stored in NumPy array
	input = PyArray_FROM_OTF(input, NPY_INT64, NPY_F_CONTIGUOUS | NPY_ALIGNED);
	output = PyArray_FROM_OTF(output, NPY_INT64, NPY_F_CONTIGUOUS | NPY_ALIGNED);

	if(!input || !output) {
		PyErr_SetString(PyExc_TypeError, "Data has to be stored in NumPy arrays.");
		return 0;
	}

	try {
		PyObject* result = PyArray_FromMatrixXd(
			self->mcbm->logLikelihood(PyArray_ToMatrixXi(input), PyArray_ToMatrixXi(output)));
		Py_DECREF(input);
		Py_DECREF(output);
		return result;
	} catch(Exception exception) {
		Py_DECREF(input);
		Py_DECREF(output);
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return 0;
	}

	return 0;
}



const char* MCBM_evaluate_doc =
	"evaluate(self, input, output)\n"
	"\n"
	"Computes the average negative conditional log-likelihood for the given data points "
	"in bits per output component (smaller is better).\n"
	"\n"
	"@type  input: ndarray\n"
	"@param input: inputs stored in columns\n"
	"\n"
	"@type  output: ndarray\n"
	"@param output: outputs stored in columns\n"
	"\n"
	"@rtype: double\n"
	"@return: performance in bits per component";

PyObject* MCBM_evaluate(MCBMObject* self, PyObject* args, PyObject* kwds) {
	const char* kwlist[] = {"input", "output", 0};

	PyObject* input;
	PyObject* output;

	// read arguments
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "OO", const_cast<char**>(kwlist), &input, &output))
		return 0;

	// make sure data is stored in NumPy array
	input = PyArray_FROM_OTF(input, NPY_INT64, NPY_F_CONTIGUOUS | NPY_ALIGNED);
	output = PyArray_FROM_OTF(output, NPY_INT64, NPY_F_CONTIGUOUS | NPY_ALIGNED);

	if(!input || !output) {
		PyErr_SetString(PyExc_TypeError, "Data has to be stored in NumPy arrays.");
		return 0;
	}

	try {
		double result = 
			self->mcbm->evaluate(PyArray_ToMatrixXi(input), PyArray_ToMatrixXi(output));
		Py_DECREF(input);
		Py_DECREF(output);
		return PyFloat_FromDouble(result);
	} catch(Exception exception) {
		Py_DECREF(input);
		Py_DECREF(output);
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return 0;
	}

	return 0;
}



const char* MCBM_train_doc =
	"train(self, input, output, parameters=None)\n"
	"\n"
	"Fits model parameters to given data using L-BFGS.\n"
	"\n"
	"The following example demonstrates possible parameters and default settings.\n"
	"\n"
	"\t>>> model.train(input, output, parameters={\n"
	"\t>>> \t'verbosity': 0\n"
	"\t>>> \t'max_iter': 1000\n"
	"\t>>> \t'threshold': 1e-5\n"
	"\t>>> \t'num_grad': 20\n"
	"\t>>> \t'batch_size': 2000\n"
	"\t>>> \t'callback': None\n"
	"\t>>> \t'cb_iter': 25\n"
	"\t>>> \t'train_priors': True\n"
	"\t>>> \t'train_weights': True\n"
	"\t>>> \t'train_features': True\n"
	"\t>>> \t'train_predictors': True\n"
	"\t>>> \t'train_input_bias': True\n"
	"\t>>> \t'train_output_bias': True\n"
	"\t>>> \t'regularize_features': 0.\n"
	"\t>>> \t'regularize_predictors': 0.\n"
	"\t>>> })\n"
	"\n"
	"The parameters C{train_priors}, C{train_weights}, and so on can be used to control which "
	"parameters will be optimized. Optimization stops after C{max_iter} iterations or if "
	"the gradient is sufficiently small enough, as specified by C{threshold}."
	"C{num_grad} is the number of gradients used by L-BFGS to approximate the inverse Hessian "
	"matrix.\n"
	"\n"
	"The parameter C{batch_size} has no effect on the solution of the optimization but "
	"can affect speed by reducing the number of cache misses.\n"
	"\n"
	"If a callback function is given, it will be called every C{cb_iter} iterations. The first "
	"argument to callback will be the current iteration, the second argument will be a I{copy} of "
	"the model.\n"
	"\n"
	"\t>>> def callback(i, mcbm):\n"
	"\t>>> \tprint i\n"
	"\n"
	"@type  input: ndarray\n"
	"@param input: inputs stored in columns\n"
	"\n"
	"@type  output: ndarray\n"
	"@param output: outputs stored in columns\n"
	"\n"
	"@type  parameters: dict\n"
	"@param parameters: a dictionary containing hyperparameters\n"
	"\n"
	"@rtype: bool\n"
	"@return: C{True} if training converged, otherwise C{False}";

PyObject* MCBM_train(MCBMObject* self, PyObject* args, PyObject* kwds) {
	const char* kwlist[] = {"input", "output", "parameters", 0};

	PyObject* input;
	PyObject* output;
	PyObject* parameters = 0;

	// read arguments
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "OO|O", const_cast<char**>(kwlist),
		&input,
		&output,
		&parameters))
		return 0;

	// make sure data is stored in NumPy array
	input = PyArray_FROM_OTF(input, NPY_INT64, NPY_F_CONTIGUOUS | NPY_ALIGNED);
	output = PyArray_FROM_OTF(output, NPY_INT64, NPY_F_CONTIGUOUS | NPY_ALIGNED);

	if(!input || !output) {
		Py_XDECREF(input);
		Py_XDECREF(output);
		PyErr_SetString(PyExc_TypeError, "Data has to be stored in NumPy arrays.");
		return 0;
	}

	try {
		if(self->mcbm->train(
				PyArray_ToMatrixXi(input), 
				PyArray_ToMatrixXi(output), 
				PyObject_ToParameters(self, parameters)))
		{
			Py_DECREF(input);
			Py_DECREF(output);
			Py_INCREF(Py_True);
			return Py_True;
		} else {
			Py_DECREF(input);
			Py_DECREF(output);
			Py_INCREF(Py_False);
			return Py_False;
		}
	} catch(Exception exception) {
		Py_DECREF(input);
		Py_DECREF(output);
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return 0;
	}

	return 0;
}



const char* MCBM_parameters_doc =
	"parameters(self, parameters=None)\n"
	"\n"
	"Summarizes the parameters of the model in a long vector.\n"
	"\n"
	"If C{parameters} is given, only the parameters with C{train_* = True} will be contained "
	"in the vector.\n"
	"\n"
	"@type  parameters: dict\n"
	"@param parameters: a dictionary containing hyperparameters\n"
	"\n"
	"@rtype: ndarray\n"
	"@return: model parameters vectorized and concatenated";

PyObject* MCBM_parameters(MCBMObject* self, PyObject* args, PyObject* kwds) {
	const char* kwlist[] = {"parameters", 0};

	PyObject* parameters = 0;

	// read arguments
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "|O", const_cast<char**>(kwlist), &parameters))
		return 0;

	try {
		MCBM::Parameters params = PyObject_ToParameters(self, parameters);

		lbfgsfloatval_t* x = self->mcbm->parameters(params);

		PyObject* xObj = PyArray_FromMatrixXd(
			Map<Matrix<lbfgsfloatval_t, Dynamic, Dynamic> >(
				x, self->mcbm->numParameters(params), 1));

		lbfgs_free(x);

		return xObj;
	} catch(Exception exception) {
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return 0;
	}

	return 0;
}



const char* MCBM_set_parameters_doc =
	"set_parameters(self, x, parameters=None)\n"
	"\n"
	"Loads all model parameters from a vector as produced by L{parameters()}.\n"
	"\n"
	"@type  x: ndarray\n"
	"@param x: all model parameters concatenated to a vector\n"
	"\n"
	"@type  parameters: dict\n"
	"@param parameters: a dictionary containing hyperparameters";

PyObject* MCBM_set_parameters(MCBMObject* self, PyObject* args, PyObject* kwds) {
	const char* kwlist[] = {"x", "parameters", 0};

	PyObject* x;
	PyObject* parameters = 0;

	// read arguments
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "O|O", const_cast<char**>(kwlist),
		&x,
		&parameters))
		return 0;

	x = PyArray_FROM_OTF(x, NPY_DOUBLE, NPY_F_CONTIGUOUS | NPY_ALIGNED);

	if(!x) {
		PyErr_SetString(PyExc_TypeError, "Parameters have to be stored in NumPy arrays.");
		return 0;
	}

	try {
		self->mcbm->setParameters(
			PyArray_ToMatrixXd(x).data(), // TODO: PyArray_ToMatrixXd unnecessary
			PyObject_ToParameters(self, parameters));

		Py_DECREF(x);
		Py_INCREF(Py_None);

		return Py_None;
	} catch(Exception exception) {
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		Py_DECREF(x);
		return 0;
	}

	return 0;
}



PyObject* MCBM_compute_gradient(MCBMObject* self, PyObject* args, PyObject* kwds) {
	const char* kwlist[] = {"input", "output", "x", "parameters", 0};

	PyObject* input;
	PyObject* output;
	PyObject* x = 0;
	PyObject* parameters = 0;

	// read arguments
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "OO|OO", const_cast<char**>(kwlist),
		&input,
		&output,
		&x,
		&parameters))
		return 0;

	// make sure data is stored in NumPy array
	input = PyArray_FROM_OTF(input, NPY_INT64, NPY_F_CONTIGUOUS | NPY_ALIGNED);
	output = PyArray_FROM_OTF(output, NPY_INT64, NPY_F_CONTIGUOUS | NPY_ALIGNED);

	if(!input || !output) {
		Py_XDECREF(input);
		Py_XDECREF(output);
		PyErr_SetString(PyExc_TypeError, "Data has to be stored in NumPy arrays.");
		return 0;
	}

	if(x)
		x = PyArray_FROM_OTF(x, NPY_DOUBLE, NPY_F_CONTIGUOUS | NPY_ALIGNED);

	try {
		MCBM::Parameters params = PyObject_ToParameters(self, parameters);

		MatrixXd gradient(self->mcbm->numParameters(params), 1); // TODO: don't use MatrixXd

		if(x)
			self->mcbm->computeGradient(
				PyArray_ToMatrixXi(input).cast<double>(), 
				PyArray_ToMatrixXi(output).cast<double>(), 
				PyArray_ToMatrixXd(x).data(), // TODO: PyArray_ToMatrixXd unnecessary
				gradient.data(),
				params);
		else
			self->mcbm->computeGradient(
				PyArray_ToMatrixXi(input).cast<double>(), 
				PyArray_ToMatrixXi(output).cast<double>(), 
				self->mcbm->parameters(params),
				gradient.data(),
				params);

		Py_DECREF(input);
		Py_DECREF(output);
		Py_XDECREF(x);

		return PyArray_FromMatrixXd(gradient);
	} catch(Exception exception) {
		Py_DECREF(input);
		Py_DECREF(output);
		Py_XDECREF(x);
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return 0;
	}
}



PyObject* MCBM_check_gradient(MCBMObject* self, PyObject* args, PyObject* kwds) {
	const char* kwlist[] = {"input", "output", "epsilon", "parameters", 0};

	PyObject* input;
	PyObject* output;
	double epsilon = 1e-5;
	PyObject* parameters = 0;

	// read arguments
	if(!PyArg_ParseTupleAndKeywords(args, kwds, "OO|dO", const_cast<char**>(kwlist),
		&input,
		&output,
		&epsilon,
		&parameters))
		return 0;

	// make sure data is stored in NumPy array
	input = PyArray_FROM_OTF(input, NPY_INT64, NPY_F_CONTIGUOUS | NPY_ALIGNED);
	output = PyArray_FROM_OTF(output, NPY_INT64, NPY_F_CONTIGUOUS | NPY_ALIGNED);

	if(!input || !output) {
		Py_XDECREF(input);
		Py_XDECREF(output);
		PyErr_SetString(PyExc_TypeError, "Data has to be stored in NumPy arrays.");
		return 0;
	}

	try {
		double err = self->mcbm->checkGradient(
			PyArray_ToMatrixXi(input).cast<double>(),
			PyArray_ToMatrixXi(output).cast<double>(),
			epsilon,
			PyObject_ToParameters(self, parameters));
		Py_DECREF(input);
		Py_DECREF(output);
		return PyFloat_FromDouble(err);
	} catch(Exception exception) {
		Py_DECREF(input);
		Py_DECREF(output);
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return 0;
	}

	return 0;
}



const char* MCBM_reduce_doc =
	"__reduce__(self)\n"
	"\n"
	"Method used by Pickle.";

PyObject* MCBM_reduce(MCBMObject* self, PyObject*, PyObject*) {
	PyObject* args = Py_BuildValue("(iii)", 
		self->mcbm->dimIn(),
		self->mcbm->numComponents(),
		self->mcbm->numFeatures());

	PyObject* priors = MCBM_priors(self, 0, 0);
	PyObject* weights = MCBM_weights(self, 0, 0);
	PyObject* features = MCBM_features(self, 0, 0);
	PyObject* predictors = MCBM_predictors(self, 0, 0);
	PyObject* input_bias = MCBM_input_bias(self, 0, 0);
	PyObject* output_bias = MCBM_output_bias(self, 0, 0);
	PyObject* state = Py_BuildValue("(OOOOOO)", 
		priors, weights, features, predictors, input_bias, output_bias);
	Py_DECREF(priors);
	Py_DECREF(weights);
	Py_DECREF(features);
	Py_DECREF(predictors);
	Py_DECREF(input_bias);
	Py_DECREF(output_bias);

	PyObject* result = Py_BuildValue("(OOO)", self->ob_type, args, state);
	Py_DECREF(args);
	Py_DECREF(state);

	return result;
}



const char* MCBM_setstate_doc =
	"__setstate__(self)\n"
	"\n"
	"Method used by Pickle.";

PyObject* MCBM_setstate(MCBMObject* self, PyObject* state, PyObject*) {
	PyObject* priors;
	PyObject* weights;
	PyObject* features;
	PyObject* predictors;
	PyObject* input_bias;
	PyObject* output_bias;

	if(!PyArg_ParseTuple(state, "(OOOOOO)",
		&priors, &weights, &features, &predictors, &input_bias, &output_bias))
		return 0;

	try {
		MCBM_set_priors(self, priors, 0);
		MCBM_set_weights(self, weights, 0);
		MCBM_set_features(self, features, 0);
		MCBM_set_predictors(self, predictors, 0);
		MCBM_set_input_bias(self, input_bias, 0);
		MCBM_set_output_bias(self, output_bias, 0);
	} catch(Exception exception) {
		PyErr_SetString(PyExc_RuntimeError, exception.message());
		return 0;
	}

	Py_INCREF(Py_None);
	return Py_None;
}
