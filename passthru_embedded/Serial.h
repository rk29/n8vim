typedef struct SerialRegisters SerialRegisters;

class Serial
{
  private:
    SerialRegisters &regs;
  public:
    Serial(int portNum, long bps);
    // returns 1 if character received
    uint8_t get(uint8_t &b);
    // returns 1 if character transmitted
    uint8_t put(uint8_t b);
};
