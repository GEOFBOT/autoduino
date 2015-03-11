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
	tcp::resolver::query query("192.168.44.190", "2015");
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

	namedWindow("Image");
	
	while(true) {
	  // Connect to the car
	  tcp::socket socket(io_service);
	  boost::asio::connect(socket, endpoint_iterator);

	  std::string data, imgdata_str;

	  // Grab image data
	  while(true) {
		boost::array<char, 128> buf;
		boost::system::error_code error;

		socket.read_some(boost::asio::buffer(buf), error);
		if(error == boost::asio::error::eof)
		  break;
		else if(error)
		  throw boost::system::system_error(error);

		data += std::string(buf.begin(), buf.end());
	  }

	  // Convert to image
	  int channels, rows;
	  std::istringstream sstr(data);
	  sstr >> channels >> rows >> imgdata_str;
	  std::vector<uchar> imgdata(imgdata_str.begin(), imgdata_str.end());
	  
	  Mat img(imgdata, true);
	  img.reshape(channels, rows);

	  imshow("Image", img);
	}
  } catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
	return -1;
  }

  destroyAllWindows();
  return 0;
}
