#include <iostream>
#include <string>
#include <sstream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using boost::asio::ip::tcp;

int main() {
  try {

	boost::asio::io_service io_service;

	tcp::resolver resolver(io_service);
	tcp::resolver::query query_meta("192.168.44.190", "2014");
	tcp::resolver::query query("192.168.44.190", "2015");
	tcp::resolver::iterator endpoint_iterator_meta = resolver.resolve(query_meta);
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	
	while(true) {
	  // Connect to the car
	  tcp::socket socket_meta(io_service);
	  tcp::socket socket(io_service);
	  boost::asio::connect(socket_meta, endpoint_iterator_meta);
	  boost::asio::connect(socket, endpoint_iterator);

	  std::string metadata, data;
	  std::vector<uchar> imgdata;

	  // Grab image metadata
	  while(true) {
		boost::array<char, 128> buf;
		boost::system::error_code error;
		socket.read_some(boost::asio::buffer(buf), error);
		if(error == boost::asio::error::eof)
		  break;
		else if(error)
		  throw boost::system::system_error(error);

		metadata += std::string(buf.begin(), buf.end());
	  }	
	  
	  // Grab image data
	  while(true) {
		boost::array<char, 128> buf;
		boost::system::error_code error;

		socket.read_some(boost::asio::buffer(buf), error);
		if(error == boost::asio::error::eof)
		  break;
		else if(error)
		  throw boost::system::system_error(error);


		std::vector<uchar> vecbuf(buf.begin(), buf.end());
		imgdata.insert(imgdata.end(), vecbuf.begin(), vecbuf.end());
	  }

	  // Convert to image
	  int channels, rows;
	  std::istringstream sstr(metadata);
	  sstr >> channels >> rows;

	  std::cout << channels << ' ' << rows << std::endl;
	  
	  Mat img(imgdata, true);
	  img.reshape(channels, rows);
	  imwrite("test.jpg", img);
	}
  } catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
	return -1;
  }

  return 0;
}
