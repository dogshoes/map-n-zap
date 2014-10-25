#include "mhcomm.h"

class CCOMPort : public CMhComm
{
    public:
        void SendSerialString(CString* String);
        char GetSerialChar();
    protected:

    private:

};