#include "Serial Communications.h"

/*SerProt_GetSerialData Error Values*/
#define GSDNoErrors			0		//No Errors in getting the serial data
#define GSDNoSTX				1		//The STX char was not the first character received
#define GSDTimeOutError		2		//Function timed out waiting for a character
#define GSDCheckSumError	4		//The check sum did not check out

class CCOMProtocol : public CCOMPort
{
    public:
        CString* GetSerialData(unsigned char* Errors);
        void SendSerialData(CString* SerString);
    protected:

    private:
        char GetSerialCharacter(BOOL* TimedOut);
};
