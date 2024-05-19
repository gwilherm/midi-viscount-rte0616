#ifndef SERIAL_PRINTER_H
#define SERIAL_PRINTER_H

#include <cstddef>

class SerialPrinter {
  public:
    SerialPrinter() = default;
    virtual ~SerialPrinter() = default;

    size_t print(int);
    size_t print(const char*);
    size_t println(void);

};

extern SerialPrinter Serial;

#endif