
#pragma once

#include "OutputStream.hpp"

class DebugOutputStream : public OutputStream
{

protected:
public:
  DebugOutputStream();
  virtual ~DebugOutputStream() {}

  /* overrides from OutputStream */

  virtual bool write(uint8_t val) override;
  virtual bool write(const void* buffer, uint32_t size) override;

  virtual bool flush() override { return true; }

  virtual bool close() override { return true; }
};