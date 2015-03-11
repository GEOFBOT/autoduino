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

	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 2015));
	
	while(true) {
	  tcp::socket socket(io_service);
	  acceptor.accept(socket);

	  std::string metadata, data;
	  std::vector<uchar> imgdata;

	  // Grab image metadata
	  {
		boost::array<char, 128> buf;
		boost::system::error_code error;
		socket.read_some(boost::asio::buffer(buf), error);
		if(error)
		  throw boost::system::system_error(error);
	  
		metadata = std::string(buf.begin(), buf.end());
	  }

	  int size, channels, rows;
	  std::istringstream sstr(metadata);
	  sstr >> size >> channels >> rows;

	  std::cout << size << ' ' << channels << ' ' << rows << std::endl;
	  
	  // Grab image data
	  {
		boost::array<char, size> buf;
		boost::system::error_code error;
		socket.read_some(boost::asio::buffer(buf), error);
		if(error)
		  throw boost::system::system_error(error);

		std::vector<uchar> vecbuf(buf.begin(), buf.end());
		imgdata.insert(imgdata.end(), vecbuf.begin(), vecbuf.end());
	  }

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
