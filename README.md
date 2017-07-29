# mnist-ocr

This is an OCR program that reads the [MNIST](http://yann.lecun.com/exdb/mnist/) training dataset to train the neural network using backpropagation,
then tests the correctness of the network on the testing dataset.


Build:

```bash
make
```

Usage:

```bash
./ocr [--gain=<gain_rate>] [--inertia=<inertia_weight>] [<internal_layer_size>]*
```

Example:

```bash
./ocr --gain=0.1 --inertia=0.2 30 45
```

This will run the program with a learning rate of 0.1, an inertia weight of 0.2, and two internal layers, where the first layer has 30 nodes and the second has 45 nodes.

The MNIST dataset are found [here](http://yann.lecun.com/exdb/mnist/)
