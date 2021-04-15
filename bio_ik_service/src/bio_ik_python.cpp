#include <Python.h>
#include <boost/python.hpp>
#include <moveit/py_bindings_tools/serialize_msg.h>
#include "bio_ik_service.cpp"

/**
 * Read a ROS message from a serialized string.
 */
template<typename M>
M from_python(const std::string &str_msg) {
  size_t serial_size = str_msg.size();
  boost::shared_array<uint8_t> buffer(new uint8_t[serial_size]);
  for (size_t i = 0; i < serial_size; ++i) {
    buffer[i] = str_msg[i];
  }
  ros::serialization::IStream stream(buffer.get(), serial_size);
  M msg;
  ros::serialization::Serializer<M>::read(stream, msg);
  return msg;
}

/**
 * Write a ROS message into a serialized string.
 */
template<typename M>
std::string to_python(const M &msg) {
  size_t serial_size = ros::serialization::serializationLength(msg);
  boost::shared_array<uint8_t> buffer(new uint8_t[serial_size]);
  ros::serialization::OStream stream(buffer.get(), serial_size);
  ros::serialization::serialize(stream, msg);
  std::string str_msg;
  str_msg.reserve(serial_size);
  for (size_t i = 0; i < serial_size; ++i) {
    str_msg.push_back(buffer[i]);
  }
  return str_msg;
}

moveit::py_bindings_tools::ByteString pyGetPositionIK(const std::string& request_str, bool approximate = false) {
  auto request = from_python<moveit_msgs::GetPositionIK::Request>(request_str);
  moveit_msgs::GetPositionIK::Response response;
  getPositionIK(request, response, approximate);
  return moveit::py_bindings_tools::serializeMsg(to_python<moveit_msgs::GetPositionIK::Response>(response));
}


BOOST_PYTHON_MODULE(bio_ik_python) {
  boost::python::def("getPositionIK", &pyGetPositionIK);
}
