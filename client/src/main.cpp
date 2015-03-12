#include <iostream>
#include <string>
#include <sstream>

#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <opencv2/opencv.hpp>

#include <cvmat_serialization.h>

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

	camera.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
	camera.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);

	boost::asio::io_service io_service;
	tcp::resolver resolver(io_service);
	tcp::resolver::query query("192.168.1.2", "2015");
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

	while(true) {
	  tcp::socket socket(io_service);
	  boost::asio::connect(socket, endpoint_iterator);

	  // Clear the camera buffer and capture an image
	  camera.grab(); camera.grab(); camera.grab(); camera.grab(); camera.grab();
	  camera >> img;

	  if(img.empty()) {
		std::cerr << "Frame is empty? WHAT THE F-*BOOOOOMMMM*" << std::endl;
		return -1;
	  }

	  std::stringstream sstr;
	  boost::archive::text_oarchive oa(sstr);
	  oa << img;

	  std::string data(sstr.str());

	  boost::system::error_code err;
	  socket.write_some(boost::asio::buffer(data), err);
	  /*std::string metadata = std::to_string(img.total() * img.elemSize()) + " " + std::to_string(img.channels()) + " " + std::to_string(img.rows) + " ";
	  while(metadata.size() <= 128)
		metadata += ' ';
	  
	  img = img.reshape(0, 1);
	  std::string imgdata = std::string((char *)img.data, img.total() * img.elemSize());
	  
	  std::cout << "Connected to port 2015. Channels, rows: " << metadata << ' ' << img.rows << std::endl;
	  boost::system::error_code err;
	  socket.write_some(boost::asio::buffer(metadata + imgdata), err);
	  */
	}
  } catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
	return -1;
  }

  return 0;
}
