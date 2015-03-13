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

	std::cout << "Server started..." << std::endl;
	
	namedWindow("test");
	
	while(true) {
	  tcp::socket socket(io_service);
	  acceptor.accept(socket);
	  std::cout << "Connected..." << std::endl;

	  size_t size;
	  std::stringstream sstr;

	  // Grab image
	  //	  while(true) {
	  {
		std::vector<char> buf(64);
		boost::system::error_code error;
		boost::asio::read(socket, boost::asio::buffer(buf));
		//socket.read_some(boost::asio::buffer(buf), error);
		//if(error == boost::asio::error::eof)
		//break;
		//if(error)
		  //throw boost::system::system_error(error);

		std::istringstream sstr2(std::string(buf.begin(), buf.end()));
		sstr2 >> size;
	  }

	  {
		std::vector<char> buf(size);
		boost::system::error_code error;
		boost::asio::read(socket, boost::asio::buffer(buf));
	  
		sstr.str(std::string(buf.begin(), buf.end()));
	  }

	  std::cout << "Received data has length " << sstr.str().length() << std::endl;
	  
	  boost::archive::text_iarchive data(sstr);

	  std::vector<uchar> imageBuf;
	  
	  Mat img;
	  data >> imageBuf;

	  img = imdecode(imageBuf, CV_LOAD_IMAGE_COLOR);

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
