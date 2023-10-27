#ifndef _NVMEMORY_H_
#define _NVMEMORY_H_

// we only have one implementation right now and nothing to choose from.
#define NVMEM_FLASH
//NVMEM_FLASH 
//NVMEM_SPIFFS

#include "../devices/device.h"
#include "storage.h"

// Handles load and store of user settings, except wifi credentials. Those are managed by the wifimanager.
class nvMemory
{
public: 
    nvMemory();
    ~nvMemory();
    bool saveConfig(TSettings* Settings);
    bool loadConfig(TSettings* Settings);
    bool deleteConfig();
private:
    bool init();
    bool Initialized_;
};

class StringX
{
public:
    StringX(int size)
    {
        ++size;
        str_ = new char[size];
        str_[0] = '\0';
        size_ = size;
    }
    ~StringX()
    {
        delete[] str_;
    }
    String getString()
    {
        return String(str_);
    }
    char* getBuffer()
    {
        return str_;
    }
    int getSize()
    {
        return size_;
    }
private:
    char* str_;
    int size_;
};

#if !defined(NVMEM_SPIFFS) && !defined(NVMEM_FLASH)
#error We need some kind of permanent storage implementation!
#endif //NVMEM_TYPE

#endif // _NVMEMORY_H_
