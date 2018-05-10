Message Types
=============

.. doxygenstruct:: cmakeserver::protocol::v1_2::message
	:members:
	:undoc-members:
.. doxygenstruct:: cmakeserver::protocol::v1_2::version
	:members:
	:undoc-members:
.. doxygenstruct:: cmakeserver::protocol::v1_2::hello
	:members:
	:undoc-members:
.. doxygenstruct:: cmakeserver::protocol::v1_2::handshake
	:members:
	:undoc-members:
.. doxygenstruct:: cmakeserver::protocol::v1_2::globalSettings
	:members:
	:undoc-members:
.. doxygenstruct:: cmakeserver::protocol::v1_2::configure
	:members:
	:undoc-members:
.. doxygenstruct:: cmakeserver::protocol::v1_2::compute
	:members:
	:undoc-members:
.. doxygenstruct:: cmakeserver::protocol::v1_2::codemodel
	:members:
	:undoc-members:
.. doxygenstruct:: cmakeserver::protocol::v1_2::ctestinfo
	:members:
	:undoc-members:
.. doxygenstruct:: cmakeserver::protocol::v1_2::cmakeinputs
	:members:
	:undoc-members:
.. doxygenstruct:: cmakeserver::protocol::v1_2::cache
	:members:
	:undoc-members:
.. doxygenstruct:: cmakeserver::protocol::v1_2::fileSystemWatchers
	:members:
	:undoc-members:

.. role:: cpp(code)
   :language: c++

iostream Support
----------------

All message types overload :cpp:`std::ostream operator<<(std::ostream &, message_type const&)`. They will be printed as a indented JSON document.

:cpp:`to_string` Support
------------------------

All message types overload :cpp:`std::string to_string(message_type const&)`. They will be printed as a JSON document without any indentation.

JSON Support
------------

All message types overload :cpp:`from_json(nlohmann::json const&, message_type &)` and :cpp:`to_json(nlohmann::json &, message_type const&)`.