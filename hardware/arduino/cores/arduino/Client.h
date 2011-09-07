#ifndef client_h
#define client_h
#include "Print.h"
#include "Stream.h"
#include "IPAddress.h"

// Error codes for some of the network operations
// This isn't an exhaustive list, but provides some of the common errors 
const int ErrNoDNSServer = -1; // No DNS server is set
const int ErrDNSLookupFailed = -2;  // There was a problem making the DNS lookup
const int ErrHostNotFound = -3; // Couldn't resolve the hostname to an IP address
const int ErrTimedOut = -3; // The operation took too long and we've given up
const int ErrConnectionFailed = -4; // We couldn't connect to the host

class Client : public Stream {

public:
  // Connect to the specified port at the given address
  // Returns 1 if the connection was successfully created, else an error occurred
  virtual int connect(IPAddress ip, uint16_t port) =0;
  // Connect to the specified port at the given host (performing a DNS lookup first)
  // Returns 1 if the connection was successfully created, else an error occurred
  virtual int connect(const char *host, uint16_t port) =0;
  virtual size_t write(uint8_t) =0;
  virtual size_t write(const char *str) =0;
  virtual size_t write(const uint8_t *buf, size_t size) =0;
  virtual int available() = 0;
  virtual int read() = 0;
  virtual int read(uint8_t *buf, size_t size) = 0;
  virtual int peek() = 0;
  virtual void flush() = 0;
  virtual void stop() = 0;
  virtual uint8_t connected() = 0;
  virtual operator bool() = 0;
protected:
  uint8_t* rawIPAddress(IPAddress& addr) { return addr.raw_address(); };
};

#endif
