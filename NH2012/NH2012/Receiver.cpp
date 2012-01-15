#include "Receiver.h"

Receiver::Receiver(boost::asio::io_service& io_service)
  : socket_(io_service)
{
}

/// Get the underlying socket. Used for making a connection or for accepting
/// an incoming connection.
boost::asio::ip::tcp::socket& Receiver::socket()
{
  return socket_;
}

/// Asynchronously write a data structure to the socket.
template <typename T, typename Handler>
void Receiver::async_write(const T& t, Handler handler)
{
  // Serialize the data first so we know how large it is.
  std::ostringstream archive_stream;
  boost::archive::text_oarchive archive(archive_stream);
  archive << t;
  outbound_data_ = archive_stream.str();

  // Format the header.
  std::ostringstream header_stream;
  header_stream << std::setw(header_length)
    << std::hex << outbound_data_.size();
  if (!header_stream || header_stream.str().size() != header_length)
  {
    // Something went wrong, inform the caller.
    boost::system::error_code error(boost::asio::error::invalid_argument);
    socket_.io_service().post(boost::bind(handler, error));
    return;
  }
  outbound_header_ = header_stream.str();

  // Write the serialized data to the socket. We use "gather-write" to send
  // both the header and the data in a single write operation.
  std::vector<boost::asio::const_buffer> buffers;
  buffers.push_back(boost::asio::buffer(outbound_header_));
  buffers.push_back(boost::asio::buffer(outbound_data_));
  boost::asio::async_write(socket_, buffers, handler);
}

/// Asynchronously read a data structure from the socket.
template <typename T, typename Handler>
void Receiver::async_read(T& t, Handler handler)
{
  // Issue a read operation to read exactly the number of bytes in a header.
  void (Receiver::*f)(
      const boost::system::error_code&,
      T&, boost::tuple<Handler>)
    = &Receiver::handle_read_header<T, Handler>;
  boost::asio::async_read(socket_, boost::asio::buffer(inbound_header_),
      boost::bind(f,
        this, boost::asio::placeholders::error, boost::ref(t),
        boost::make_tuple(handler)));
}

/// Handle a completed read of a message header. The handler is passed using
/// a tuple since boost::bind seems to have trouble binding a function object
/// created using boost::bind as a parameter.
template <typename T, typename Handler>
void Receiver::handle_read_header(const boost::system::error_code& e,
    T& t, boost::tuple<Handler> handler)
{
  if (e)
  {
    boost::get<0>(handler)(e);
  }
  else
  {
    // Determine the length of the serialized data.
    std::istringstream is(std::string(inbound_header_, header_length));
    std::size_t inbound_data_size = 0;
    if (!(is >> std::hex >> inbound_data_size))
    {
      // Header doesn't seem to be valid. Inform the caller.
      boost::system::error_code error(boost::asio::error::invalid_argument);
      boost::get<0>(handler)(error);
      return;
    }

    // Start an asynchronous call to receive the data.
    inbound_data_.resize(inbound_data_size);
    void (Receiver::*f)(
        const boost::system::error_code&,
        T&, boost::tuple<Handler>)
      = &Receiver::handle_read_data<T, Handler>;
    boost::asio::async_read(socket_, boost::asio::buffer(inbound_data_),
      boost::bind(f, this,
        boost::asio::placeholders::error, boost::ref(t), handler));
  }
}

/// Handle a completed read of message data.
template <typename T, typename Handler>
void Receiver::handle_read_data(const boost::system::error_code& e,
    T& t, boost::tuple<Handler> handler)
{
  if (e)
  {
    boost::get<0>(handler)(e);
  }
  else
  {
    // Extract the data structure from the data just received.
    try
    {
      std::string archive_data(&inbound_data_[0], inbound_data_.size());
      std::istringstream archive_stream(archive_data);
      boost::archive::text_iarchive archive(archive_stream);
      archive >> t;
    }
    catch (std::exception& e)
    {
      // Unable to decode data.
      boost::system::error_code error(boost::asio::error::invalid_argument);
      boost::get<0>(handler)(error);
      return;
    }

    // Inform caller that data has been received ok.
    boost::get<0>(handler)(e);
  }
}