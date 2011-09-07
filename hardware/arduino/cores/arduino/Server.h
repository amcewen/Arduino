#ifndef server_h
#define server_h

class Server {
public:
  virtual void begin() =0;
  // Derived classes MUST provide an available() method similar to the one shown below.
  // This isn't declared as a pure virtual function because the return type will vary with
  // each derived Server class
  //Client available();
};

#endif
