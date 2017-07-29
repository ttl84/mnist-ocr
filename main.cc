#include <iostream>
#include "stringhelper.hh"
#include "network2.hh"
#include "mnist_reader.hh"

void runAll () {
	MNIST training;
	MNIST testing;
	try {
		training = readMNIST("mnist/train-images.idx3-ubyte", "mnist/train-labels.idx1-ubyte");
		testing = readMNIST("mnist/t10k-images.idx3-ubyte", "mnist/t10k-labels.idx1-ubyte");
		//training = readMNIST("testdata/images.idx3-ubyte", "testdata/labels.idx1-ubyte");

		double score;
		double scoreX;
		double scoreY;
		double maxScore = 0;
		double bestX;
		double bestY;
		double bestPrimaryError;
		double bestSecondaryError;
		unsigned progress = training.numberOfImages()/100;
		unsigned curProgress = 0;
		for( double x = 0; x < 1; x+= 0.02){
			for( double y = 0; y < 1; y+=0.02){
				std::vector<unsigned> shape = {training.pixelsPerImage(), 40, 25, 20, 15, 10};
				Network2 nn(shape);
				score = 0;
				scoreX = 0;
				scoreY = 0;
				curProgress = 0;
				progress = training.numberOfImages()/100;
				std::cout<<"Training"<<std::endl;
				for (unsigned i = 0; i < training.numberOfImages(); i++) {
					if ( i%progress == 0){
						curProgress += 1;
						if(i != 0){
							std::cout<<'\b'<<std::flush;
							std::cout<<'\b'<<std::flush;
							if(i-10 >= 10){
								std::cout<<'\b'<<std::flush;
							}
						}
						std::cout<<curProgress<<"%"<<std::flush;
					}
					int label = training.labelAt(i);
					std::vector<double> inputPixels = training.imageAt(i);
					nn.forward(inputPixels);
					//nn.print();
					std::vector<double> expectedOutput(10);
					expectedOutput[label] = 1.0;
					nn.backward(expectedOutput,x,y);
					//std::cout << "[ " << label << " , "<<nn.prediction()<< " ] "<<std::endl;
			
					//std::cout<<"BackProp: "<<std::endl;
					//nn.print();
				}
				std::cout<<std::endl;
				std::cout<<"Training Complete"<<std::endl<<std::endl;
				std::cout<<"Testing"<<std::endl;
		
				progress = testing.numberOfImages()/100;
				curProgress = 0;
				double error = 0;
				double secondError = 0;
				for (unsigned i = 0; i < testing.numberOfImages(); i++) {
					/*if ( i%progress == 0){
						curProgress += 1;
						if(i != 0){
							std::cout<<'\b'<<std::flush;
							std::cout<<'\b'<<std::flush;
							if(i-10 >= 10){
								std::cout<<'\b'<<std::flush;
							}
						}
						std::cout<<curProgress<<"%"<<std::flush;
					}*/
					int label = testing.labelAt(i);
					std::vector<double> inputPixels = testing.imageAt(i);
					nn.forward(inputPixels);
					//nn.print();
					std::vector<double> expectedOutput(10);
					expectedOutput[label] = 1.0;
					//std::cout << "[ " << label << " , "<<nn.prediction()<< " ] ";
					if (label != nn.prediction()){
						error += 1;
						//std::cout<<" ---------ERROR---------"<<nn.secondPrediction();
						if(label != nn.secondPrediction()){
							secondError +=1;
						}
					}
					//std::cout<<std::endl;
				}
		
				//std::cout<<"Error rate: "<< error/testing.numberOfImages()*100<<"%"<<std::endl;
				//std::cout<<"Error rate second best: "<< secondError/error*100<<"%"<<std::endl;
				std::cout<<"Testing Complete"<<std::endl;
				scoreX = 100 - error/testing.numberOfImages()*100;
				scoreY = 0.5*(100 - secondError/error*100);
				score = scoreX + scoreY;
				
				if (score > maxScore){
					maxScore = score;
					bestX = x;
					bestY = y;
					bestPrimaryError = error/testing.numberOfImages()*100;
					bestSecondaryError = secondError/error*100;
				} 
			}
		}
		std::cout << "Best [gain,alpha]: [" <<bestX<< "," << bestY<<"]"<<std::endl;
		std::cout << "Primary Error: " <<bestPrimaryError<<std::endl;
		std::cout << "Secondary Error: " <<bestSecondaryError<<std::endl;
		
	} catch (std::string& e) {
		std::cout << e << '\n';
	} catch (char const* e) {
		std::cout << e << '\n';
	}
}
void run(double gain, double inertia, std::vector<unsigned> shape) {
	MNIST training;
	MNIST testing;

	try {
		training = readMNIST("mnist/train-images.idx3-ubyte", "mnist/train-labels.idx1-ubyte");
		testing = readMNIST("mnist/t10k-images.idx3-ubyte", "mnist/t10k-labels.idx1-ubyte");

		unsigned progress = training.numberOfImages()/100;
		unsigned curProgress = 0;

		shape.insert(shape.begin(), training.pixelsPerImage());
		shape.push_back(10);
		Network2 nn(shape);

		progress = training.numberOfImages()/100;
		std::cout<<"Training"<<std::endl;
		for (unsigned i = 0; i < training.numberOfImages(); i++) {
			if ( i%progress == 0){
				curProgress += 1;
				if(i != 0){
					std::cout<<'\b'<<std::flush;
					std::cout<<'\b'<<std::flush;
					if(i-10 >= 10){
						std::cout<<'\b'<<std::flush;
					}
				}
				std::cout<<curProgress<<"%"<<std::flush;
			}
			int label = training.labelAt(i);
			std::vector<double> inputPixels = training.imageAt(i);
			nn.forward(inputPixels);
			//nn.print();
			std::vector<double> expectedOutput(10);
			expectedOutput[label] = 1.0;
			nn.backward(expectedOutput,gain,inertia);
			//std::cout << "[ " << label << " , "<<nn.prediction()<< " ] "<<std::endl;
	
			//std::cout<<"BackProp: "<<std::endl;
			//nn.print();
		}
		std::cout<<std::endl;
		std::cout<<"Training Complete"<<std::endl<<std::endl;
		std::cout<<"Testing"<<std::endl;

		double error = 0;
		double secondError = 0;
		for (unsigned i = 0; i < testing.numberOfImages(); i++) {
			int label = testing.labelAt(i);
			std::vector<double> inputPixels = testing.imageAt(i);
			nn.forward(inputPixels);
			//nn.print();
			std::vector<double> expectedOutput(10);
			expectedOutput[label] = 1.0;
			std::cout << "[ " << label << " , "<<nn.prediction()<< " ] ";
			if (label != nn.prediction()){
				error += 1;
				std::cout<<" ---------ERROR---------"<<nn.secondPrediction();
				if(label != nn.secondPrediction()){
					secondError +=1;
				}
				//std::cout << ' ' << std::flush;
			} else {
				//std::cout << '@' << std::flush;
			}
			std::cout<<std::endl;
		}
		std::cout << '\n';

		//std::cout<<"Error rate: "<< error/testing.numberOfImages()*100<<"%"<<std::endl;
		//std::cout<<"Error rate second best: "<< secondError/error*100<<"%"<<std::endl;
		std::cout<<"Testing Complete"<<std::endl;
		std::cout << "error rate: " << error / testing.numberOfImages() << '\n';
		std::cout << "second best error rate: " << secondError / error << '\n';	
	} catch (std::string& e) {
		std::cout << e << '\n';
	} catch (char const* e) {
		std::cout << e << '\n';
	}
}
int main(int argc, char ** argv) {
	double gain = 0.1;
	double inertia = 0.2;
	bool all = false;
	std::vector<unsigned> shape;

	for (int i = 0; i < argc; i++) {
		std::string option(argv[i]);
		std::string param;
		unsigned layerSize;
		if (stripPrefix(option, "--gain=", param)) {
			std::istringstream(param) >> gain;
			
		} else if (stripPrefix(option, "--inertia=", param)) {
			std::istringstream(param) >> inertia;
			
		} else if (option == "--all") {
			all = true;
		} else if(std::istringstream(option) >> layerSize) {
			shape.push_back(layerSize);
		}
	}

	

	if (all) {
		runAll();
	} else {
		if (shape.size() == 0) {
			shape = {30, 25};
		}
		std::cout << "gain: " << gain << '\n';
		std::cout << "inertia: " << inertia << '\n';
		std::cout << "shape: input ";
		for (auto size: shape) {
			std::cout << size << ' ';
		}
		std::cout << "output\n";
		run(gain, inertia, shape);
	}
	return 0;
}
