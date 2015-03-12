#include <iostream>
#include <string>
#include <sstream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <opencv2/opencv.hpp>

#include <cvmat_serialization.h>

using namespace cv;
using boost::asio::ip::tcp;

int main() {
  try {

	boost::asio::io_service io_service;

	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 2015));

	namedWindow("test");
	
	while(true) {
	  tcp::socket socket(io_service);
	  acceptor.accept(socket);
	  std::stringstream sstr;

	  // Grab image
	  while(true) {
		boost::array<char, 128> buf;
		boost::system::error_code error;
		socket.read_some(boost::asio::buffer(buf), error);
		if(error == boost::asio::error::eof)
		  break;
		else if(error)
		  throw boost::system::system_error(error);
	  
		sstr << std::string(buf.begin(), buf.end());
	  }

	  boost::archive::text_iarchive data(sstr);

	  Mat img;
	  data >> img;

	  /*int size, channels, rows;
	  std::istringstream sstr(metadata);
	  sstr >> size >> channels >> rows;

	  std::cout << size << ' ' << channels << ' ' << rows << std::endl;
	  
	  // Grab image data
	  while(true) {
		boost::array<char, 128> buf;
		boost::system::error_code error;
		socket.read_some(boost::asio::buffer(buf), error);
		if(error == boost::asio::error::eof || imgdata.size() >= size)
		  break;
		else if(error)
		  throw boost::system::system_error(error);

		std::vector<uchar> vecbuf(buf.begin(), buf.end());
		imgdata.insert(imgdata.end(), vecbuf.begin(), vecbuf.end());

		std::cout << "Receiving data..." << std::endl;
	  }


	  std::vector<uchar> clipped_imgdata(imgdata.begin(), imgdata.begin() + size);
	  std::cout << rows << ' ' << clipped_imgdata.size() << std::endl;
	  
	  Mat img(clipped_imgdata, true);
	  img.reshape(channels, rows);
	  */
	  imwrite("test.jpg", img);
	  imshow("test", img);
	  std::cout << "image written" << std::endl; 
	}
  } catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
	return -1;
  }

  return 0;
}
