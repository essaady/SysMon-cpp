#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

class MemoryMonitor {
  private:

  unsigned long total_memory;
  unsigned long available_memory;
  bool readMemoryData();
  double calculateUsedPercent() const;

public:

  MemoryMonitor();
  bool update();

  unsigned long getTotalMemory() const;
  unsigned long getFreeMemory() const;
  unsigned long getUsedMemory() const;

  void display() const;


};

#endif