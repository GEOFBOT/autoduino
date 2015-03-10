#include <iostring>
#include <string>

#include <boost/asio.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using boost::asio::ip::tcp;

int main() {
  try {

	// OpenCV camera and image
	VideoCapture camera(0);
	Mat img;

	if(!camera.isOpened()) {
	  std::cerr << "Could not open camera! :0 le gasp" << std::endl;
	  return -1;
	}

	boost::asio::io_service io_service;
	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 2015));

	while(true) {
	  tcp::socket socket(io_service);
	  acceptor.accept(socket);

	  // Clear the camera buffer and capture an image
	  camera.grab(); camera.grab(); camera.grab(); camera.grab(); camera.grab();
	  camera >> img;

	  if(img.empty()) {
		std::cerr << "Frame is empty? WHAT THE F-*BOOOOOMMMM*" << std::endl;
		return -1;
	  }

	  std::string response = std::to_string(img.channels()) + " " + std::to_string(img.cols) + " " + std::to_string(img.rows) + " ";
	  response += std::string(img.data, img.channels() * img.cols * img.rows);
	  
	  boost::system::error_code err;
	  boost::asio::write(socket, boost::asio::buffer(response), err);
	}
  } catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
	return -1;
  }

  return 0;
}
